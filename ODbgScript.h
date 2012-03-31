#pragma once

#include <windows.h>
#include "ollydbg201.h"
#include "mru.h"
#include "OllyLang.h"

//#include "version.h"
//#include "mru.h"

//using namespace std;

//static wchar_t	wndprogclass[32];	   // Name of progress window class
//static wchar_t	wndlogclass[32];	   // Name of log window class
//static wchar_t	wndwatchclass[32];
extern int		focusonstop;             // Foreground on pause

extern bool		dbgfocus;

extern void*	pmemforexec;

extern t_menu mainmenu[];

extern MRU mru;

//extc _export HWND cdecl DebugScript(const char* const filename);

//For unique vars btw processes
//#pragma data_seg( ".GLOBALS" )
//#pragma data_seg()

// OllyLang object
extern OllyLang* ollylang;
