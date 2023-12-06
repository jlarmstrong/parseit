#include <ctype.h>
#include <fstream.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "StdAfx.h"
#include "playerList.h"

#define DATELEN 11
#define NUMLEN 15
#define EXPIRE 30

/* **************** Return date number of seconds since 01/01/70 ********* */
long playerList::getDateInSec()
{
	time_t aTime;
	time(&aTime);
	return aTime;
}

/* ************* make sure name is good ********************************** */
int goodName(char *str)
{
	

  if (str == NULL || 
	  stricmp(str,"") == 0 ||
	  stricmp(str,"\t") == 0 ||
	  stricmp(str,"SAY:") == 0 ||
	  stricmp(str, "MSG:") == 0 )
	  return FALSE;

  int len = strlen(str);
  int pos = 0, i;

  for (i = 0; *str != NULL; ++i) {
	  if (*str++ == ' ')
		  ++pos;
  }
  if (len == pos)
	  return FALSE;

  return TRUE;
}

 /* **************** determine if we need to delete from list ************ */
int playerList::needToDelete(long begTime, long endTime, int retention)
{
  int retVal = FALSE;
//  struct _listStuff *iterator = 0;

  if (begTime == 0)
	  return retVal;



  double tdiff = difftime(endTime, begTime);
  int numDays = (int) tdiff/60/60/24;
  if (numDays > retention)
     retVal = TRUE;

  return retVal;
}		

/* **************** PlayerList constructor ****************************** */
playerList::playerList()
{
   _curNumNames = 0;
   _curListPos = 0;
   pList = 0;
   _listDate[0] = '\0';
}
/* *********************************************************************** */
void playerList::operator=(playerList *lptr)
{
	_curNumNames = lptr->_curNumNames;
	pList = lptr->pList;
	_curListPos = lptr->_curListPos;

}

/* **************** playerList destructor ******************************** */
playerList::~playerList()
{

   struct _listStuff *next1 = pList, *theOne = pList;

   /* free up memory that was allocated for the list of players */
   while (theOne != 0) {
      next1 = theOne->next;
      delete theOne;
      theOne = next1;
   }

   pList = 0;
   _curNumNames = 0;
   _curListPos = 0;
}

/* ************ Find a name in the list and return a pointer ************ */
_listStuff *playerList::findName(const char *name)
{
   struct _listStuff *iterator = 0;
   int havematch = 0;
   char lname[NAMELEN+1], llname[NAMELEN+1];
   unsigned int i;

   /* convert name to lower case for comparison */
  for (i = 0; i < strlen(name); i++)
	  lname[i] = tolower(name[i]);

  lname[i] = '\0';

   for (iterator = pList; iterator > 0; iterator = iterator->next) {
      /* convert name to lower case for comparison */
	  for (i = 0; i < strlen(iterator->name); i++)
		 llname[i] = tolower(iterator->name[i]);
	  llname[i] = '\0';

      if (strcmp(lname,llname) == 0 )  {
		 havematch = 1;
         break;
	  }
   }

   /* return a pointer to the position in the list where the name was found */
   if (havematch)
      _curListPos = iterator;
   else
      _curListPos = 0;

   return iterator;
}

/* ************ Insert a name in the linked list ********************** */
void playerList::insertName(const char *name, int win, int loss, int capture, long sec, int suicide)
{
   struct _listStuff *iterator = 0, *prev = 0;
   //long haveMatch = 0;

   /* find last (prev) node - we need this so we can update the pointer to 
      the "next" in the list when the new one is added */
   for (iterator = pList; iterator > 0; prev = iterator,iterator = iterator->next)
      ;

   /* allocate a new struct and added it to the linked list */
   //struct _listStuff *add = new struct _listStuff;
   _listStuff *add = new _listStuff;

   if (add == (_listStuff *)NULL)
   {
	   cout<<"!!! OUT OF MEMORY !!!"<<endl;
	   exit(0);
   }

   strcpy(add->name,name);
   add->kills = win;
   add->deaths = loss;
   add->caps = capture;
   add->rank = 0;
   add->percent = 0;
   add->points = 0;
   add->dateInSec = sec;
   add->suicides = suicide;
   add->next = 0;

   _curNumNames++;
   _curListPos = add; 

   if (prev != 0)
      prev->next = add;
   else                       // special case of nothing in the list
      pList = add;

}

