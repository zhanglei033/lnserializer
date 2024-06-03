#include "lnsocket.h"
#include "lnassert.h"

#define TAG "lnsocket"
#if LN_PLATFORM_WIN32
static std::atomic_flag g_wsa_inited;

void WSAInit() LN_NOEXCEPT
{
    if (!g_wsa_inited.test_and_set())
    {
        WSADATA wsadata;
        int     result = WSAStartup(MAKEWORD(2, 2), &wsadata);
        LN_ASSERT(result == 0, "Failed to initialize Winsock");
    }
}

void WSADeinit() LN_NOEXCEPT
{
    if (g_wsa_inited.test_and_set())
    {
        g_wsa_inited.clear();
        WSACleanup();
    }
}
#endif

static inline LN_NODISCARD int lnsocket_errno_negative(int fd) LN_NOEXCEPT
{
    int err = lnsocket_errno();
    if (fd >= 0) closesocket(fd);
    return err > 0 ? -err : -1;
}

// socket -> setsockopt -> bind
static int lnsocket_bind_impl(lnsockaddr_u* localaddr, int type) LN_NOEXCEPT
{
    int fd = (int)socket(localaddr->sa.sa_family, type, 0);
    if (fd < 0)
    {
        int err = lnsocket_errno_negative(fd);
        LN_LOGI(TAG, "socket failed: {}.", err);
        return err;
    }
    lnsocket_reuse_addr(fd, 1);
    if (localaddr->sa.sa_family == AF_INET6)
    {
        lnsocket_ip_v6only(fd, 0);
    }
    if (bind(fd, &localaddr->sa, lnsocket_addr_len(localaddr)) < 0)
    {
        int err = lnsocket_errno_negative(fd);
        LN_LOGI(TAG, "bind failed: {}.", err);
        return err;
    }
    return fd;
}

static int lnsocket_listen_impl(int fd) LN_NOEXCEPT
{
    if (fd < 0) return fd;
    if (listen(fd, SOMAXCONN) < 0)
    {
        int err = lnsocket_errno_negative(fd);
        LN_LOGI(TAG, "listen failed: {}.", err);
        return err;
    }
    return fd;
}

// socket -> lnsocket_nonblocking -> connect
static int xsockaddr_connect_impl(lnsockaddr_u* peeraddr, int nonblocking) LN_NOEXCEPT
{
    int ret    = 0;
    int connfd = (int)socket(peeraddr->sa.sa_family, SOCK_STREAM, 0);
    if (connfd < 0)
    {
        int err = lnsocket_errno_negative(connfd);
        LN_LOGI(TAG, "socket failed: {}.", err);
        return err;
    }

    if (nonblocking) lnsocket_nonblocking(connfd);

    ret = connect(connfd, &peeraddr->sa, lnsocket_addr_len(peeraddr));

    if (ret < 0 && lnsocket_errno() != EINPROGRESS)
    {
        int err = lnsocket_errno_negative(connfd);
        LN_LOGI(TAG, "connect failed: {}.", err);
        return err;
    }

    return connfd;
}

static int lnsocket_connect_timeout_impl(int connfd, int ms) LN_NOEXCEPT
{
    int            err    = 0;
    socklen_t      optlen = sizeof(err);
    struct timeval tv     = {ms / 1000, (ms % 1000) * 1000};
    fd_set         writefds;
    FD_ZERO(&writefds);
    FD_SET(connfd, &writefds);
    int ret = select(connfd + 1, 0, &writefds, 0, &tv);
    if (ret < 0)
    {
        int err = lnsocket_errno_negative(connfd);
        LN_LOGI(TAG, "select failed: {}.", err);
        return err;
    }
    if (ret == 0)
    {
        errno   = ETIMEDOUT;
        int err = lnsocket_errno_negative(connfd);
        LN_LOGI(TAG, "select failed: {}.", err);
        return err;
    }
    if (getsockopt(connfd, SOL_SOCKET, SO_ERROR, (char*)&err, &optlen) < 0 || err != 0)
    {
        if (err != 0) errno = err;
        int err = lnsocket_errno_negative(connfd);
        LN_LOGI(TAG, "getsockopt SO_ERROR failed: {}.", err);
        return err;
    }
    lnsocket_blocking(connfd);
    return connfd;
}

