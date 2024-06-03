#pragma once
#include "lnplatform.h"

inline constexpr auto g_lnsocket_localhost               = "127.0.0.1";
inline constexpr auto g_lnsocket_anyaddr                 = "0.0.0.0";
inline constexpr auto g_lnsocket_ipv4_len                = sizeof("255.255.255.255");
inline constexpr auto g_lnsocket_ipv6_len                = sizeof("65535.65535.65535.65535.65535.65535.65535.65535");
inline constexpr auto g_lnsocket_port_len                = sizeof("65535");
inline constexpr auto g_lnsocket_ipv4_addr_len           = g_lnsocket_port_len + g_lnsocket_ipv4_len;
inline constexpr auto g_lnsocket_ipv6_addr_len           = g_lnsocket_port_len + g_lnsocket_ipv6_len;
inline constexpr auto g_lnsocket_connect_timeout_default = 10000; // ms
inline constexpr auto g_lnsocket_pair_read_index         = 0;
inline constexpr auto g_lnsocket_pair_write_index        = 1;

#if LN_PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET lnsocket_t;
typedef int    socklen_t;

void WSAInit() LN_NOEXCEPT;
void WSADeinit() LN_NOEXCEPT;

static inline LN_NODISCARD int lnsocket_blocking(int fd) LN_NOEXCEPT
{
    unsigned long nb = 0;
    return ioctlsocket(fd, FIONBIO, &nb);
}

static inline LN_NODISCARD int lnsocket_nonblocking(int fd) LN_NOEXCEPT
{
    unsigned long nb = 1;
    return ioctlsocket(fd, FIONBIO, &nb);
}

#undef EAGAIN
#undef EINPROGRESS
#undef EINTR
#undef ENOTSOCK
#undef EMSGSIZE

#define EAGAIN      WSAEWOULDBLOCK
#define EINPROGRESS WSAEINPROGRESS
#define EINTR       WSAEINTR
#define ENOTSOCK    WSAENOTSOCK
#define EMSGSIZE    WSAEMSGSIZE

#elif LN_PLATFORM_ESP32
typedef int lnsocket_t;
#ifndef SOCKET
#define SOCKET int
#endif
#define INVALID_SOCKET         -1

static inline LN_NODISCARD int lnsocket_blocking(int fd) LN_NOEXCEPT
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
}

static inline LN_NODISCARD int lnsocket_nonblocking(int fd) LN_NOEXCEPT
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

#define LN_NET_SOCKET_MAX_SIZE CONFIG_LWIP_MAX_SOCKETS
#define LN_NET_SOCKET_OFFSET   LWIP_SOCKET_OFFSET

#else
typedef int lnsocket_t;
#ifndef SOCKET
#define SOCKET int
#endif
#define INVALID_SOCKET -1

static inline LN_NODISCARD int lnsocket_blocking(int fd) LN_NOEXCEPT
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
}

static inline LN_NODISCARD int lnsocket_nonblocking(int fd) LN_NOEXCEPT
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

static inline LN_NODISCARD int closesocket(int fd) LN_NOEXCEPT
{
    return close(fd);
}

#endif

typedef struct sockaddr     sockaddr;
typedef struct sockaddr_in  sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;

union lnsockaddr_u
{
    sockaddr     sa;
    sockaddr_in  sin;
    sockaddr_in6 sin6;
};

enum lnsocket_type_e : uint16_t
{
    LN_SOCKET_TYPE_UNKNOWN     = 0,
    LN_SOCKET_TYPE_STDIN       = LN_BIT0,
    LN_SOCKET_TYPE_STDOUT      = LN_BIT1,
    LN_SOCKET_TYPE_STDERR      = LN_BIT2,
    LN_SOCKET_TYPE_STDIO       = LN_SOCKET_TYPE_STDIN | LN_SOCKET_TYPE_STDOUT | LN_SOCKET_TYPE_STDERR, // STDIO
    LN_SOCKET_TYPE_FILE        = LN_BIT3,                                                              // FILE
    LN_SOCKET_TYPE_IP          = LN_BIT4,
    LN_SOCKET_TYPE_SOCK_RAW    = LN_SOCKET_TYPE_IP, // SOCK RAW
    LN_SOCKET_TYPE_UDP         = LN_BIT5,
    LN_SOCKET_TYPE_KCP         = LN_BIT6,
    LN_SOCKET_TYPE_DTLS        = LN_BIT7,
    LN_SOCKET_TYPE_SOCK_DGRAM  = LN_SOCKET_TYPE_UDP | LN_SOCKET_TYPE_KCP | LN_SOCKET_TYPE_DTLS, // SOCK DGRAM
    LN_SOCKET_TYPE_TCP         = LN_BIT8,
    LN_SOCKET_TYPE_SSL         = LN_BIT9,
    LN_SOCKET_TYPE_TLS         = LN_SOCKET_TYPE_SSL,
    LN_SOCKET_TYPE_SOCK_STREAM = LN_SOCKET_TYPE_TCP | LN_SOCKET_TYPE_SSL | LN_SOCKET_TYPE_TLS, // SOCK STREAM
    LN_SOCKET_TYPE_SOCKET      = LN_SOCKET_TYPE_SOCK_RAW | LN_SOCKET_TYPE_SOCK_DGRAM | LN_SOCKET_TYPE_SOCK_STREAM,
};

