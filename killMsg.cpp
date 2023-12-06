// killMsg.cpp: implementation of the killMsg class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StdAfx.h"
#include "killMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

killMsg::killMsg()
{

}

/* *********************************************** */
killMsg::killMsg(char *cfgfile)
{
	_killMsgList = 0;
	readKillFile(cfgfile);
}

/* *********************************************** */
killMsg::~killMsg()
{
struct killmsg *next1 = _killMsgList, *theOne = _killMsgList;

   /* free up memory that was allocated for the list of kill messages */
   while (theOne != 0) 
      {
      next1 = theOne->next;
      delete theOne;
      theOne = next1;
      }

   _killMsgList = 0;
   _currKillMsg = 0;

}

/* *********************************************** */
int killMsg::readKillFile(char  *cfgfile)
{
	FILE *fptr;
	int winner;
	char temp[STRMAX+1], delim1, delim2, str1[KILLMSGLEN+1], str2[KILLMSGLEN+1];
	char *newLine;

	if ((fptr = fopen(cfgfile,"r")) == NULL)
	{
	   /// this is an error 
	    strcpy(temp,"Cannot open ");
	    strcat(temp,cfgfile);
	    writeerror(temp);
	    //cout<<temp<<endl;
	    exit(1);
	}

	while ((fgets(temp, STRMAX, fptr)) != 0)
	{
       // get rid of the '\n'
		newLine = strchr(temp,'\n');
		if (newLine)
			*newLine = '\0';

		if (!parseMsg(temp, &winner, &delim1, &delim2, str1, str2))
            continue;

		if (!insertInList(winner, str1, delim1, str2, delim2))
			return FALSE;
	}

	if (fptr)
	   fclose(fptr);

	return TRUE;
}

/* *********************************************** */
int killMsg::insertInList(int winner, char *str1, char delim1, char *str2, char delim2)
{
	struct killmsg *iterator = 0, *prev = 0;
   
	// find the end of the list ... this is where we will insert the new one 
	for (iterator = _killMsgList; iterator > 0; prev = iterator,iterator = iterator->next)
      ;

   /* allocate a new struct and added it to the linked list */
   //struct killmsg *add = new struct killmsg;
   killmsg *add = new killmsg;

   if (add == NULL)
	   return FALSE;

   add->winner = winner;
   strcpy(add->string1, str1);
   add->delim1 = delim1;
   strcpy(add->string2, str2);
   add->delim2 = delim2;
   add->next = 0;

   if (prev != 0)
      prev->next = add;
   else                       // special case of nothing in the list
      _killMsgList = add;


	return TRUE;
}

/* *********************************************** */
int killMsg::parseMsg(char *temp, int *winner, char *delim1, char *delim2, 
			 char *str1, char *str2)

{
	char rbracket='[',
		 lbracket=']';
	char *beg, *tok;
	int len;

	// initialize all of them first
	*winner = 0;
	*delim1 = '\0';
	*delim2 = '\0';
	*str1 = '\0';
	*str2 = '\0';

	// for now [name2] is always first and is always the loser
	// *winner = 2;
        if ( strnicmp(NAME1, temp, strlen(NAME1)) == 0)
	    *winner = 1;
        if ( strnicmp(NAME2, temp, strlen(NAME2)) == 0)
        *winner = 2;
        if ( strnicmp(CAPF, temp, strlen(CAPF)) == 0)
		*winner = 3;
        if ( strnicmp(NAME3, temp, strlen(NAME3)) == 0)
        *winner = 4;


	beg = temp;
	// find the ] at the end of [name2]
    tok = strchr(beg, lbracket);
	// increment to the next char ... this is the first delimiter
    beg = tok+1;
    *delim1 = *beg;

    // increment to the next char
	beg++;
	//now find the [ in [name1]
    tok = strchr(beg, rbracket);
    // if null there is no [name1] so get it all
	if (tok == NULL) {
		strcpy(str1,beg);
		return TRUE;
	}

    // pull off from 1 past the first delimiter to 1 char before [name1]
	len = tok - beg - 1;
	strncpy(str1,beg,len);
	str1[len] = '\0';

	// find the ] in [name1]
    tok = strchr(beg, lbracket);
	// at the end of the kill message
	if (*(tok+1) == '\0' || strlen(tok) == 0)
		return TRUE;
	// increment to the next char ... this is the 2nd delimiter
    beg = tok+1;
	*delim2 = *beg;
    beg++;
	// this is the last part of the kill message
	strcpy(str2,beg);

	return TRUE;
}

/* *********************************************** */
void killMsg::dump()
{
	struct killmsg *it = 0;
             
	for (it = _killMsgList; it > 0; it = it->next)
	{
		cout<<"w="<<it->winner<<"#s1="<<it->string1<<"#d1="<<it->delim1<<
			"#s2="<<it->string2<<"#d2="<<it->delim2<<endl;
	}
}

/* *********************************************** */
killmsg *killMsg::getNextKillMsg()
{
	killmsg *returnOne;

	// if we are at the first entry in the list
	if (_currKillMsg == _killMsgList) {
		returnOne = _killMsgList;
	}
	else 
		returnOne = _currKillMsg;

	if (_currKillMsg != NULL)
	   _currKillMsg = _currKillMsg->next;

	return returnOne;


}
