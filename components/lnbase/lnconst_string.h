#pragma once
#include "lnplatform.h"

namespace ln {
template <class T, size_t N, class = std::enable_if_t<N != 0>>
struct const_string
{
    using value_type = T;
    using size_type  = size_t;
    value_type m_elems[N];

    template <size_type... I>
    constexpr const_string(const value_type (&elems)[N], std::index_sequence<I...>) LN_NOEXCEPT
        : m_elems{elems[I]...}
    {
    }

    template <size_t Prefix, size_t Suffix>
    LN_NODISCARD constexpr auto substr() const LN_NOEXCEPT
    {
        return substr_impl<Prefix>(std::make_index_sequence<N - Prefix - Suffix - 1>{});
    }

    friend std::ostream& operator<<(std::ostream& os, const const_string& str)
    {
        using ostr_type                   = std::basic_ostream<T, std::char_traits<T>>;
        typename ostr_type::iostate state = ostr_type::goodbit;

        size_t pad;
        if (os.width() <= 0 || static_cast<size_t>(os.width()) <= N)
        {
            pad = 0;
        }
        else
        {
            pad = static_cast<size_t>(os.width()) - N;
        }

        const typename ostr_type::sentry ok(os);

        if (!ok)
        {
            state |= ostr_type::badbit;
        }
        else
        {
            try
            {
                if ((os.flags() & ostr_type::adjustfield) != ostr_type::left)
                {
                    for (; 0 < pad; --pad)
                    {
                        if (std::char_traits<T>::eq_int_type(std::char_traits<T>::eof(), os.rdbuf()->sputc(os.fill())))
                        {
                            state |= ostr_type::badbit;
                            break;
                        }
                    }
                }

                if (state == ostr_type::goodbit && os.rdbuf()->sputn(str.m_elems, static_cast<std::streamsize>(N)) != static_cast<std::streamsize>(N))
                {
                    state |= ostr_type::badbit;
                }
                else
                {
                    for (; 0 < pad; --pad)
                    {
                        if (std::char_traits<T>::eq_int_type(std::char_traits<T>::eof(), os.rdbuf()->sputc(os.fill())))
                        {
                            state |= ostr_type::badbit;
                            break;
                        }
                    }
                }

                os.width(0);
            } catch (...)
            {
                os.setstate(ostr_type::badbit, true);
            }
        }

        os.setstate(state);
        return os;
    }

    LN_NODISCARD auto to_string() const
    {
        return std::basic_string_view<value_type>(m_elems, N);
    }

private:
    template <size_t Prefix, size_type... I>
    LN_NODISCARD constexpr auto substr_impl(std::index_sequence<I...>) const LN_NOEXCEPT
    {
        return const_string<value_type, N - Prefix>({m_elems[I + Prefix]..., '\0'}, std::make_index_sequence<N - Prefix>{});
    }
};

template <class T, size_t N>
LN_NODISCARD static constexpr auto const_string_create(const T (&elems)[N]) LN_NOEXCEPT
{
    return const_string<T, N>(elems, std::make_index_sequence<N>{});
}
} // namespace ln
