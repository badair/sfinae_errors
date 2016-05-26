<!--
Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
-->

# SfinaeErrors <a target="_blank" href="http://melpon.org/wandbox/permlink/eBn8swZQKzG1Hmmd">![Try it online][badge.wandbox]</a>

A single C++ header file for SFINAE-able error messages. Templated code with SFINAE-able error messages is easier to debug, and doesn't suffer from the inflexibility that comes with `static_assert`.

```cpp
    
    #include <type_traits>
    #include "sfinae_errors.hpp"
    
    // An error "origin" describes the source of the substitution failure
    DEFINE_SFINAE_ERROR_ORIGIN(const_copyable)
    
    // T_must_be_const and T_must_be_copy_constructible are 
    // SFINAE "exceptions", associated with const_copyable
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

## Compatibility

* Clang 3.4 and later
* GCC 4.7.3 and later
* Visual Studio 2015

Older platforms are untested.

## [License](LICENSE.md)

Distributed under the Boost software license.

<!-- Links -->
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg

