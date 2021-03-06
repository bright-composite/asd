//---------------------------------------------------------------------------

#pragma once

#ifndef META_FLAG_H
#define META_FLAG_H

//---------------------------------------------------------------------------

#include <type_traits>

//---------------------------------------------------------------------------

#ifndef _MSC_VER
namespace std
{
    template<class T>
    using underlying_type_t = typename underlying_type<T>::type;
}
#endif // _MSC_VER


namespace asd
{
    using std::declval;

    template<class F, class S>
    constexpr auto check_flag(F flag, S set) -> decltype((flag & set) == flag)
    {
        return static_cast<F>(flag & set) == flag;
    }

    template<class F, class S, typename = decltype(declval<S>() | declval<F>())>
    constexpr auto set_flag(F flag, S & set) -> S &
    {
        return set = static_cast<S>(set | flag);
    }

    template<class F, class S, typename = decltype(declval<S>() & ~declval<F>())>
    constexpr auto clear_flag(F flag, S & set) -> S &
    {
        return set = static_cast<S>(set & ~flag);
    }

    template<class F, class S, typename = decltype(declval<S>() | declval<F>()), typename = decltype(declval<S>() & ~declval<F>())>
    constexpr auto toggle_flag(F flag, S & set, bool state) -> S &
    {
        return set = (state ? static_cast<S>(set | flag) : static_cast<S>(set & ~flag));
    }
    
    template<class F, class S, typename = decltype(declval<S>() | declval<F>()), typename = decltype(declval<S>() & ~declval<F>())>
    constexpr auto toggle_flag(F flag, S & set) -> S &
    {
        return set = static_cast<S>(set ^ flag);
    }

    template<class F, class S>
    constexpr auto has_some_flags(F flags, S set) -> decltype((flags & set) != 0)
    {
        return (flags & set) != 0;
    }

    using std::underlying_type;
    using std::underlying_type_t;

#define adapt_enum_flags(Enum)																		\
    inline constexpr underlying_type_t<Enum> operator & (Enum a, Enum b)							\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) & static_cast<underlying_type_t<Enum>>(b);	\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator & (underlying_type_t<Enum> a, Enum b)			\
    {																								\
        return a & static_cast<underlying_type_t<Enum>>(b);											\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator & (Enum a, underlying_type_t<Enum> b)			\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) & b;											\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator | (Enum a, Enum b)							\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) | static_cast<underlying_type_t<Enum>>(b);	\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator | (underlying_type_t<Enum> a, Enum b)			\
    {																								\
        return a | static_cast<underlying_type_t<Enum>>(b);											\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator | (Enum a, underlying_type_t<Enum> b)			\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) | b;											\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator ^ (Enum a, Enum b)							\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) ^ static_cast<underlying_type_t<Enum>>(b);	\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator ^ (underlying_type_t<Enum> a, Enum b)			\
    {																								\
        return a ^ static_cast<underlying_type_t<Enum>>(b);											\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator ^ (Enum a, underlying_type_t<Enum> b)			\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) ^ b;											\
    }																								\
                                                                                                    \
    inline constexpr bool operator == (Enum a, underlying_type_t<Enum> b)							\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) == b;										\
    }																								\
                                                                                                    \
    inline constexpr bool operator == (int a, Enum b)												\
    {																								\
        return a == static_cast<underlying_type_t<Enum>>(b);										\
    }																								\
                                                                                                    \
    inline constexpr bool operator != (Enum a, underlying_type_t<Enum> b)							\
    {																								\
        return static_cast<underlying_type_t<Enum>>(a) != b;										\
    }																								\
                                                                                                    \
    inline constexpr bool operator != (underlying_type_t<Enum> a, Enum b)							\
    {																								\
        return a != static_cast<underlying_type_t<Enum>>(b);										\
    }																								\
                                                                                                    \
    inline constexpr underlying_type_t<Enum> operator ~ (Enum value)								\
    {																								\
        return ~static_cast<underlying_type_t<Enum>>(value);										\
    }

    template <class E>
    struct flags
    {
        using value_type = underlying_type_t<E>;
        
        flags() : value(0) {}
        flags(const flags & f) : value(f.value) {}
        flags(E flag) : value(static_cast<value_type>(flag)) {}
        
        flags & operator = (const flags & f) {
            value = f.value;
            return * this;
        }
        
        flags & operator = (E flag) {
            value = static_cast<value_type>(flag);
            return * this;
        }
        
        bool check(E flag) const {
            return check_flag(flag, value);
        }
        
        auto set(E flag) {
            return set_flag(flag, value);
        }
        
        auto clear(E flag) {
            return clear_flag(flag, value);
        }
        
        auto toggle(E flag) {
            return toggle_flag(flag, value);
        }
        
        auto toggle(E flag, bool state) {
            return toggle_flag(flag, value, state);
        }
        
        operator value_type () const {
            return value;
        }
        
    private:
        value_type value;
    };
}

//---------------------------------------------------------------------------
#endif
