// configFile.cpp: implementation of the configFile class.
//
//////////////////////////////////////////////////////////////////////

// #include <unistd.h>
#include "StdAfx.h"
#include "configFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* *********************************************** */
configFile::configFile()
{
    _configFilePtr = (FILE *)NULL;

//    _mapstats     = '\0';	_mapstatsWasSpecified = 0;
    _list[0]     = '\0';	_listWasSpecified = 0;
    _daily[0]    = '\0';	_dailyWasSpecified = 0;
    _msg[0]      = '\0';	_msgWasSpecified = 0;
    _mTmpl[0]    = '\0';	_mTmplWasSpecified = 0;
    _dTmpl[0]    = '\0';	_dTmplWasSpecified = 0;
//    _mapTmpl[0]    = '\0';	_mapTmplWasSpecified = 0;
    _htmList[0]  = '\0';	_htmListWasSpecified = 0;
    _htmDaily[0] = '\0';	_htmDailyWasSpecified = 0;
//    _htmMaps[0] = '\0';	_htmMapsWasSpecified = 0;
    _statusFile[0]  = '\0';	_statusFileWasSpecified = 0;
    _expired     = 0;		_expiredWasSpecified = 0;
    _mode     = 0;		_modeWasSpecified = 0;
	_module[0] = 0;		_moduleWasSpecified = 0;

    notDone	 = FALSE;
    line	 = 0;
}

/* *********************************************** */
configFile::~configFile()
{
    if (_configFilePtr) {
	fclose(_configFilePtr);
	_configFilePtr = (FILE *)NULL;
	notDone = FALSE;
	line = 0;
    }
}

/* *********************************************** */
int configFile::openConfig(char *filename)
{
    char errorLine[STRMAX*2];

    if ((_configFilePtr = fopen(filename,"r")) == NULL) {
    	// this is an error handle later
	sprintf(errorLine, "%s ==> Cannot open config file!", filename);
	writeerror(errorLine);
    	exit(1);
    }

    notDone = TRUE;
    line = 0;

    return (1);
}

/* *********************************************** */
void configFile::closeConfig()
{
    if (_configFilePtr) {
	fclose(_configFilePtr);
	_configFilePtr = (FILE *)NULL;
	notDone = FALSE;
	line = 0;
    }
}

