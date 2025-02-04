#ifndef CLASSIC_ASSERT_H
#define CLASSIC_ASSERT_H

#ifdef _DEBUG
    #ifdef _MSC_VER  // MSVC
        #define CLASSIC_ASSERT(condition) if (!(condition)) __debugbreak()
    #else  // GCC/Clang
        #include <iostream>
        #define CLASSIC_ASSERT(condition) if (!(condition)) __builtin_trap()
    #endif
#endif

#endif