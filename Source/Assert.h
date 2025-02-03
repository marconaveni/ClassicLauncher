#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>

#define ASSERT(condition) if (!condition) __builtin_trap()

#endif