// myLog.h: interface for the myLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLOG_H__5B6B6261_70C8_11D2_B762_00C0F02907C3__INCLUDED_)
#define AFX_MYLOG_H__5B6B6261_70C8_11D2_B762_00C0F02907C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class myLog  
{
public:
	myLog();
	virtual ~myLog();
	int openMyLog( char *logname, char *statusname, long *numLastRead );
	int logDateIsNewer();
	void writeMyLog(int num, char *lastLine);
	int isThisTheOne(char *line);

private:
	FILE *_logOutPtr;
	char _sLogName[STRMAX+1];
	char _lastLine[STRMAX+1];
	time_t getFileTime();
	long _lastFileTime;

};

#endif // !defined(AFX_MYLOG_H__5B6B6261_70C8_11D2_B762_00C0F02907C3__INCLUDED_)