enum lnsocket_side_e : uint8_t
{
    LN_SOCKET_SIDE_SERVER = 0,
    LN_SOCKET_SIDE_CLIENT = 1,
};

LN_NODISCARD bool lnsocket_host_is_ipv4(const std::string& host) LN_NOEXCEPT;

LN_NODISCARD bool lnsocket_host_is_ipv6(const std::string& host) LN_NOEXCEPT;

LN_NODISCARD bool lnsocket_host_is_ipaddr(const std::string& host) LN_NOEXCEPT;

LN_NODISCARD int lnsocket_host_resolve_addr(const std::string& host, lnsockaddr_u* addr) LN_NOEXCEPT;

LN_NODISCARD int lnsocket_errno() LN_NOEXCEPT;

LN_NODISCARD std::string lnsocket_addr_ip(lnsockaddr_u* addr) LN_NOEXCEPT;

LN_NODISCARD uint16_t lnsocket_addr_port(lnsockaddr_u* addr) LN_NOEXCEPT;

LN_NODISCARD int lnsocket_addr_set_ip(lnsockaddr_u* addr, const std::string& host) LN_NOEXCEPT;

void lnsocket_addr_set_port(lnsockaddr_u* addr, int port) LN_NOEXCEPT;

LN_NODISCARD int lnsocket_addr_set_ipport(lnsockaddr_u* addr, const std::string& host, int port) LN_NOEXCEPT;

LN_NODISCARD socklen_t lnsocket_addr_len(lnsockaddr_u* addr) LN_NOEXCEPT;

LN_NODISCARD std::string lnsocket_addr_str(lnsockaddr_u* addr) LN_NOEXCEPT;

void lnsocket_addr_print(lnsockaddr_u* addr) LN_NOEXCEPT;

// socket -> setsockopt -> bind
// @param type: SOCK_STREAM(tcp) SOCK_DGRAM(udp)
// @return fd
LN_NODISCARD int lnsocket_bind(int port, const std::string& host = g_lnsocket_anyaddr, int type = SOCK_STREAM) LN_NOEXCEPT;

// Bind -> listen
// @return listenfd
LN_NODISCARD int lnsocket_listen(int port, const std::string& host = g_lnsocket_anyaddr) LN_NOEXCEPT;

// ResolveAddr -> socket -> nonblocking -> connect
// @return connfd
LN_NODISCARD int lnsocket_connect(const std::string& host, int port, int nonblock = 0) LN_NOEXCEPT;

// connect(host, port, 1)
// @return connfd
LN_NODISCARD int lnsocket_connect_nonblock(const std::string& host, int port) LN_NOEXCEPT;

// connect(host, port, 1) -> select -> blocking
LN_NODISCARD int lnsocket_connect_timeout(const std::string& host, int port, int ms = g_lnsocket_connect_timeout_default) LN_NOEXCEPT;

// send timeout
int lnsocket_send_timeout(int fd, int timeout) LN_NOEXCEPT;

// recv timeout
int lnsocket_recv_timeout(int fd, int timeout) LN_NOEXCEPT;

// send buf size
int lnsocket_send_buf_size(int fd, int size) LN_NOEXCEPT;

// recv buf size
int lnsocket_recv_buf_size(int fd, int size) LN_NOEXCEPT;

// 允许重用TIME_WAIT状态的sockaddr
int lnsocket_reuse_addr(int fd, int on = 1) LN_NOEXCEPT;

