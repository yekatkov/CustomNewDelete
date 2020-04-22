# Overview

We attempt to build library which uses stl. Also we define private opeator 'new' and 'delete'. Visibility  of operators 'new' and 'delete' we achive by using of linker script libfoo.version.

I expect that stl will use our private operators new/delete, but there is two different situations which depends on optimization.

## Don't use inlining

**Building**

`mkdir build`  
`cd build`  
`cmake -DUSE_INLINE=FALSE ..`  
`cmake --build .`

If we launch example, we will see that we free exactly the same memory which we'd allocated. Everything are as expected.

    ./CustomNewDelete 
    malloc : 0x564262a18e70 : 0
    free : 0x564262a18e70 : 0

## Use inlining

**Building**

`mkdir build`  
`cd build`  
`cmake -DUSE_INLINE=TRUE ..`  
`cmake --build .`

    ./CustomNewDelete 
    malloc : 0x556673b62e70 : 0
    free : 0x556673b632ac : 0
    free(): invalid pointer
    Aborted (core dumped)

We can see that deallocation address differs from allocation one. It happens because of optimization. This allocation was called for object which was created with 'operator new' from libstdc++.so.

I will provide stack for calling new/delete here:

    #0  operator new (size=32) at CustomNewDelete/foo/new.cpp:32
    #1  0x00007ffff7bd39f5 in fillString (in=in@entry=0x7ffff7bd3ba8 \"stl::f\", 'o' <repeats 48 times>) at CustomNewDelete/foo/foo.cpp:6
    #2  0x00007ffff7bd3ace in foo () at CustomNewDelete/foo/foo.cpp:12
    #3  0x0000555555554739 in main () at CustomNewDelete/main.cpp:5

**Here we construct basic_sting with 'operator new' from libstdc++.so**

    #0  0x00007ffff74ec280 in operator new(unsigned long) () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
    #1  0x00007ffff7bd3a45 in std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char const*> (__end=0x7ffff7bd3bde \"\", __beg=0x7ffff7bd3ba8 \"stl::f\", 'o' <repeats 48 times>, this=0x555555767e74) at /usr/include/c++/5/bits/basic_string.tcc:219
    #2  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct_aux<char const*> (__end=0x7ffff7bd3bde \"\", __beg=0x7ffff7bd3ba8 \"stl::f\", 'o' <repeats 48 times>, this=0x555555767e74) at /usr/include/c++/5/bits/basic_string.h:195
    #3  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char const*> (__end=0x7ffff7bd3bde \"\", __beg=0x7ffff7bd3ba8 \"stl::f\", 'o' <repeats 48 times>, this=0x555555767e74) at /usr/include/c++/5/bits/basic_string.h:214
    #4  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string (__a=..., __s=0x7ffff7bd3ba8 \"stl::f\", 'o' <repeats 48 times>, this=0x555555767e74) at /usr/include/c++/5/bits/basic_string.h:457
    #5  fillString (in=<optimized out>, in@entry=0x7ffff7bd3ba8 \"stl::f\", 'o' <repeats 48 times>) at CustomNewDelete/foo/foo.cpp:6
    #6  0x00007ffff7bd3ace in foo () at CustomNewDelete/foo/foo.cpp:12
    #7  0x0000555555554739 in main () at CustomNewDelete/main.cpp:5

**But here we destruct the same object with our redefined 'operator delete'**

    #0  operator delete (ptr=0x555555768eb0) at CustomNewDelete/foo/new.cpp:38
    #1  0x00007ffff7bd3aee in __gnu_cxx::new_allocator<char>::deallocate (__p=<optimized out>, this=0x555555767e74) at /usr/include/c++/5/ext/new_allocator.h:110
    #2  std::allocator_traits<std::allocator<char> >::deallocate (__n=<optimized out>, __p=<optimized out>, __a=...) at /usr/include/c++/5/bits/alloc_traits.h:517
    #3  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_destroy (__size=<optimized out>, this=0x555555767e74) at /usr/include/c++/5/bits/basic_string.h:185
    #4  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_dispose (this=0x555555767e74) at /usr/include/c++/5/bits/basic_string.h:180
    #5  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string (this=0x555555767e74, __in_chrg=<optimized out>) at /usr/include/c++/5/bits/basic_string.h:559
    #6  foo () at CustomNewDelete/foo/foo.cpp:13
    #7  0x0000555555554739 in main () at CustomNewDelete/main.cpp:5