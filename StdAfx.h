// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2DDAD7E2_6051_11D2_B760_00C0F02907C3__INCLUDED_)
#define AFX_STDAFX_H__2DDAD7E2_6051_11D2_B760_00C0F02907C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TRUE 1
#define FALSE 0
// TODO: reference additional headers your program requires here

#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>

#define KILLMSGLEN 80
#define STRMAX 100
#define DATELEN 11

void writeerror(char* );
char* getdatetime (char* );

#if UNIX
#include <unistd.h>

#define SLEEP_MULTIPLIER 1
#define SLEEP sleep

#define stricmp(a,b) strcasecmp(a,b)
#define strnicmp(a,b,c) strncasecmp(a,b,c)
#else
#define SLEEP_MULTIPLIER 1000
#define SLEEP _sleep
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2DDAD7E2_6051_11D2_B760_00C0F02907C3__INCLUDED_)