bool lnsocket_host_is_ipv4(const std::string& host) LN_NOEXCEPT
{
    struct sockaddr_in sin;
    return inet_pton(AF_INET, host.data(), &sin) == 1;
}

bool lnsocket_host_is_ipv6(const std::string& host) LN_NOEXCEPT
{
    struct sockaddr_in6 sin6;
    return inet_pton(AF_INET6, host.data(), &sin6) == 1;
}

bool lnsocket_host_is_ipaddr(const std::string& host) LN_NOEXCEPT
{
    return lnsocket_host_is_ipv4(host) || lnsocket_host_is_ipv6(host);
}

int lnsocket_host_resolve_addr(const std::string& host, lnsockaddr_u* addr) LN_NOEXCEPT
{
#ifdef LN_PLATFORM_WIN32
    WSAInit();
#endif
    if (inet_pton(AF_INET, host.data(), &addr->sin.sin_addr) == 1)
    {
        addr->sa.sa_family = AF_INET; // host is ipv4
        return 0;
    }

    if (inet_pton(AF_INET6, host.data(), &addr->sin6.sin6_addr) == 1)
    {
        addr->sa.sa_family = AF_INET6; // host is ipv6
    }

    struct addrinfo* ais = nullptr;
    int              ret = getaddrinfo(host.data(), nullptr, nullptr, &ais);
    if (ret != 0 || ais == nullptr || ais->ai_addr == nullptr || ais->ai_addrlen == 0)
    {
        LN_LOGE(TAG, "unknown host: {} err:{}", host, ret);
        return ret;
    }

    struct addrinfo* pai = ais;
    while (pai != nullptr)
    {
        if (pai->ai_family == AF_INET) break;
        pai = pai->ai_next;
    }
    if (pai == nullptr) pai = ais;
    memcpy(addr, pai->ai_addr, pai->ai_addrlen);
    freeaddrinfo(ais);
    return 0;
}

