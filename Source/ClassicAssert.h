#ifndef CLASSIC_ASSERT_H
#define CLASSIC_ASSERT_H

#include <assert.h>

#ifdef _DEBUG
    #ifdef _MSC_VER  // MSVC
        #define CLASSIC_ASSERT(condition, msg) if (!(condition)) __debugbreak() 
    #else  // GCC/Clang
        #define CLASSIC_ASSERT(condition, msg) if (!(condition)) __builtin_trap() 
    #endif
#else
    #define CLASSIC_ASSERT(condition, msg) 
#endif

#endif