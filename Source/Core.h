#ifndef CORE_H
#define CORE_H

#include "ClassicAssert.h"
#include "Data/Color.h"
#include "Data/Rectangle.h"
#include "Data/Transform.h"
#include "Data/Vector2.h"
#include "Input/InputManager.h"
#include "Themes/Themes.h"
#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Print.h"
#include "Utils/Resources.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/TimerManager.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "raylib.h"

#define TEXT(...) TextFormat(__VA_ARGS__)
#define TEXTBOOL(a) TextFormat("%s", (a) == true ? "true" : "false")

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