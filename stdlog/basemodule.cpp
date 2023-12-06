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

// Win32 Header Files
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/***************************************************
TLS = Thread Local Storage

Thread Local Storage is used in Win32 DLLs to store
locally allocated memory in individual threads.

Note: I am not sure if this is necessary, because I
believe its only purpose is for multithreaded
programs, but it is probably good practice
nonetheless.
***************************************************/

// TLS Index
DWORD dwTLSIndex;

// DllMain - Standard Entry Point for DLLs
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	BOOL fIgnore = FALSE;
	killMsg *pKillMsg;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		// Attaching to process -> Get TLS Index
		// Retrieve the process's TLS index
		if ( (dwTLSIndex = TlsAlloc()) == 0xFFFFFFFF)
		{
			return FALSE;
		}

		break;

	case DLL_THREAD_ATTACH:			// Attaching to thread -> Allocate Data
		pKillMsg = (killMsg *) LocalAlloc(LPTR, sizeof(pKillMsg));
		if (pKillMsg != NULL)
			fIgnore = TlsSetValue(dwTLSIndex, pKillMsg);

		break;

	case DLL_THREAD_DETACH:			// Detaching thread -> Free Data
		pKillMsg = (killMsg *) TlsGetValue(dwTLSIndex);
		if (pKillMsg != NULL)
			LocalFree((HLOCAL)pKillMsg);
		break;

	case DLL_PROCESS_DETACH:		// Detaching process -> Free TLS Index
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

// The following two functions are global functions used by killMsg
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

/*
****************************************************************************************
Exported function:
	int loadKillMsgs(char* cfgFile)

Function:
	Loads the kill msg data from a file and fills the allocated killMsg structure.

Parameters:
	cfgFile - string (char*)
		Full path and filename of the killMsg config file

Return Value:
	TRUE - Success
	FALSE - Failure
****************************************************************************************
*/
int loadKillMsgs(char* cfgFile)
{
	return TRUE;
}

/*
****************************************************************************************
Exported function:
	int parseLine(char *pszLine, returnMsg *pReturn)

Function:
	Parses a single line of data and returns the data in the returnMsg structure

Parameters:
	pszLine - string (char*)
		Line from a log file. This line will be compared against the elements of the
		killMsg structure.
	pReturn - pointer to returnMsg structure
		After the given line is parsed, the information will be returned in 

Return Value:
	TRUE - Success
	FALSE - Failure
****************************************************************************************
*/
int parseLine(char *pszLine, returnMsg *pReturn)
{
	int match = FALSE; 
	killmsg *kmsg;
	killMsg *km;
	char *str1, *str2, delim1, delim2;
	int winner;

#ifdef WIN32
	// Retrieve the stored killMsg structure
	km = (killMsg *) TlsGetValue(dwTLSIndex);

	// If it is empty, nothing can be done
	if (km == NULL)
		return FALSE;
#endif

	// Set the internal linked list pointer to first element in list
	km->setToTopOfList();
	
	while ( (kmsg = km->getNextKillMsg()) != (killmsg *)NULL )
	{
		// Retrieve the data for each killMsg entry
		str1 = km->getString1(kmsg);
		str2 = km->getString2(kmsg);
		winner = km->getWinner(kmsg);
		delim1 = km->getDelim1(kmsg);
		delim2 = km->getDelim2(kmsg);
        
		// Attempt to match current line with current killMsg
		if (findkillMessage(str1, str2, winner, delim1, delim2, pszLine, pReturn))
		{
			// The correct killMsg type was found, so stop hunting.
			match = TRUE;
			break;
		}
	}

	return match;
}

/*
****************************************************************************************
Internal function:
	int findkillMessage(char *pattern1, char *pattern2, int winner, char delim1,
		char delim2, char* pszLine, returnMsg *pReturn)

Function:
	Compare the server log line against the current kill message template

Parameters:
	pattern1 - string (char*)
		The text after the first variable reference
	pattern2 - string (char*)
		The text after the second variable reference
	winner - integer (int)
		The type of message
	delim1 - character (char)
		The first variable in the string
	delim2 - character (char)
		The second variable in the string
	pszLine - string (char*)
		Line from a log file. This is the line that will be compared against the current
		kill message data
	pReturn - pointer to returnMsg structure
		After the given line is parsed, the information will be returned in 

Return Value:
	TRUE - Success
	FALSE - Failure
****************************************************************************************
*/

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

	// We have the proper matches, so let's continue to actually handle the data
	if (goAhead == TRUE)
	{
		// Set the names to nothing by default
		pReturn->winname[0] = '\0';
		pReturn->losename[0] = '\0';

		// Kill message: Killer's name is first
		if (winner == 1) {
			pReturn->winner = 1;
			if (name1pos > 0) {
				strncpy(pReturn->winname,pszLine+(name1pos-1),name1len);
				pReturn->winname[name1len] = '\0';
			}
			if (name2pos > 0) {
				strncpy(pReturn->losename,pszLine+(name2pos),name2len);
				pReturn->losename[name2len] = '\0';
			}
		}

		// Kill message: Killer's name is second
		if (winner == 2) {
			pReturn->winner = 2;
			if (name2pos > 1) {
				strncpy(pReturn->winname,pszLine+(name2pos),name2len);
				pReturn->winname[name2len] = '\0';
			}
			if (name1pos > 0) {
				strncpy(pReturn->losename,pszLine+(name1pos-1),name1len);
				pReturn->losename[name1len] = '\0';
			}
		}

		// Suicide message
		if (winner == 3) {
			pReturn->winner = 3;
			if (name1pos > 0) {
				strncpy(pReturn->winname,pszLine+(name1pos-1),name1len);
				pReturn->winname[name1len] = '\0';
			}
		}

		// Flag capture message
		if (winner == 4) {
			pReturn->winner = 4;
			if (name1pos > 0) {
				strncpy(pReturn->winname,pszLine+(name1pos-1),name1len);
				pReturn->winname[name1len] = '\0';
			}
		}
	}

	return goAhead;
}

