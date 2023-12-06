// htmlFormatter.cpp: implementation of the htmlFormatter class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "StdAfx.h"
#include "htmlFormatter.h"
#include "playerList.h"

#define TAGBEG '<'
#define TAGEND '>'
#define LOOP "<loop>"
#define LOOPEND "</loop>"
#define FORWARD "<forward>"
#define BACKWARD "<backward>"

//extern int errno;

enum tagids {nameId, rankId, percentId, pointId, killId, deathId, capId, suicideId };

#define NUMTAG 8
#define HTMLMAX 500

static char *tagNames[] = {
  "<name>", "<rank>", "<percent>", "<point>", "<kill>", "<death>", "<cap>", "<suicides>"
};

struct {
	char tag[10];
	int tagID;
} tagData[NUMTAG+1];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

htmlFormatter::htmlFormatter(char *htmlTmpl, char *htmlOutput, playerList *plist)
{
	char temp[STRMAX+1];

	// open the html template file for input
	if ((tmplPtr = fopen(htmlTmpl,"r")) == NULL) {
		sprintf(temp,"Cannot open %s",htmlTmpl);
	    writeerror(temp);
	    //cout<<temp<<endl;
	    exit(1);
	}

	// open the output file where we will write the html formatted stuff
	if ((htmlOut = fopen(htmlOutput,"w")) == NULL) {
		sprintf(temp,"Cannot open %s error=%d",htmlOutput,errno);
	    writeerror(temp);
	    //cout<<temp<<endl;
	    exit(1);
	}

	list = plist;

	// build the stuct of tag info
	int i;
	for (i = nameId; i < NUMTAG; i++) {
		 strcpy(tagData[i].tag, tagNames[i]);
		 tagData[i].tagID = (tagids) i;
	}
}
//////////////////////////////////////////////////////////////////////
htmlFormatter::~htmlFormatter()
{
   if (tmplPtr)
	   fclose(tmplPtr);

   if (htmlOut)
	   fclose(htmlOut);
}

//////////////////////////////////////////////////////////////////////

#define LOOPMAX 20

int htmlFormatter::writeHtml()
{
	char temp[STRMAX+1], buffer[STRMAX+1];
	char loopData[LOOPMAX][STRMAX+1];
	int loopNum, i;

	// if we could not open either file return FALSE
	if (!tmplPtr || !htmlOut)
		return FALSE;

	while ((fgets(temp, STRMAX, tmplPtr)) != 0)
	{
		// this is the html stuff
		if (strnicmp(LOOP,temp,strlen(LOOP)) != 0)
		   fputs(temp,htmlOut);
		else {
			// read in the table template stuff
			for ( loopNum = 0; loopNum < LOOPMAX; loopNum++) {
				if ( fgets(temp, STRMAX, tmplPtr) != 0 &&
				    strnicmp(temp, LOOPEND, strlen(LOOPEND)) != 0)
			       strcpy(loopData[loopNum],temp);
				else 
					break;
			}

			// write out the contents of the list
			list->setToTopOfList();
			 do {
				for (i = 0; i < loopNum; i++)
				   if (buildALine(loopData[i], buffer) != 0)
					   fputs(buffer, htmlOut);
			} while (list->getNextPlayer() );
		
		}
	}
		    

return TRUE;
}

//////////////////////////////////////////////////////////////////////
int htmlFormatter::buildALine(char *in, char *out)
{
	char *tok, *beg = in, *cur = in;
	int match = FALSE, i, pos, len = 0;
	char temp[HTMLMAX+1];

	*out = '\0';
	
	while (strlen(cur) > 0) {
	   match = FALSE;
	   pos = 0;
	   tok = strchr(cur,TAGBEG);
	   if (tok == NULL) { // at the end of the string
		   strcat(out,cur);
		   break;
	   }

	   for (i = 0; i < NUMTAG; i++) {
	       if( strnicmp(tok,tagData[i].tag, strlen(tagData[i].tag)) == 0) {
			   len = strlen(tagData[i].tag);
		       pos = tok - beg;
		       match = TRUE;
		       break;
		   }
		   else {
		       continue;
		   }
		}

		if (match) {
			strncat(out, cur, tok-cur);

			switch (tagData[i].tagID) {

			case nameId: strcat(out, list->getName());
				break;

			case rankId: sprintf(temp,"%d",list->getRank());
				strcat(out, temp);
				break;

			case percentId: sprintf(temp,"%.2f",list->getPercent());
				strcat(out, temp);
				break;

			case pointId: sprintf(temp,"%d",list->getPoints());
				strcat(out, temp);
				break;

			case killId: sprintf(temp, "%d",list->getKills());
				strcat(out, temp);
				break;

			case deathId: sprintf(temp, "%d", list->getDeaths());
				strcat(out, temp);
				break;

			case capId: sprintf(temp, "%d", list->getCaps());
				strcat(out, temp);
				break;

			case suicideId: sprintf(temp, "%d", list->getSuicides());
				strcat(out, temp);
				break;

			}

		    beg += (pos + len);
		    cur = beg;
		}
	    else {
		  len = tok - cur + 1;
		  strncat(out,cur,len);
		  cur += len;
          continue;
		}

	}

	return TRUE;
}
