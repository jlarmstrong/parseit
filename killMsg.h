// killMsg.h: interface for the killMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KILLMSG_H__25BB8521_612B_11D2_B760_00C0F02907C3__INCLUDED_)
#define AFX_KILLMSG_H__25BB8521_612B_11D2_B760_00C0F02907C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "configFile.h"

#define NAME1 "[name1]"
#define NAME2 "[name2]"
#define MAPNAME "[map]"
#define CAPF "[cap]"
#define NAME3 "[name3]"

typedef struct killmsg {
	int winner;			// length 4  (starts on word boundary)
	char string1[KILLMSGLEN+1];	// length 81 (starts on word boundary)
	char delim1;			// length 1
	char pad1, pad2;		// length 2
	char string2[KILLMSGLEN+1];	// length 81 (starts on word boundary)
	char delim2;			// length 1
	char pad3, pad4;		// length 2
	killmsg *next;			// length 4  (starts on word boundary)
} killmsg;

class killMsg  
{
public:
			killMsg(char *cfgfile);
	virtual ~killMsg();
	killmsg *getKillList() {return _killMsgList;}
	void	dump();
	killmsg *getNextKillMsg();
	int		getWinner(killmsg *km) {return km->winner;}
	char	*getString1(killmsg *km) {return km->string1;}
	char	getDelim1(killmsg *km) {return km->delim1;}
	char	*getString2(killmsg *km) {return km->string2;}
	char	getDelim2(killmsg *km) {return km->delim2;}
	void	setToTopOfList() {_currKillMsg = _killMsgList;}

private:
	killmsg *_killMsgList;
	killmsg *_currKillMsg;
			killMsg();
    int		readKillFile(char *cfgFile);
	int		parseMsg(char *temp, int *winner, char *delim1, char *delim2, 
					 char *str1, char *str2);
	int		insertInList(int winner, char *str1, char delim1, char *str2, char delim2);
		
};

#endif // !defined(AFX_KILLMSG_H__25BB8521_612B_11D2_B760_00C0F02907C3__INCLUDED_)
