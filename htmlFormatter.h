// htmlFormatter.h: interface for the htmlFormatter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLFORMATTER_H__167CC3E0_6C29_11D2_B762_00C0F02907C3__INCLUDED_)
#define AFX_HTMLFORMATTER_H__167CC3E0_6C29_11D2_B762_00C0F02907C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "playerList.h"

class htmlFormatter  
{
public:
			htmlFormatter(char *htmlTmpl, char *htmlOutput, playerList *plist);
	virtual ~htmlFormatter();
	int		 writeHtml();

private:
	FILE		*tmplPtr;
	FILE		*htmlOut;
	playerList	*list;
	int			buildALine(char *in, char *out);
};

#endif // !defined(AFX_HTMLFORMATTER_H__167CC3E0_6C29_11D2_B762_00C0F02907C3__INCLUDED_)
