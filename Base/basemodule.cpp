// basemodule.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "killMsg.h"
#include "dlldata.h"

// Exported:
int loadKillMsgs(char* cfgFile);
int parseLine(char *pszLine, returnMsg *pReturn);

// Internal:
void writeerror(char* err);
char* getdatetime (char* t_d);
int findkillMessage(char *pattern1, char *pattern2, int winner, char delim1, char delim2, char* pszLine, returnMsg *pReturn);

/* --- Win32 DLL code --- */
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

DWORD dwTLSIndex;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	BOOL fIgnore = FALSE;
	killMsg *pKillMsg;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		// Attaching to process -> Initalize
		// Retrieve the process's TLS index
		if ( (dwTLSIndex = TlsAlloc()) == 0xFFFFFFFF)
		{
			return FALSE;
		}

		break;

	case DLL_THREAD_ATTACH:
		pKillMsg = (killMsg *) LocalAlloc(LPTR, sizeof(pKillMsg));
		if (pKillMsg != NULL)
			fIgnore = TlsSetValue(dwTLSIndex, pKillMsg);

		break;

	case DLL_THREAD_DETACH:
		pKillMsg = (killMsg *) TlsGetValue(dwTLSIndex);
		if (pKillMsg != NULL)
			LocalFree((HLOCAL)pKillMsg);
		break;

	case DLL_PROCESS_DETACH:
		// Free any allocated memory
		pKillMsg = (killMsg *) TlsGetValue(dwTLSIndex);
		if (pKillMsg != NULL)
			LocalFree((HLOCAL)pKillMsg);

		// Free the retrieved TLS index
		TlsFree(dwTLSIndex);
		break;
	}

    return TRUE;
}
#endif
/* --- End Win32 DLL code --- */

/* ********* Write to an errlog log ************************************** */
void writeerror(char* err)
{ 
    char date[STRMAX+1];
    FILE *fptr;

    fptr = fopen("ERROR.txt","a");
    if (fptr) {
		getdatetime (date);
        fprintf(fptr, "[%s] %s\n", date, err);
        fprintf(stderr, "[%s] %s\n", date, err);
        fclose(fptr);
    } 
}

/* **************** Return date as MMM DD YYYY *************************** */
char* getdatetime (char* t_d)
{
 
   //struct timeb timebuffer;
   time_t timebuffer;
   char *timeline;

   //ftime( &timebuffer );
   //timeline = ctime( & ( timebuffer.time ) );
   timebuffer = time(0);
   timeline = ctime ( &timebuffer );

   strncpy(t_d, &timeline[4],7);
   t_d[7] = '\0';
   strncat(t_d, &timeline[20],4);
   return t_d;
}

int loadKillMsgs(char* cfgFile)
{
	killMsg *pKillMsg;

#ifdef WIN32
	// Set our TLS value to the killMsg pointer
 	pKillMsg = (killMsg*) TlsGetValue(dwTLSIndex);

	if (pKillMsg == NULL) {
		pKillMsg = (killMsg *) LocalAlloc(LPTR, sizeof(killMsg));
		if (pKillMsg != NULL)
			TlsSetValue(dwTLSIndex, pKillMsg);
	}
#endif

	// Load the data from the kill message file
	if (!pKillMsg->readKillFile(cfgFile))
		return FALSE;

	return TRUE;
}

int parseLine(char *pszLine, returnMsg *pReturn)
{
	int match = FALSE; 
	killmsg *kmsg;
	killMsg *km;
	char *str1, *str2, delim1, delim2;
	int winner;

#ifdef WIN32
	km = (killMsg *) TlsGetValue(dwTLSIndex);
#endif

	km->setToTopOfList();
	
	while ( (kmsg = km->getNextKillMsg()) != (killmsg *)NULL ) {
		str1 = km->getString1(kmsg);
		str2 = km->getString2(kmsg);
		winner = km->getWinner(kmsg);
		delim1 = km->getDelim1(kmsg);
		delim2 = km->getDelim2(kmsg);
        
		if (findkillMessage(str1, str2, winner, delim1, delim2, pszLine, pReturn)) {
			match = TRUE;
			break;
		}
	}

	return match;
}

