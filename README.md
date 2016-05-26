<!--
Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
-->

# SfinaeErrors 

<!-- <a target="_blank" href="https://travis-ci.org/badair/callable_traits">![Travis status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/project/badair/callable-traits">![Appveyor status][badge.Appveyor]</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/sTCnJr0lIF9sCAsf">![Try it online][badge.wandbox]</a> <a target="_blank" href="https://gitter.im/badair/callable_traits">![Gitter Chat][badge.Gitter]</a>
-->

A single C++ header file for SFINAE-able error messages. SFINAE-able error messages similar to `static_assert`, except that they are SFINAE-able. A library that uses SfinaeErrors lends itself to easy compile-time debugging, without giving up the flexibility of SFINAE.

```cpp
    
    #include <type_traits>
    #include "sfinae_errors.hpp"
    
    // An error "origin" tells users the template
    // which caused the problem 
    DEFINE_SFINAE_ERROR_ORIGIN(const_copyable)
    
    // T_must_be_const and T_must_be_copy_constructible are 
    // SFINAE "exceptions" that are associated with an origin
    DEFINE_SFINAE_ERROR_FOR(const_copyable, T_must_be_const)
    DEFINE_SFINAE_ERROR_FOR(const_copyable, T_must_be_copy_constructible)
        
    // This is an alias template that aliases T, but SFINAEs
    // when T is not const or when T is not copy constructible.
    // Note that if the aliased type is a template, it is not
    // instantiated until after the checks are performed.
    template<typename T>
    using const_copyable = sfinae_errors::sfinae_try< T,
        sfinae_errors::fail_if<!std::is_copy_constructible<T>::value, T_must_be_copy_constructible>,
        sfinae_errors::fail_if<!std::is_const<T>::value, T_must_be_const>
    >;
    
    int main(){
        
        // compiler error:
        //     no type named 'type' in 'error::const_copyable<T_must_be_const_>::_'
        
        const_copyable<int> i = 0;
        
        return i;
    }
    
```

## [License](LICENSE.md)

<!-- Links -->
[badge.Appveyor]: https://ci.appveyor.com/api/projects/status/uf0l91v7l4wc4kw6/branch/master?svg=true
[badge.Gitter]: https://img.shields.io/badge/gitter-join%20chat-blue.svg
[badge.Travis]: https://travis-ci.org/badair/callable_traits.svg?branch=master
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg

