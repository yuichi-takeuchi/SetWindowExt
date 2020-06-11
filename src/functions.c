#include "XOPStandardHeaders.h"			// Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#define NO_INPUT_STRING 1 + FIRST_XOP_ERR
#define INPUT_STRING_TOO_LONG 2 + FIRST_XOP_ERR
// All structures are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=mac68k
#endif
#ifdef _WINDOWS_
	#pragma pack(2)
#endif

int SetProcedureText( struct {	double dFlag;
								Handle hFn;
								Handle hName;
								double dResult;}* p)
{
	int lenName;
	int lenFn;
	int iErr;
	if((p->hName == NULL) || (p->hFn == NULL))
	{
		p->dResult = NO_INPUT_STRING;
		return NO_INPUT_STRING;
	}

	lenName = GetHandleSize(p->hName);
	lenFn = GetHandleSize(p->hFn);

	SetHandleSize(p->hName, lenName + 1);
	if (iErr = MemError()) 
	{
		DisposeHandle(p->hName);
		DisposeHandle(p->hFn);
		p->dResult = iErr;
		return iErr;
	}
	*(*(p->hName)+lenName) = '\0';

	iErr = SetIgorProcedure(*p->hName, p->hFn, (int) p->dFlag);

	DisposeHandle(p->hName);
	//DisposeHandle(p->hFn);
	p->dResult = iErr;
	return iErr;

}
int DoRecreationDialog( struct {Handle hInStr; double dResult;}* p)
{
	int len;
	int iErr = 0;
	int index = 0;
	char szName[MAX_OBJ_NAME+1] = "";

	if (p->hInStr == NULL) return NO_INPUT_STRING;
	len = GetHandleSize(p->hInStr);
	if (len > MAX_OBJ_NAME) return INPUT_STRING_TOO_LONG; 

	for(index=0;index<len;index++)
	{
		szName[index] = *(*(p->hInStr) + index);
	}
	szName[len] = '\0';
	DisposeHandle(p->hInStr);

	p->dResult = DoWindowRecreationDialog(szName);
	iErr = SetIgorStringVar("S_FnName", szName, 0);

	return iErr;

}

// All structures are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=reset
#endif
#ifdef _WINDOWS_
	#pragma pack()
#endif
