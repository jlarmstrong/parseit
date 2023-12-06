#ifndef PARSEMODULE_H_INCLUDED
#define PARSEMODULE_H_INCLUDED

#include "DynamicLibrary.h"

#ifdef _WIN32
#include "base\dlldata.h"
#else
#include "base/dlldata.h"
#endif

class ParseModule : public DynamicLibrary
{
public:

	ParseModule(void) : DynamicLibrary() {};
	ParseModule(const char *libName) : DynamicLibrary(libName) {};

	~ParseModule(void){};

	int loadKillMsgs(char *cfgfile);
	int parseLine(char *pszLine, returnMsg *pReturn);

	void freeMsgs(void);
};

#endif