int lnsocket_errno() LN_NOEXCEPT
{
#if LN_PLATFORM_WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

std::string lnsocket_addr_ip(lnsockaddr_u* addr) LN_NOEXCEPT
{
    if (addr->sa.sa_family == AF_INET)
    {
        char ip[g_lnsocket_ipv4_len] = {0};
        inet_ntop(AF_INET, &addr->sin.sin_addr, ip, g_lnsocket_ipv4_len);
        return std::string(ip, g_lnsocket_ipv4_len);
    }
    else if (addr->sa.sa_family == AF_INET6)
    {
        char ip[g_lnsocket_ipv6_len] = {0};
        return inet_ntop(AF_INET6, &addr->sin6.sin6_addr, ip, g_lnsocket_ipv6_len);
        return std::string(ip, g_lnsocket_ipv6_len);
    }
    return std::string();
}

uint16_t lnsocket_addr_port(lnsockaddr_u* addr) LN_NOEXCEPT
{
    uint16_t port = 0;
    if (addr->sa.sa_family == AF_INET)
    {
        port = ntohs(addr->sin.sin_port);
    }
    else if (addr->sa.sa_family == AF_INET6)
    {
        port = ntohs(addr->sin6.sin6_port);
    }
    return port;
}

int lnsocket_addr_set_ip(lnsockaddr_u* addr, const std::string& host) LN_NOEXCEPT
{
    if (!host.empty())
    {
        addr->sin.sin_family      = AF_INET;
        addr->sin.sin_addr.s_addr = htonl(INADDR_ANY);
        return 0;
    }
    return lnsocket_host_resolve_addr(host, addr);
}

void lnsocket_addr_set_port(lnsockaddr_u* addr, int port) LN_NOEXCEPT
{
    if (addr->sa.sa_family == AF_INET)
    {
        addr->sin.sin_port = htons(port);
    }
    else if (addr->sa.sa_family == AF_INET6)
    {
        addr->sin6.sin6_port = htons(port);
    }
}

int lnsocket_addr_set_ipport(lnsockaddr_u* addr, const std::string& host, int port) LN_NOEXCEPT
{
    int ret = lnsocket_addr_set_ip(addr, host);
    if (ret != 0) return ret;
    lnsocket_addr_set_port(addr, port);
    return 0;
}

socklen_t lnsocket_addr_len(lnsockaddr_u* addr) LN_NOEXCEPT
{
    if (addr->sa.sa_family == AF_INET)
    {
        return sizeof(struct sockaddr_in);
    }
    else if (addr->sa.sa_family == AF_INET6)
    {
        return sizeof(struct sockaddr_in6);
    }
    return sizeof(lnsockaddr_u);
}

std::string lnsocket_addr_str(lnsockaddr_u* addr) LN_NOEXCEPT
{
    if (addr->sa.sa_family == AF_INET)
    {
        char     ip[g_lnsocket_ipv4_len] = {0};
        uint16_t port                    = 0;
        inet_ntop(AF_INET, &addr->sin.sin_addr, ip, g_lnsocket_ipv4_len);
        port = ntohs(addr->sin.sin_port);
        return fmt::format("{}:{}", ip, port);
    }
    else if (addr->sa.sa_family == AF_INET6)
    {
        char     ip[g_lnsocket_ipv6_len] = {0};
        uint16_t port                    = 0;
        inet_ntop(AF_INET6, &addr->sin6.sin6_addr, ip, g_lnsocket_ipv6_len);
        port = ntohs(addr->sin6.sin6_port);
        return fmt::format("{}:{}", ip, port);
    }
    return {};
}

void lnsocket_addr_print(lnsockaddr_u* addr) LN_NOEXCEPT
{
    auto addr_str = lnsocket_addr_str(addr);
    LN_LOGI(TAG, "{}", addr_str);
}

int lnsocket_bind(int port, const std::string& host, int type) LN_NOEXCEPT
{
    lnsockaddr_u localaddr = {0};
    int          ret       = lnsocket_addr_set_ipport(&localaddr, host, port);
    if (ret != 0) return std::abs(ret);
    return lnsocket_bind_impl(&localaddr, type);
}

int lnsocket_listen(int port, const std::string& host) LN_NOEXCEPT
{
    int fd = lnsocket_bind(port, host, SOCK_STREAM);
    if (fd < 0) return fd;
    return lnsocket_listen_impl(fd);
}

int lnsocket_connect(const std::string& host, int port, int nonblocking) LN_NOEXCEPT
{
    lnsockaddr_u peeraddr = {0};
    int          ret      = lnsocket_addr_set_ipport(&peeraddr, host, port);
    if (ret != 0) return std::abs(ret);
    return xsockaddr_connect_impl(&peeraddr, nonblocking);
}

int lnsocket_connect_nonblock(const std::string& host, int port) LN_NOEXCEPT
{
    return lnsocket_connect(host, port, 0);
}

int lnsocket_connect_timeout(const std::string& host, int port, int ms) LN_NOEXCEPT
{
    int connfd = lnsocket_connect(host, port, 1);
    if (connfd < 0) return connfd;
    return lnsocket_connect_timeout_impl(connfd, ms);
}

int lnsocket_send_timeout(int fd, int timeout) LN_NOEXCEPT
{
    struct timeval tv = {timeout / 1000, (timeout % 1000) * 1000};
    return setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
}

int lnsocket_recv_timeout(int fd, int timeout) LN_NOEXCEPT
{
    struct timeval tv = {timeout / 1000, (timeout % 1000) * 1000};
    return setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
}

int lnsocket_send_buf_size(int fd, int size) LN_NOEXCEPT
{
    return setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(int));
}

int lnsocket_recv_buf_size(int fd, int size) LN_NOEXCEPT
{
    return setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int));
}

int lnsocket_reuse_addr(int fd, int on) LN_NOEXCEPT
{
#ifdef SO_REUSEADDR
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(int));
#else
    return 0;
#endif
}
int lnsocket_reuse_port(int fd, int on) LN_NOEXCEPT
{
#ifdef SO_REUSEPORT
    return setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(int));
#else
    return 0;