/* *********** Delete an entry in the linked list *********************** */
void playerList::deleteName(const char  *name)
{

   struct _listStuff *prev = 0, *theOne = 0;
   int havematch = 0;

   /* find the entry to delete */
   for (theOne = pList; theOne > 0 ; prev = theOne, theOne = theOne->next) 
      if ( theOne->name == name) {
	     havematch = 1;
	     break;
	  }
  
   /* update the pointer to the "next" in the list before deleteing */
   if (havematch) {
      if (theOne != pList)
         prev->next = theOne->next;
      else
         pList = theOne->next;   // special case of deleting the 1st one
                                     // in the list
      _curNumNames--;
                                     
      delete theOne;
   }

}

/* ************ Return the number of players in the linked list  ******** */
int playerList::getNumNames()
{
   return _curNumNames; 
}

/* ********** Swap routine used for sorting the linked list ************* */
void playerList::swapStuff(_listStuff *p1, _listStuff *p2, _listStuff *prev)
{
	_listStuff *tmp;

	tmp = p1->next;
	if (p1 == pList) // special case of 1st entry in list
		pList = p2;

	if (prev > 0)
		prev->next = p2;

	p1->next = p2->next;
	p2->next = p1;
}
/* ************** Sort a the linked list ********************************* */
void playerList::sortListByPoints()
{
  struct _listStuff *iterator = 0, *prev;
  int switched;

  do {
    switched = FALSE;
	iterator = pList;
	prev = 0;
    while (iterator > 0) {
		if (iterator->points < iterator->next->points) {
				swapStuff(iterator,iterator->next,prev);
				switched = TRUE;
		}

		if (iterator->next == (_listStuff *)NULL)
			break;

		prev = iterator;
		iterator = iterator->next;

		if (iterator->next == (_listStuff *)NULL)
			break;
	}
  } while (switched == TRUE);
}

/* ************** Sort a the linked list ********************************* */
/* **
void playerList::sortListByPercent()
{
  struct _listStuff *iterator = 0, *prev;
  int switched;

  do {
    switched = FALSE;
	iterator = pList;
	prev = 0;
    while (iterator > 0) {
		if (iterator->percent < iterator->next->percent) {
				swapStuff(iterator,iterator->next,prev);
				switched = TRUE;
		}

		if (iterator->next == (_listStuff *)NULL)
			break;

		prev = iterator;
		iterator = iterator->next;

		if (iterator->next == (_listStuff *)NULL)
			break;
	}
  } while (switched == TRUE);
}
* */
/* ************** Sort a the linked list by kills ************************ */
// updated to sort by kills after points used to be after percent
void playerList::sortListByKills()
{
  struct _listStuff *iterator = 0, *prev;
  int switched;

  do {
    switched = FALSE;
	iterator = pList;
	prev = 0;
    while (iterator > 0) {
		if ( iterator->points == iterator->next->points ) {
			if (iterator->points > 0 &&
			    iterator->kills < iterator->next->kills) {
			 	  swapStuff(iterator,iterator->next,prev);
				   switched = TRUE;
			}
			else  { // percent is negative
				if (iterator->points < 0 && 
					iterator->deaths > iterator->next->deaths) {
				      swapStuff(iterator,iterator->next,prev);
				      switched = TRUE;
				}
			}
		}

		if (iterator->next == (_listStuff *)NULL)
			break;

		prev = iterator;
		iterator = iterator->next;

		if (iterator->next == (_listStuff *)NULL)
			break;
	}
  } while (switched == TRUE);
}

/* **************Calculate the percentage of wins and losses *********** */
void playerList::calculatePercent()
{
  struct _listStuff *iterator = 0;
  int top1, bottom1;
  double percnt;

  for (iterator = pList; iterator > 0; iterator = iterator->next) {
	  if (iterator->deaths == 0 && iterator->kills == 0)
		  iterator->percent = 0;
	  else {
		  top1 = iterator->kills;
		  bottom1 = iterator->kills + iterator->deaths;
          percnt = (double) top1/bottom1;
	      iterator->percent = percnt *100.0;
	  }
   }
}

/* ************ Calculate player points based on (kills + caps) - deaths = points ******************************** */

void playerList::calculatePoints()
{
	 int points;
	 struct _listStuff *iterator = 0;

	for(iterator = pList; iterator > 0; iterator = iterator->next)
		if(iterator->deaths == 0 && iterator->kills == 0 && iterator->suicides && iterator->caps == 0)
			iterator->points = 0;
		else
		{
			points = (iterator->kills + iterator->caps) - (iterator->deaths + iterator->suicides);
			iterator->points = points;
		}
}

