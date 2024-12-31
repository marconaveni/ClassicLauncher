#ifndef CORE_H
#define CORE_H

#include "Data/Color.h"
#include "Data/Rectangle.h"
#include "Data/TransformProperties.h"
#include "Data/Vector2.h"
#include "Input/InputManager.h"
#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Print.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/Timer.h"
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

#endif  // CORE_H