#ifndef CORE_H
#define CORE_H

#include "Data/Color.h"
#include "Data/Rectangle.h"
#include "Data/Vector2.h"
#include "Data/TransformProperties.h"
#include "Utils/Math.h"
#include "Utils/Print.h"
#include "Utils/StringFunctionLibrary.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "raylib.h"

#define TEXT(...) TextFormat(__VA_ARGS__)
#define TEXTBOOL(a) TextFormat("%s", a == true ? "true" : "false")

#endif