/* ************ Update a players rank ************************************ */
void playerList::updateRank()
{
  struct _listStuff *iterator = 0;
  int i, rank = 1;
  double lastPoint = 0;
//// changed from percentage to points
  for (iterator = pList, i = 1; iterator > 0; iterator = iterator->next, i++) {
	  if (lastPoint > iterator->points || lastPoint == iterator->points)
		  ++rank;

	  lastPoint = iterator->points;
	  iterator->rank = rank;
  }
}

/* ********* Update the playerList ************************************ */
void playerList::updateList(char *name, int death, int kill, int cap, int suicide)
{
	long curDate = getDateInSec();

	_listStuff *lPtr;
	
	if (goodName(name) == FALSE)
		return;

	if ( (lPtr = findName(name)) ) {
		lPtr->deaths += death;
		lPtr->kills += kill;
		lPtr->caps += cap;
		lPtr->dateInSec = curDate;
		lPtr->suicides += suicide;
	}
	else 
	  insertName(name, death, kill, cap, curDate, suicide);
}

/* ********* Read in the list file & create the linked list ******** */
void playerList::readList(char *readFileName, int retention)
{
	FILE *fptr;
	char name[NAMELEN+1], *beg, *tok, temp[NUMLEN+1];
    long curDate = getDateInSec();
    long numSec = 0;
	int  numsuicide = 0;
	char *newLine;

	if ((fptr = fopen(readFileName,	"r")) == (FILE *)NULL) {
	    return;
	}

    char str[STRMAX+1];
	char date[DATELEN+1];

    if (fgets(date,STRMAX,fptr) == 0) {
	    //sprintf(temp,"Error reading date from %s",readFileName);
	    //writeerror(temp);
	    // Note on change:  Right now, the date isn't used anywhere.
	    // So, we will assume the file is not there or empty, and
	    // exit normally.  Things should happen fine after that.
	    return;
	}

	// get rid of the '\n'
	newLine = strchr(date,'\n');
	if (newLine)
		*newLine = '\0';
    if (date == (char *)NULL)
		getdatetime(date);

	strncpy(_listDate,date,DATELEN);
 
	while ((fgets(str, STRMAX, fptr)) != 0) {
		/* parse the stuff read from the list */
		// get rid of the '\n'
		newLine = strchr(str,'\n');
		if (newLine)
			*newLine = '\0';
		beg = str;
        tok = strchr(beg,'\001');
		strncpy(name,beg,tok-beg);
		name[tok-beg] = '\0';

		beg += tok-beg +1;
        tok = strchr(beg,'\001');
		strncpy(temp,beg,tok-beg);
		temp[tok-beg] = '\0';
		int numwin = atoi(temp);

		beg += tok-beg +1;
        tok = strchr(beg,'\001');

		strncpy(temp,beg,tok-beg);
		temp[tok-beg] = '\0';

//		if (tok == (char *)NULL) {
//		   strncpy(temp,beg,strlen(beg)-1);
//		   temp[strlen(beg)-1]='\0';
//		}
//		else { // there is the date in seconds following the losses 
//	       strncpy(temp,beg,tok-beg);
//		   temp[tok-beg] = '\0';
//		}
		int numloss = atoi(temp);


		beg += tok-beg +1;
        tok = strchr(beg,'\001');
		strncpy(temp,beg,tok-beg);
		temp[tok-beg] = '\0';
		int numcap = atoi(temp);
////////////////////////

		beg += tok-beg +1;

		tok = strchr(beg,'\001');

		/* now get the date/time in seconds */
		numSec=0;
		if (tok != (char *)NULL) {

//		    strncpy(temp,beg,strlen(beg));
			strncpy(temp,beg,tok-beg);
//		    temp[strlen(beg)]='\0';
     		temp[tok-beg] = '\0';
			numSec = atol(temp);
		}
        // if no time in file use current date/time
		if (numSec ==0)
			numSec = curDate;


		beg += tok-beg +1;
//        tok = strchr(beg,'\001');
		strncpy(temp,beg,strlen(beg));
		temp[strlen(beg)] = '\0';
		int numsuicide = atoi(temp);



		// if the time/date for the player exceeds the "expire" allowed
		// don't put them in the list ... just drop them
		if (needToDelete(numSec,curDate,retention) == FALSE)
		if (goodName(name) == TRUE)
		   insertName(name,numwin,numloss,numcap,numSec,numsuicide);

	}

	if (fptr)
	   fclose(fptr);	
}

