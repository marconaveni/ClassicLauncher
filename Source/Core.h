#ifndef CORE_H
#define CORE_H

#include "Utils/Math.h"
#include "Utils/Print.h"
#include "Utils/UtilsFunctionLibrary.h"
#include "Utils/StringFunctionLibrary.h"
#include "Data/Vector2.h"

#define TEXT(...) TextFormat(__VA_ARGS__)
#define TEXTBOOL(a) TextFormat("%s", a == true ? "true" : "false")

#endif