#endif
}

int lnsocket_udp_broadcast(int fd, int on) LN_NOEXCEPT
{
#ifdef SO_BROADCAST
    return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(int));
#else
    return 0;
#endif
}

int lnsocket_ip_v6only(int fd, int on) LN_NOEXCEPT
{
#ifdef IPV6_V6ONLY
    return setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&on, sizeof(int));
#else
    return 0;
#endif
}

int lnsocket_bind_addr(int fd, const char* local_host, int local_port) LN_NOEXCEPT
{
    lnsockaddr_u addr = {0};
    int          ret  = lnsocket_addr_set_ipport(&addr, local_host, local_port);
    if (ret != 0) return -1;

    if (bind(fd, &addr.sa, lnsocket_addr_len(&addr)) < 0)
    {
        int err = lnsocket_errno_negative(fd);
        LN_LOGI(TAG, "failed to bind host({}) port({}) (err){}.", local_host, local_port, err);
        return err;
    }
    return 0;
}

lnsocket_type_e lnsocket_get_type(int fd) LN_NOEXCEPT
{
    socklen_t optlen = sizeof(int);
    int       type;

    int ret = getsockopt(fd, SOL_SOCKET, SO_TYPE, (char*)&type, &optlen);

    if (ret == 0)
    {
        switch (type)
        {
        case SOCK_STREAM: return LN_SOCKET_TYPE_TCP;
        case SOCK_DGRAM: return LN_SOCKET_TYPE_UDP;
        case SOCK_RAW: return LN_SOCKET_TYPE_IP;
        default: return LN_SOCKET_TYPE_SOCKET;
        }
    }
    else
    {
        if (lnsocket_errno() == ENOTSOCK)
        {
            switch (type)
            {
            case 0: return LN_SOCKET_TYPE_STDIN;
            case 1: return LN_SOCKET_TYPE_STDOUT;
            case 2: return LN_SOCKET_TYPE_STDERR;
            default: return LN_SOCKET_TYPE_FILE;
            }
        }
        else
        {
            return LN_SOCKET_TYPE_UNKNOWN;
        }
    }
}

int lnsocket_linger(int fd, int timeout) LN_NOEXCEPT
{
#ifdef SO_LINGER
    struct linger linger;
    if (timeout >= 0)
    {
        linger.l_onoff  = 1;
        linger.l_linger = timeout;
    }
    else
    {
        linger.l_onoff  = 0;
        linger.l_linger = 0;
    }
    // NOTE: SO_LINGER change the default behavior of close, send RST, avoid TIME_WAIT
    return setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
#else
    return 0;
#endif
}

int lnsocket_close(int fd) LN_NOEXCEPT
{
    int ret = 0;
    if (fd >= 0)
    {
        ret = closesocket(fd);
        fd  = -1;
    }
    return ret;
}

int lnsocket_tcp_nodelay(int fd, int on) LN_NOEXCEPT
{
#ifdef TCP_NODELAY
    return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(int));
#else
    return 0;
#endif
}

int lnsocket_tcp_nopush(int fd, int on) LN_NOEXCEPT
{
#ifdef TCP_NOPUSH
    return setsockopt(fd, IPPROTO_TCP, TCP_NOPUSH, (const char*)&on, sizeof(int));
#elif defined(TCP_CORK)
    return setsockopt(fd, IPPROTO_TCP, TCP_CORK, (const char*)&on, sizeof(int));
#else
    return 0;
#endif
}

int lnsocket_tcp_keepalive(int fd, int on, int delay) LN_NOEXCEPT
{
#ifdef TCP_KEEPALIVE
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&on, sizeof(int)) != 0)
    {
        return lnsocket_errno();
    }
#else
    return 0;
#endif

#ifdef TCP_KEEPALIVE
    return setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, (const char*)&delay, sizeof(int));
#elif defined(TCP_KEEPIDLE)
    // TCP_KEEPIDLE     => tcp_keepalive_time
    // TCP_KEEPCNT      => tcp_keepalive_probes
    // TCP_KEEPINTVL    => tcp_keepalive_intvl
    return setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, (const char*)&delay, sizeof(int));
