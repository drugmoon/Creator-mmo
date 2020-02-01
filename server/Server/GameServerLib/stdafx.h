#pragma once

#include <windows.h>
#include <shlwapi.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <math.h>

#include <time.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "UtilFun.h"
#include "UtilString.h"

#ifdef NDEBUG
#ifdef LIMIT_RELEASE
#define SCRIPT_EXP "lua"
#else
#define SCRIPT_EXP "lc"
#endif
#else
#define SCRIPT_EXP "lua"
#endif

#define LEUD_INT64 _int64
#include "../Sexylib/leudgrid/Performance.h"