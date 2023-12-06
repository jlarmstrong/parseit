// myLog.cpp: implementation of the myLog class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
//#include <io.h>
#include <time.h>
#include <sys/stat.h>

#include "StdAfx.h"
#include "myLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

myLog::myLog()
{
   _lastFileTime = 0L;
}

myLog::~myLog()
{
   if (_logOutPtr)
	   fclose(_logOutPtr);
}

int myLog::logDateIsNewer()
{

	if ( getFileTime() > _lastFileTime )
	   return TRUE;
	else
	   return FALSE;
}

void myLog::writeMyLog(int num, char *lastLine)
{
	//char temp[STRMAX+1];
	time_t curTime = getFileTime();

	if (_logOutPtr) {
	   fprintf(_logOutPtr, "%lu", (long)num);
	   fputs("\n",_logOutPtr);
	   fprintf(_logOutPtr, "%lu", curTime);
	   fputs("\n",_logOutPtr);
	   fputs(lastLine,_logOutPtr);
	
	   fclose(_logOutPtr);
	   _logOutPtr = NULL;
	}


}
int myLog::openMyLog( char *logname, char *statusname, long *numLastRead )
{
   char temp[STRMAX+1];

   strcpy(_sLogName,logname);

   *numLastRead = 0;
   if ( (_logOutPtr = fopen(statusname,"r+")) != NULL ) {
	   fgets(temp,STRMAX, _logOutPtr);
	   *numLastRead = atol(temp);

       fgets(temp,STRMAX, _logOutPtr);
	   _lastFileTime = atol(temp);
	   fgets(_lastLine,STRMAX,_logOutPtr);
	   fseek(_logOutPtr,0L,SEEK_SET);
   }
   else { //  didn't exist so open it for writing  
	   if ( (_logOutPtr = fopen(statusname,"w")) == NULL ) {
		  // error 
	   }
   }
	return TRUE;
}

time_t myLog::getFileTime()
{
  struct stat file_info;
  time_t retval;
  FILE *fptr;

  if( (fptr = fopen( _sLogName, "r" )) == (FILE *)NULL) {
      writeerror( "No server log file in current directory!" );
          retval =  (time_t) 0;
      exit(1);
  }
  else {
      if( fstat( fileno(fptr), &file_info ) == -1L ) {
	  writeerror( "No server log file in current directory!" );
	  retval =  (time_t) 0;
	  exit(1);
      }
      else {
	  retval = file_info.st_mtime;
	  fclose( fptr );
      }
  }

  return retval;
}

int myLog::isThisTheOne(char *line)
{
	if (strncmp(line, _lastLine,strlen(_lastLine)) == 0)
		return TRUE;
	else
		return FALSE;
}