#else
    return 0;
#endif
}

int lnsocket_create_socket(const char* host, int port, lnsocket_type_e type, lnsocket_side_e side, lnsockaddr_u* addr) LN_NOEXCEPT
{
#if LN_PLATFORM_WIN32
    WSAInit();
#endif
    int sock_type = (type & LN_SOCKET_TYPE_SOCK_STREAM) ? SOCK_STREAM :
                    (type & LN_SOCKET_TYPE_SOCK_DGRAM)  ? SOCK_DGRAM :
                    (type & LN_SOCKET_TYPE_SOCK_RAW)    ? SOCK_RAW :
                                                          -1;
    if (sock_type == -1) return -1;

    int ret = -1;
    if (port >= 0)
    {
        ret = lnsocket_addr_set_ipport(addr, host, port);
    }
    if (ret != 0) return -1;

    int fd = (int)socket(addr->sa.sa_family, sock_type, IPPROTO_IP);
    if (fd < 0)
    {
        LN_LOGE(TAG, "create socket failed: {}.", lnsocket_errno());
        return -1;
    }

    if (side == LN_SOCKET_SIDE_SERVER)
    {
#ifndef LN_PLATFORM_WIN32
        lnsocket_reuse_addr(fd, 1);
#endif
        if (addr->sa.sa_family == AF_INET6)
        {
            lnsocket_ip_v6only(fd, 0);
        }
        if (bind(fd, &addr->sa, lnsocket_addr_len(addr)) < 0)
        {
            LN_LOGE(TAG, "bind addr failed: {}.", lnsocket_errno());
            closesocket(fd);
            return -1;
        }
        if (sock_type == SOCK_STREAM)
        {
            if (listen(fd, SOMAXCONN) < 0)
            {
                LN_LOGE(TAG, "listen failed: {}.", lnsocket_errno());
                closesocket(fd);
                return -1;
            }
        }
    }

    return fd;
}

int lnsocket_read(int fd, lnsocket_type_e type, void* buf, int len, void* ssl, sockaddr* peeraddr, socklen_t* addrlen) LN_NOEXCEPT
{
    int nread = 0;
    switch (type)
    {
    case LN_SOCKET_TYPE_SSL:
        // TODO: ssl
        // nread = xssl_read(io->ssl, buf, len);
        break;
    case LN_SOCKET_TYPE_TCP:
        nread = recv(fd, (char*)buf, len, 0);
        break;
    case LN_SOCKET_TYPE_UDP:
    case LN_SOCKET_TYPE_KCP:
    case LN_SOCKET_TYPE_IP:
        nread = recvfrom(fd, (char*)buf, (int)len, 0, peeraddr, addrlen);
        break;
    default:
        nread = read(fd, buf, (uint32_t)len);
        break;
    }
    return nread;
}

int lnsocket_write(int fd, lnsocket_type_e type, const void* buf, size_t len, void* ssl, sockaddr* peeraddr) LN_NOEXCEPT
{
    int nwrite = 0;
    switch (type)
    {
    case LN_SOCKET_TYPE_SSL:
        // TODO: ssl
        // nread = hssl_write(io->ssl, buf, len);
        break;
    case LN_SOCKET_TYPE_TCP:
        // TODO: recv
        // int flag = 0;
        // #ifdef MSG_NOSIGNAL
        // flag |= MSG_NOSIGNAL;
        // #endif
        // nread = send(io->fd, buf, len, flag);
        break;
    case LN_SOCKET_TYPE_UDP:
    case LN_SOCKET_TYPE_KCP:
    case LN_SOCKET_TYPE_IP:
        nwrite = sendto(fd, (const char*)buf, (int)len, 0, peeraddr, sizeof(sockaddr));
        break;
    default:
        nwrite = write(fd, buf, (uint32_t)len);
        break;
    }
    return nwrite;
}

