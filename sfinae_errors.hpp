/*!
@copyright Barrett Adair 2016

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef SFINAE_ERRORS_HPP
#define SFINAE_ERRORS_HPP

#include <type_traits>

namespace sfinae_errors {

    namespace detail {

        struct sfinae_error{};

        template<typename T>
        struct success {
            static constexpr bool value = true;
            struct _ { using type = T; };
        };
        
        #ifdef __cpp_lib_logical_traits
        #define SFINAE_ERRORS_DISJUNCTION(...) ::std::disjunction< __VA_ARGS__ >
        #else
        #define SFINAE_ERRORS_DISJUNCTION(...) ::sfinae_errors::detail::disjunction< __VA_ARGS__ >
        
        //polyfill for C++17 std::disjunction
        template<typename...>
            struct disjunction
                : std::false_type {};

        template<typename T>
            struct disjunction<T>
                : T {};

        template<typename T, typename... Ts>
            struct disjunction<T, Ts...>
                : std::conditional<T::value != false, T, disjunction<Ts...>>::type {};
        
        #endif //#ifdef __cpp_lib_logical_traits
    }

    template<bool B, typename T>
    struct fail_if : T {

        static_assert(std::is_base_of<detail::sfinae_error, T>::value,
            "incorrect usage of fail_if");

        static constexpr bool value = B;
    };

    template<typename T, typename... ThrowIfs>
    using sfinae_try = typename SFINAE_ERRORS_DISJUNCTION(
        ThrowIfs...,
        detail::success<T>
    )::_::type;
}

#define SFINAE_ERRORS_PP_CAT_(x, y) x ## y
#define SFINAE_ERRORS_PP_CAT(x, y) SFINAE_ERRORS_PP_CAT_(x, y)

#define DEFINE_SFINAE_ERROR_ORIGIN(origin)    \
namespace error {                             \
    template<typename ErrorMessage>           \
    struct origin :                           \
        ::sfinae_errors::detail::sfinae_error \
        { struct _ {}; };                     \
}                                             \
/**/

#define DEFINE_SFINAE_ERROR_FOR(origin, name)                  \
struct SFINAE_ERRORS_PP_CAT(name, _ ){};                       \
struct name : error::origin<SFINAE_ERRORS_PP_CAT(name, _ )>{}; \
/**/

#endif // #ifndef SFINAE_ERRORS_HPP