/* *********************************************** */
int configFile::readConfig()
{
    char inputStr[STRMAX+1], variable[STRMAX+1];
    char errorLine[STRMAX*2], *value, *str, *p;
    int insideList = 0;
    long fpos;
    
    // Each line in the file must be in one of the following formats:
    //
    //   [WP] "[" [IGNORED]
    //   [WP] variable [WP] [:=] [WP] value [WP] [IGNORED]
    //   [WP] "#" [IGNORED]
    //   [WP] ";" [IGNORED]
    //   [WP]
    //   "[end]"
    //
    // [WP] are optional whitespaces.  [IGNORED] means everything to end of
    // line is ignored.  This means that when an '[' is encountered, if that
    // line isn't an '[END]', it will assume a new list is ready to start.
    // the '[END]' is ignored if encountered and is only provided to keep 
    // config files from previous versions of ParseIt from breaking.  [:=]
    // means that either a colon or an equal sign (not both) must be there.
    // everything after the value is ignored and the value itself must not
    // have any whitespace.  The variable must be one ParseIt understands.
    //
    // Now, for some general rules that need to be spelled out...
    //
    //   1) comments are defined as anything after '#' or ';'.
    //   2) blank lines and anything after comments are ignored.
    //   3) a list begins with a line [xxxx].  Anything can go in the [].
    //   4) a list ends with the next [xxxx] or at EOF.
    //   5) if an [end] is encountered, it is ignored.
    //   6) notDone is FALSE only when an EOF condition occurs.
    //   7) every variable is currently required to exist.
    //   8) we must protect against empty config files and short lists.
    //   9) let's not be case sensitive!
    //
    // If we can do all this, we will be in good shape...

    if (notDone == FALSE) return notDone;

    // Let's assume we will hit an EOF condition before returning...
    // We can always set it back to TRUE for other reasons...
    notDone = FALSE;

    while (fpos=ftell(_configFilePtr), fgets(inputStr,STRMAX,_configFilePtr)) {
	line++;

	// First, lets skip over any leading white space on the line.
	for (str = inputStr; *str && isspace(*str); str++);

	// Next, end the line at the first newline or comment character.
	for (p = str; *p && *p != ';' && *p != '#'; p++);
	*p = '\0';

	// Is it a blank line?  Note that if the line was blank or only 
	// contained a comment on it, the above lines turned it *into* a
	// blank line... this means the following line catches everything!
	if (*str == '\0') continue;

	// Okay... we are either at a [xxxx] line or a var=value line.
	// Var=value lines are not allowed if we haven't seen a [xxxx] first.

	if (*str == '[') {
	    if (!strnicmp(str, "[end]", 5)) continue;

	    // If we haven't started a list yet, then let's start one!
	    if (!insideList) {
	        insideList = TRUE;
		continue;
	    }

	    // Otherwise, we must have found the start of another list.
	    // But since we must be able to re-read this line the next
	    // time we call this function, we have to put it back!
	    fseek(_configFilePtr, fpos, SEEK_SET);
	    notDone = TRUE;
	    break;
	}

	// If we are here, that means we have a valid line to parse and
	// we have already started a new list.  Things get a little hairy
	// here, but in order to support a fairly loose style in the file,
	// we need to do some leg work... but trust me, it is worth it.

	// Copy our work so we can mangle the copy...
	strncpy(variable, str, STRMAX);

	// Find the end of the variable, marked by a :, = or a whitespace.
	for (p = variable; *p && !isspace(*p) && *p != ':' && *p != '='; p++);
	if (*p == '\0') {
	    // Whoops, only the variable was provided and no value!
	    sprintf(errorLine, "config.ini ==> Missing VALUE on line %d!", line);
	    writeerror(errorLine);
	    exit(1);
	}

	// If we are sitting at a space, then we must first null-terminate
	// the variable, and then skip all the white space leading up to
	// the '=' or ':' character.
	if (isspace(*p)) {
	    *p++ = '\0';
	    while (*p && isspace(*p)) p++;
	    if (*p == '\0') {
	        // Still no value following the variable!
		sprintf(errorLine, "config.ini ==> Missing VALUE on line %d!", line);
		writeerror(errorLine);
		exit(1);
	    }
	}

	// At this point, we should be sitting at an '=' or ':'.  Skip
	// past it, and then skip past any following white space.  Just in
	// case the '=' or ':' was attached to the end of the variable with
	// no intervening spaces, we will set the current location to '\0'
	// to null terminate the variable.
	*p++ = '\0';
	while (*p && isspace(*p)) p++;
	if (*p == '\0') {
	    // Oops, no value following the '=' or ':'
	    sprintf(errorLine, "config.ini ==> Missing VALUE on line %d!", line);
	    writeerror(errorLine);
	    exit(1);
	}

	// Alright, p should now be pointing to the start of the value.
	// Simply look for the first whitespace and terminiate the value.
	for (value = p; *p && !isspace(*p); p++);
	*p = '\0';

	// The parsing is done, now we interpret the variable and do something
	// with the value.  If we encounter an invalid variable, show an error
	// and keep going.  Also we need to keep track of the variables seen,
	// since most of them are required.

	if (!strnicmp(variable, "list", STRMAX)) {
	    strncpy(_list, value, STRMAX);
	    _listWasSpecified = 1;
	}
//	else if (!strnicmp(variable, "mapstats", STRMAX)) {
//	    _mapstats = atoi(value);
//	    _mapstatsWasSpecified = 1;
//	}
	else if (!strnicmp(variable, "daily", STRMAX)) {
	    strncpy(_daily, value, STRMAX);
	    _dailyWasSpecified = 1;
	}
	else if (!strnicmp(variable, "log", STRMAX)) {
	    strncpy(_log, value, STRMAX);
	    _logWasSpecified = 1;
	}
	else if (!strnicmp(variable, "msg", STRMAX)) {
	    strncpy(_msg, value, STRMAX);
	    _msgWasSpecified = 1;
	}
	else if (!strnicmp(variable, "listtmpl", STRMAX)) {
	    strncpy(_mTmpl, value, STRMAX);
	    _mTmplWasSpecified = 1;
	}
	else if (!strnicmp(variable, "dailytmpl", STRMAX)) {
	    strncpy(_dTmpl, value, STRMAX);
	    _dTmplWasSpecified = 1;
	}
//	else if (!strnicmp(variable, "mapstmpl", STRMAX)) {
//	    strncpy(_mTmpl, value, STRMAX);
//	    _mapTmplWasSpecified = 1;
//	}
	else if (!strnicmp(variable, "htmllist", STRMAX) ||
	         !strnicmp(variable, "htmlist",  STRMAX)) {
	    strncpy(_htmList, value, STRMAX);
	    _htmListWasSpecified = 1;
	}
	else if (!strnicmp(variable, "htmldaily", STRMAX) ||
	         !strnicmp(variable, "htmdaily",  STRMAX)) {
	    strncpy(_htmDaily, value, STRMAX);
	    _htmDailyWasSpecified = 1;
	}
//	else if (!strnicmp(variable, "htmlmaps", STRMAX) ||
//	         !strnicmp(variable, "htmmaps",  STRMAX)) {
//	    strncpy(_htmMaps, value, STRMAX);
//	    _htmMapsWasSpecified = 1;
//	}
	else if (!strnicmp(variable, "sleep", STRMAX)) {
	    // Sleep is not implemented.   Simply ignore it.
	}
	else if (!strnicmp(variable, "status", STRMAX)) {
	    strncpy(_statusFile, value, STRMAX);
	    _statusFileWasSpecified = 1;
	}
	else if (!strnicmp(variable, "expire", STRMAX)) {
	    _expired = atoi(value);
	    _expiredWasSpecified = 1;
	}
	else if (!strnicmp(variable, "mode", STRMAX)) {
	    _mode = atoi(value);
	    _modeWasSpecified = 1;
	}
	else if (!strnicmp(variable, "module", STRMAX)) {
		strncpy(_module, value, STRMAX);
		_moduleWasSpecified = 1;
	}
	else {
	    sprintf(errorLine, "config.ini ==> Unknown variable '%s' on line %d", variable, line);
	    writeerror(errorLine);
	    exit(1);
	}
    }

    if (!_listWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'List' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
    else if (!_dailyWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'Daily' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
    else if (!_logWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'Log' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    }
    else if (!_msgWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'Msg' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
    else if (!_mTmplWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'ListTmpl' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
    else if (!_dTmplWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'DailyTmpl' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
//    else if (!_mapTmplWasSpecified) {
//	sprintf(errorLine, "config.ini ==> 'MapsTmpl' required before line %d", line);
//	writeerror(errorLine);
//	exit(1);
//  } 
    else if (!_htmListWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'HtmList' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
    else if (!_htmDailyWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'HtmDaily' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
//    else if (!_htmMapsWasSpecified) {
//	sprintf(errorLine, "config.ini ==> 'HtmMaps' required before line %d", line);
//	writeerror(errorLine);
//	exit(1);
//   } 
    else if (!_statusFileWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'Status' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    } 
    else if (!_expiredWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'Expired' required before line %d", line);
	writeerror(errorLine);
	exit(1);
    }
	else if (!_moduleWasSpecified) {
	sprintf(errorLine, "config.ini ==> 'Module' required before line %d", line);
	writeerror(errorLine);
	exit(1);
	}

    return TRUE;
}

/* *********************************************** */
void configFile::deleteConfig()
{
//    _mapstats     = 0;		_mapstatsWasSpecified = 0;
    _list[0]     = '\0';	_listWasSpecified = 0;
    _daily[0]    = '\0';	_dailyWasSpecified = 0;
    _msg[0]      = '\0';	_msgWasSpecified = 0;
    _mTmpl[0]    = '\0';	_mTmplWasSpecified = 0;
    _dTmpl[0]    = '\0';	_dTmplWasSpecified = 0;
//    _mapTmpl[0]    = '\0';	_mapTmplWasSpecified = 0;
    _htmList[0]  = '\0';	_htmListWasSpecified = 0;
    _htmDaily[0] = '\0';	_htmDailyWasSpecified = 0;
//    _htmMaps[0] = '\0';	_htmMapsWasSpecified = 0;
    _statusFile[0]  = '\0';	_statusFileWasSpecified = 0;
    _expired     = 0;		_expiredWasSpecified = 0;
    _mode     = 0;		_modeWasSpecified = 0;
	_module[0] = 0;		_moduleWasSpecified = 0;

}

/* *********************************************** */
void configFile::dump()
{
//    if (_mapstats) cout<<"Mapstats: " <<_mapstats<<endl;
    if (_list) cout<<"List: " <<_list<<endl;
    if (_daily) cout<<"Daily: "<<_daily<<endl;
    if (_msg) cout<<"Msg: "<<_msg<<endl;
    if (_mTmpl) cout<<"mTmpl: "<<_mTmpl<<endl;
//    if (_mapTmpl) cout<<"mapTmpl: "<<_mapTmpl<<endl;
    if (_dTmpl) cout<<"dTmpl: "<<_dTmpl<<endl;
    if (_htmList) cout<<"htmList: "<<_htmList<<endl;
    if (_htmDaily) cout<<"htmDaily: "<<_htmDaily<<endl;
	if (_module) cout<<"module: "<<_module<<endl;
//    if (_htmMaps) cout<<"htmMaps: "<<_htmMaps<<endl;
    cout<<"expire: "<<_expired<<endl;
}