/* ****** For debugging - "dump" the contents of the linked list ********* */
void playerList::dump()
{
	struct _listStuff *theOne = 0;

   for (theOne = pList; theOne > 0 ; theOne = theOne->next) {
	 cout<<theOne->name<<"\t"<<theOne->kills<<"\t"<<theOne->deaths<<
		 "\t"<<theOne->caps<<"\t"<<theOne->suicides<<"\t"<<theOne->dateInSec<<"\t"<<
		 theOne->points<<"\t"<<(double) theOne->percent<<"\t"<<
		 theOne->rank<<endl;
	 }
}

/* ******* Write out a player file ******************************** */
void playerList::writePlayerFile(const char *outputFile)
{
	FILE *fptr;
	_listStuff *theOne;
	char temp[STRMAX+1], win[NUMLEN+1], loss[NUMLEN+1], capture[NUMLEN+1], sec[NUMLEN+1], date2[DATELEN+1], suicide[NUMLEN+1];

	getdatetime(date2);
 

    if ((fptr = fopen(outputFile,"w")) != (FILE *)NULL) {
	   strcat(date2,"\n");
       fputs(date2,fptr);
	}
	else {
		sprintf(temp, "Error opening %s",outputFile);
		writeerror(temp);
		exit(1);
	}
    
	for (theOne = pList; theOne > 0 ; theOne = theOne->next)
		 {
		 //itoa(theOne->kills,win,10);
		 //itoa(theOne->deaths,loss,10);
		 //ltoa(theOne->dateInSec,sec,10);
		 sprintf(win, "%d", theOne->kills);
		 sprintf(loss, "%d", theOne->deaths);
		 sprintf(capture, "%d", theOne->caps);
		 sprintf(sec, "%lu", theOne->dateInSec);
		 sprintf(suicide, "%d", theOne->suicides);
		 sprintf(temp,"%s\001%s\001%s\001%s\001%s\001%s\n",theOne->name,win,loss,capture,sec,suicide);
		 fputs(temp,fptr);
		 } 

	fclose(fptr);         
}

/* ****** Delete the player file on disk  ********* */

void playerList::deletePlayerFile(char *filename)
{
	FILE *fptr;
    char date[30+1], date2[30+1];

	date[0] = '\0';
	getdatetime(date2);

	if ((fptr = fopen(filename,"r")) != (FILE *)NULL)
       fgets(date,STRMAX,fptr);

	// if the file wasn't there don't try to close it
	if (fptr)
	   fclose(fptr);

    // dates are not the same create a new file ... new file every day
	if (date != (char *)NULL && strncmp(date, date2, DATELEN) != 0)	{
	    fptr = fopen(filename,"w");
	    fclose(fptr);
	}
}

/* ****** process the player list ********* */

void playerList::processPlayerList(const char *filename)
{
   calculatePercent();
   calculatePoints();
//   sortListByPercent();
   sortListByKills();
   sortListByPoints();
   updateRank();
   writePlayerFile(filename);
}

/* *********************************************** */
int playerList::getNextPlayer()
{
	if (_curListPos != 0 && _curListPos->next != 0) {
	   _curListPos = _curListPos->next;
	   return TRUE;
	}

	return FALSE;

}

/* *********************************************** */
char * playerList::getName()
{
	if (_curListPos != 0) {
	   return _curListPos->name;
	}

	return "";

}
/* *********************************************** */
int playerList::getCaps()
{
	if (_curListPos != 0)
		return _curListPos->caps;

	return 0;

}
/* *********************************************** */
int playerList::getDeaths()
{
	if (_curListPos != 0)
		return _curListPos->deaths;

	return 0;

}

/* *********************************************** */
int playerList::getKills()
{
	if (_curListPos != 0)
		return _curListPos->kills;

	return 0;

}


/* *********************************************** */
int playerList::getRank()
{
	if (_curListPos != 0)
		return _curListPos->rank;

	return 0;

}

/* *********************************************** */
double playerList::getPercent()
{
	if (_curListPos != 0)
		return _curListPos->percent;

	return 0.0;

}
/* *********************************************** */
int playerList::getPoints()
{
	if (_curListPos != 0)
		return _curListPos->points;

	return 0;

}
/* *********************************************** */
int playerList::getSuicides()
{
	if (_curListPos != 0)
		return _curListPos->suicides;

	return 0;

}