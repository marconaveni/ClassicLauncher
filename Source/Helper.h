#ifndef HELPER_H
#define HELPER_H

#include "Utils/Log.h"
#include "Utils/Print.h"
#include "Utils/String.h"

#define TEXT(...) String::TextFormat(__VA_ARGS__)
#define TEXTBOOL(a) String::TextFormat("%s", (a) == true ? "true" : "false")

#ifdef _DEBUG


//const char* text, float duration , const char* label , const Color& textColor, bool bLog = false
#define PRINT(...) GetPrint()->PrintOnScreen(__VA_ARGS__)
#else
#define PRINT(...)
#endif

// #define FORCE_THEME_1X  // force texture load debug 1x

#endif  // HELPER_H