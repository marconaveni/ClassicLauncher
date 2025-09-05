#ifndef CORE_H
#define CORE_H


#define TEXT(...) rlw::TextFormat(__VA_ARGS__)
#define TEXTBOOL(a) rlw::TextFormat("%s", (a) == true ? "true" : "false")

#ifdef _DEBUG
/* const char* text,
 * float duration ,
 * const char* label ,
 * const Color& textColor,
 * bool bLog = false);
 */
#define PRINT(...) Application::Get().GetPrint()->PrintOnScreen(__VA_ARGS__)
#else
#define PRINT(...)
#endif

// #define FORCE_THEME_1X  // force texture load debug 1x

#endif  // CORE_H