int lnsocket_create_pair(int family, int type, int protocol, int* fds) LN_NOEXCEPT
{
#if LN_HAS_SYSEVNETFD
    int efd = eventfd(0, 0);
    if (efd < 0)
    {
        LN_LOGE(TAG, "eventfd create failed!");
        return -1;
    }
    fds[LN_SOCKET_PAIR_READ_INDEX] = fds[LN_SOCKET_PAIR_WRITE_INDEX] = efd;
    return 0;
#elif LN_HAS_UNISTD && !LN_PLATFORM_ESP32
    if (pipe(fds) != 0)
    {
        LN_LOGE(TAG, "eventfd create failed!");
        return -1;
    }
#elif LN_HAS_SYSSOCKET && !LN_PLATFORM_ESP32
    return socketpair(AF_LOCAL, type, protocol, fds);
#endif
    if (family != AF_INET || type != SOCK_STREAM)
    {
        return -1;
    }
#if LN_PLATFORM_WIN32
    WSAInit();
#endif

    static uint16_t port = 16888;
    port++;
    int         listenfd      = -1;
    int         connectfd     = -1;
    int         acceptfd      = -1;
    sockaddr_in localaddr     = {0};
    socklen_t   addrlen       = sizeof(sockaddr_in);
    localaddr.sin_family      = AF_INET;
    localaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    localaddr.sin_port        = htons(port);
    listenfd                  = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listenfd < 0)
    {
        LN_LOGE(TAG, "socket create failed: {}.", lnsocket_errno());
        goto label_error;
    }

    if (bind(listenfd, (struct sockaddr*)&localaddr, addrlen) < 0)
    {
        LN_LOGE(TAG, "socket bind failed: {}.", lnsocket_errno());
        goto label_error;
    }

    if (listen(listenfd, 1) < 0)
    {
        LN_LOGE(TAG, "socket listen failed: {}.", lnsocket_errno());
        goto label_error;
    }

    if (getsockname(listenfd, (struct sockaddr*)&localaddr, &addrlen) < 0)
    {
        LN_LOGE(TAG, "socket getsockname failed: {}.", lnsocket_errno());
        goto label_error;
    }

    // connector
    connectfd = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (connectfd < 0)
    {
        LN_LOGE(TAG, "socket create failed: {}.", lnsocket_errno());
        goto label_error;
    }

    if (connect(connectfd, (struct sockaddr*)&localaddr, addrlen) < 0)
    {
        LN_LOGE(TAG, "socket connect failed: {}.", lnsocket_errno());
        goto label_error;
    }

    // acceptor
    acceptfd = (int)accept(listenfd, (struct sockaddr*)&localaddr, &addrlen);
    if (acceptfd < 0)
    {
        LN_LOGE(TAG, "socket accept failed: {}.", lnsocket_errno());
        goto label_error;
    }

    closesocket(listenfd);
    fds[g_lnsocket_pair_read_index]  = connectfd;
    fds[g_lnsocket_pair_write_index] = acceptfd;
    return 0;

label_error:
    if (listenfd != -1)
    {
        closesocket(listenfd);
    }

    if (connectfd != -1)
    {
        closesocket(connectfd);
    }

    if (acceptfd != -1)
    {
        closesocket(acceptfd);
    }
    return -1;
}

void lnsocket_destory_pair(int* fds) LN_NOEXCEPT
{
#if LN_HAS_SYS_EVNETFD
    if (fds[0] >= 0)
    {
        close(fds[0]);
        fds[0] = -1;
    }
#elif LN_HAS_UNISTD
    if (fds[0] >= 0)
    {
        close(fds[0]);
        fds[0] = -1;
    }

    if (fds[1] >= 0)
    {
        close(fds[1]);
        fds[1] = -1;
    }
#elif LN_HAS_SYS_SOCKET
    if (fds[0] >= 0)
    {
        closesocket(fds[0]);
        fds[0] = -1;
    }
    if (fds[1] >= 0)
    {
        closesocket(fds[1]);
        fds[1] = -1;
    }
#endif
    fds[0] = fds[1] = -1;
}

int lnsocket_pair_write(int fd, const void* buf, int len) LN_NOEXCEPT
{
#if LN_HAS_SYS_EVNETFD || LN_HAS_UNISTD
    return write(fd, buf, len);
#else
    return send(fd, (const char*)buf, len, 0);
#endif
}