/* ********************* find a kill message ************************* */
/* compare the server log entry against the kill message templates *** */
int findkillMessage(char *pattern1, char *pattern2, int winner, char delim1, char delim2, char* pszLine, returnMsg *pReturn)
{
	unsigned int lenE = strlen(pszLine), 
		len1 = strlen(pattern1),
		len2 = strlen(pattern2);
	char *cur = pszLine, *beg = pszLine, *tok;
	int match1 = FALSE, match2 = FALSE;
	int goAhead = FALSE;
	//int endof1st = 0;

	unsigned int name1pos = 0,
		name2pos = 0,
		name1len = 0,
		name2len = 0;

	if (lenE < len1 || lenE < len2) 
		return FALSE;

	// try to find the 1st string
	while (strlen(cur) > 0) {
	   tok = strchr(cur,delim1);
	   if (tok == (char *)NULL)
		   break;
	   if (strlen(cur) < len1)
		   break;
	   if ( strncmp(cur,pattern1,len1) == 0) {
		   name1pos = 1;           // for now not always 1st in the log
		   match1 = TRUE;
		   break;
	   }
	   else {
		   name1len = tok - beg;
		   cur += tok - cur +1;
		   continue;
	   }

	}

	if (match1) {
	   name2pos = len1 + name1len + 2;
	   beg = pszLine + name2pos;
	   if ((unsigned) name2pos > strlen(pszLine)) {
		   cur = (char *)NULL;
		   name2pos = 0;
	   }
	   else
	      cur = pszLine + name2pos;

   	   while (cur != (char *)NULL && strlen(cur) > 0) {
		  // there is no second pattern so length of the name is the rest of the string
		  if ( delim2 == '\0' && (pattern2 == (char *)NULL || strcmp(pattern2,"") == 0)) {
			   name2len = strlen(cur);
			   if (strlen(cur) > 0)
				   match2 = TRUE;
			   break;
		  }
	      tok = strchr(cur,delim2);
		  if (tok == (char *)NULL)
			  break;
		  if (strlen(cur) < len2)
			  break;
	      if ( strncmp(tok+1,pattern2,len2) == 0) {
			  name2len = tok - beg;
		      match2 = TRUE;
		      break;
		  }
	      else {
		     cur += tok-cur +1;
		     continue;
		  }

		}
	}

	if (!match1) {
		name1pos = 0;
		name1len = 0;
	}

	if (!match2) {
		name2pos = 0;
		name2len = 0;
	}

   // there is no second pattern john
    if ( delim2 == '\0' && (pattern2 == (char *)NULL || strcmp(pattern2,"") == 0)) {
		goAhead = match1;
	}
	else {
		goAhead = (match1 & match2);
	}

	if (goAhead == TRUE)
	{
		pReturn->winname[0] = '\0';
		pReturn->losename[0] = '\0';

		if (winner == 1) {
			pReturn->winner = 1;
			if (name1pos > 0) {
				strncpy(pReturn->winname,pszLine+(name1pos-1),name1len);
				pReturn->winname[name1len] = '\0';
				//pl->updateList(winname, 0, 1, 0, 0);
			}
			if (name2pos > 0) {
				strncpy(pReturn->losename,pszLine+(name2pos),name2len);
				pReturn->losename[name2len] = '\0';
				//pl->updateList(losename, 1, 0, 0, 0);
			}
		}

		if (winner == 2) {
			pReturn->winner = 2;
			if (name2pos > 1) {
				strncpy(pReturn->winname,pszLine+(name2pos),name2len);
				pReturn->winname[name2len] = '\0';
				//pl->updateList(winname, 0, 1, 0, 0);
			}
			if (name1pos > 0) {
				strncpy(pReturn->losename,pszLine+(name1pos-1),name1len);
				pReturn->losename[name1len] = '\0';
				//pl->updateList(losename, 1, 0, 0, 0);
			}
		}

		if (winner == 3) {
			pReturn->winner = 3;
			if (name1pos > 0) {
				strncpy(pReturn->winname,pszLine+(name1pos-1),name1len);
				pReturn->winname[name1len] = '\0';
				//pl->updateList(winname, 0, 0, 1, 0);
			}
		}

		if (winner == 4) {
			pReturn->winner = 4;
			if (name1pos > 0) {
				strncpy(pReturn->winname,pszLine+(name1pos-1),name1len);
				pReturn->winname[name1len] = '\0';
				//pl->updateList(winname, 0, 0, 0, 1);
			}
		}
	}

	return goAhead;
}

