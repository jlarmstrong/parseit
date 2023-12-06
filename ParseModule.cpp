#include "StdAfx.h"

#ifdef _WIN32
#define NOGDI
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "ParseModule.h"

#define PARSEMODULE_LOADKILL	0
#define PARSEMODULE_PARSELINE	1
#define PARSEMODULE_FREEMSGS	2

int ParseModule::loadKillMsgs(char *cfgfile)
{
	int (*f)(char*) = reinterpret_cast<int(*)(char*)>( GetProcAddr("loadKillMsgs__FPc"));

	if (f)
		return f(cfgfile);

	return 0;
}

int ParseModule::parseLine(char *pszLine, returnMsg *pReturn)
{
	int (*f)(char*,returnMsg*) = reinterpret_cast<int(*)(char*,returnMsg*)>( GetProcAddrCached("parseLine__FPcP10_returnMsg", PARSEMODULE_PARSELINE));

	if (f)
		return f(pszLine, pReturn);

	return 0;
}

void ParseModule::freeMsgs()
{
	DLPROC f = GetProcAddr("freeMsgs");

	f();
}
