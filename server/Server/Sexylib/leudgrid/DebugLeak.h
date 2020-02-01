#pragma once

#include <crtdbg.h>

#ifdef _DEBUG
#define DUMP_LEAK	_CrtSetDbgFlag((_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & 0x0000FFFF) | _CRTDBG_LEAK_CHECK_DF);
#else
#define DUMP_LEAK
#endif

//_CrtSetBreakAlloc