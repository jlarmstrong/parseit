#if !defined(_DLL_DATA_INCLUDED_)
#define _DLL_DATA_INCLUDED_

#define MSGTYPE_KILL	1

typedef struct _returnMsg {
	int nMessageType;
	char winname[STRMAX+1];
	char losename[STRMAX+1];
	int winner;
} returnMsg;

#endif