// configFile.h: interface for the configFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGFILE_H__2DDAD7E6_6051_11D2_B760_00C0F02907C3__INCLUDED_)
#define AFX_CONFIGFILE_H__2DDAD7E6_6051_11D2_B760_00C0F02907C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class configFile  
{
public:
		configFile();
	virtual ~configFile();
	int	openConfig(char *filename);
	void	closeConfig();
	int	readConfig();
	void	deleteConfig();
	void	dump();
	char	*getList() {return _list;}
	char	*getDaily() {return _daily;}
//	char	*getMaps() {return _htmMaps; }
	char	*getMsg() {return _msg;}
	char	*getLog() {return _log;}
	char	*getMTmpl() {return _mTmpl;}
	char	*getDTmpl() {return _dTmpl;}
//	char	*getMapTmpl() {return _mapTmpl;}
	char	*getHtmList() {return _htmList; }
	char	*getHtmDaily() {return _htmDaily; }
	char	*getStatusFile() {return _statusFile; }
	char	*getModule() {return _module; }
//	int	getMapstats() {return _mapstats; }
	int	getExpired() {return _expired; }
	int getMode() {return _mode; }

private:

//	int   _mapstats;			int _mapstatsWasSpecified;
	char _list[STRMAX+1];		int _listWasSpecified;
	char _daily[STRMAX+1];		int _dailyWasSpecified;
	char _log[STRMAX+1];		int _logWasSpecified;
	char _msg[STRMAX+1];		int _msgWasSpecified;
	char _mTmpl[STRMAX+1];		int _mTmplWasSpecified;
	char _dTmpl[STRMAX+1];		int _dTmplWasSpecified;
//	char _mapTmpl[STRMAX+1];		int _mapTmplWasSpecified;
	char _htmList[STRMAX+1];	int _htmListWasSpecified;
	char _htmDaily[STRMAX+1];	int _htmDailyWasSpecified;
//	char _htmMaps[STRMAX+1];	int _htmMapsWasSpecified;
	char _statusFile[STRMAX+1];	int _statusFileWasSpecified;
	char _module[STRMAX+1];		int _moduleWasSpecified;
	int   _expired;			int _expiredWasSpecified;
	int   _mode;			int _modeWasSpecified;

	FILE *_configFilePtr;
	int notDone;
	int line;
};

#endif // !defined(AFX_CONFIGFILE_H__2DDAD7E6_6051_11D2_B760_00C0F02907C3__INCLUDED_)