// 允许多个套接字绑定同一端口
int lnsocket_reuse_port(int fd, int on = 1) LN_NOEXCEPT;

// 设置udp广播
int lnsocket_udp_broadcast(int fd, int on = 1) LN_NOEXCEPT;

// 仅ipv6可用
int lnsocket_ip_v6only(int fd, int on = 1) LN_NOEXCEPT;

// 绑定本地地址
int lnsocket_bind_addr(int fd, const char* local_host, int local_port) LN_NOEXCEPT;

// 读取fd的类型
LN_NODISCARD lnsocket_type_e lnsocket_get_type(int fd) LN_NOEXCEPT;

/*
 * @brief timeout == 0时
 *        立即关闭该连接，通过发送RST分组(而不是用正常的FIN|ACK|FIN|ACK四个分组)来关闭该连接。
 *        至于发送缓冲区中如果有未发送完的数据，则丢弃。主动关闭一方的TCP状态则跳过TIMEWAIT，直接进入CLOSED。
 *        timeout >= 0时
 *        将连接的关闭设置一个超时。如果socket发送缓冲区中仍残留数据，进程进入睡眠，内核进入定时状态去尽量去发送这些数据。
 *        在超时之前，如果所有数据都发送完且被对方确认，内核用正常的FIN | ACK | FIN | ACK四个分组来关闭该连接，close() 成功返回。
 *        如果超时之时，数据仍然未能成功发送及被确认，用上述a方式来关闭此连接。close() 返回EWOULDBLOCK。
 */
int lnsocket_linger(int fd, int timeout = 1) LN_NOEXCEPT;

int lnsocket_close(int fd) LN_NOEXCEPT;

int lnsocket_tcp_nodelay(int fd, int on = 1) LN_NOEXCEPT;

int lnsocket_tcp_nopush(int fd, int on = 1) LN_NOEXCEPT;

int lnsocket_tcp_keepalive(int fd, int on = 1, int delay = 60) LN_NOEXCEPT;

LN_NODISCARD int lnsocket_create_socket(const char* host, int port, lnsocket_type_e type, lnsocket_side_e side, lnsockaddr_u* addr) LN_NOEXCEPT;

#define LN_SOCKET_CREATE_TCP_CLIENT(host, port, addr) lnsocket_create_socket(host, port, LN_SOCKET_TYPE_TCP, LN_SOCKET_SIDE_CLIENT, addr)
#define LN_SOCKET_CREATE_TCP_SERVER(host, port, addr) lnsocket_create_socket(host, port, LN_SOCKET_TYPE_TCP, LN_SOCKET_SIDE_SERVER, addr)
#define LN_SOCKET_CREATE_SSL_CLIENT(host, port, addr) lnsocket_create_socket(host, port, LN_SOCKET_TYPE_SSL, LN_SOCKET_SIDE_CLIENT, addr);
#define LN_SOCKET_CREATE_SSL_SERVER(host, port, addr) lnsocket_create_socket(host, port, LN_SOCKET_TYPE_SSL, LN_SOCKET_SIDE_SERVER, addr);
#define LN_SOCKET_CREATE_UDP_CLIENT(host, port, addr) lnsocket_create_socket(host, port, LN_SOCKET_TYPE_UDP, LN_SOCKET_SIDE_CLIENT, addr);
#define LN_SOCKET_CREATE_UDP_SERVER(host, port, addr) lnsocket_create_socket(host, port, LN_SOCKET_TYPE_UDP, LN_SOCKET_SIDE_SERVER, addr);

int lnsocket_read(int fd, lnsocket_type_e type, void* buf, int len, void* ssl, sockaddr* peeraddr, socklen_t* addrlen) LN_NOEXCEPT;

int lnsocket_write(int fd, lnsocket_type_e type, const void* buf, size_t len, void* ssl, sockaddr* peeraddr) LN_NOEXCEPT;

/*
 * @brief 创建一组可互相通讯的fd
 *
 * @param family
 * @param type
 * @param protocol
 * @param fds int[2]
 * @return int
 */
int lnsocket_create_pair(int family, int type, int protocol, int* fds) LN_NOEXCEPT;

void lnsocket_destory_pair(int* fds) LN_NOEXCEPT;

int lnsocket_pair_write(int fd, const void* buf, int len) LN_NOEXCEPT;
