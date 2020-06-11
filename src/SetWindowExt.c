// All structures are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=mac68k
#endif
#ifdef _WINDOWS_
	#pragma pack(2)
#endif

#include "XOPStandardHeaders.h"			// Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "SetWindowExt.h"

/* Global Variables */
int giSettingsModified = FALSE;				// True if need to save new settings.
char gszBuffer[256];

/*	DoCmd()

	DoCmd() is called when XOP is called from the host application with message = CMD.

	DoCmd() parses the command line and does the appropriate action.
	It returns 0 if everything went OK or error code otherwise.
*/
static int
DoCmd(void)
{
	int iResult = 0;
	int flag = 0;
	char szInputWindowName[MAX_OBJ_NAME+1];

	//check for flag "/U"
	flag = GetFlag("UD");
	if (flag == (int)'D') return DoDebugCmd();
	if (flag < 0) return(BADFLG);		//bad flag
	if (GetFlag("")) return(SYNERR);	//syntax error (extra flags)
		
	//get name of graph
	if (iResult = GetName(szInputWindowName)) return(iResult);

	//make sure no extraneous garbage in cmd
	if (iResult = CheckTerm())	return(iResult);

	return DoCmd2(szInputWindowName, flag);
}

static int
DoDebugCmd(void)
{
	HWND hwnd;
	int index = 0;
	int iWinType = 0;
	long wpWndProc = 0;
	char szCurrentWindowName[MAX_OBJ_NAME+1];

	sprintf(gszBuffer,"Type\tIgorWndProc\tSWXActiveWndProc\tSWXInActiveWndProc\r");
	XOPNotice(gszBuffer);
	sprintf(gszBuffer,"%3d\t\t0x%08X\t\t0x%08X\t\t0x%08X\r",GRAFWIN,GetIgorWndProc(GRAFWIN),GetSWXWndProc(GRAFWIN),GetSWXInactiveWndProc(GRAFWIN));
	XOPNotice(gszBuffer);
	sprintf(gszBuffer,"%3d\t\t0x%08X\t\t0x%08X\t\t0x%08X\r",SSWIN,GetIgorWndProc(SSWIN),GetSWXWndProc(SSWIN),GetSWXInactiveWndProc(SSWIN));
	XOPNotice(gszBuffer);
	sprintf(gszBuffer,"%3d\t\t0x%08X\t\t0x%08X\t\t0x%08X\r",PLWIN,GetIgorWndProc(PLWIN),GetSWXWndProc(PLWIN),GetSWXInactiveWndProc(PLWIN));
	XOPNotice(gszBuffer);
	sprintf(gszBuffer,"%3d\t\t0x%08X\t\t0x%08X\t\t0x%08X\r",MWWIN,GetIgorWndProc(MWWIN),GetSWXWndProc(MWWIN),GetSWXInactiveWndProc(MWWIN));
	XOPNotice(gszBuffer);
	sprintf(gszBuffer,"%3d\t\t0x%08X\t\t0x%08X\t\t0x%08X\r",PANELWIN,GetIgorWndProc(PANELWIN),GetSWXWndProc(PANELWIN),GetSWXInactiveWndProc(PANELWIN));
	XOPNotice(gszBuffer);

	sprintf(gszBuffer,"\rIndex\t%-32s\tType\tWndProc\r","Name");
	XOPNotice(gszBuffer);
	index = 0;
	while(iWinType = WinInfo(index, IGORTARGETWINDOWS, szCurrentWindowName, &hwnd))
	{
		wpWndProc = GetWindowLong(hwnd, GWL_WNDPROC);
		index++;
		sprintf(gszBuffer,"%3d\t\t%-32s\t%2d\t\t0x%08X\r",index, szCurrentWindowName, iWinType, wpWndProc);
		XOPNotice(gszBuffer);
	}

	return 0;
}

static int
DoCmd2(char szInputWindowName[MAX_OBJ_NAME+1], int flag)
{
	HWND hwnd;
	int index = 0;
	int iResult = 0;
	int iWinType = 0;
	char szCurrentWindowName[MAX_OBJ_NAME+1];

	Capitalize(szInputWindowName);
	if (strcmp("KWTOPWIN", szInputWindowName)==0)
	{
		iWinType = WinInfo(0, IGORTARGETWINDOWS, szInputWindowName, &hwnd);
	}
	else
	{
		index = 0;
		do
		{
			iWinType = WinInfo(index, IGORTARGETWINDOWS, szCurrentWindowName, &hwnd);
			Capitalize(szCurrentWindowName);
			if (strcmp(szCurrentWindowName, szInputWindowName) == 0) break;
			index++;
		}
		while(iWinType);
	}
	if (iWinType==0) return(EXPECTED_TARG_NAME);

	SetLastError(0);

	if (flag && (GetIgorWndProc(iWinType) != NULL))
	{
		iResult = SetWindowLong(hwnd, GWL_WNDPROC, (LONG) GetIgorWndProc(iWinType));
	}
	else {
		if (GetIgorWndProc(iWinType) == NULL) DoSetup(iWinType);	//do initialization
	
		iResult = SetWindowLong(hwnd, GWL_WNDPROC, (LONG) GetSWXWndProc(iWinType));
	}
	if (iResult == 0) return 0;
	iResult = GetLastError();
	if (iResult) return WindowsErrorToIgorError(iResult);
	giSettingsModified = TRUE;
	return 0;
}


/*	DoCleanup()

	DoCleanup() is called when XOP is called from the host application
	with message = CLEANUP or NEW.  

	It returns 0 if everything went OK or error code otherwise.
*/
int
DoCleanup(int iWinType)
{
	HWND hwnd;
	int index = 0;
	int iResult = 0;
	int iWinMask = Type2Mask(iWinType);
	char szWindowName[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = GetIgorWndProc(iWinType);

	if (wpIgorWndProc != NULL)
	{
		if (WinInfo(0, iWinMask, szWindowName, &hwnd))
		{
			//sprintf(gszBuffer,"SetWindowExt is deinitializing Igor window type #%d.\r", iWinType);
			//XOPNotice(gszBuffer);

			SetLastError(0);
			SetClassLong(hwnd, GCL_STYLE, GetClassLong(hwnd, GCL_STYLE) & ~CS_DBLCLKS);
			iResult = GetLastError();
			if (iResult) return WindowsErrorToIgorError(iResult);

			SetLastError(0);
			SetClassLong(hwnd, GCL_WNDPROC, (LONG) wpIgorWndProc);
			iResult = GetLastError();
			if (iResult) return WindowsErrorToIgorError(iResult);

			do
			{
				//sprintf(gszBuffer,"SetWindowExt is deinitializing window #%ld:  %s.\r", index, szWindowName);
				//XOPNotice(gszBuffer);

				SetLastError(0);
				iResult = SetWindowLong(hwnd, GWL_WNDPROC, (LONG) wpIgorWndProc);
				if ((iResult != (LONG) GetSWXWndProc(iWinType)) && (iResult != (LONG) GetSWXInactiveWndProc(iWinType))) 
				{
					sprintf(gszBuffer,"Warning:  SetWindowExt detected an unexpected window procedure (address 0x%08X) during deinitialization of window #%ld:  %s.\r", iResult, index, szWindowName);
					XOPNotice(gszBuffer);
				}
				iResult = GetLastError();
				if (iResult) return WindowsErrorToIgorError(iResult);
			}
			while (WinInfo(++index, iWinMask, szWindowName, &hwnd));
		}
		else
		{
			sprintf (gszBuffer, "Warning:  SetWindowExt tried to deinitalize Igor window type #%d, but no windows of this type exist.\r", iWinType);
			XOPNotice(gszBuffer);
		}
		SetIgorWndProc(iWinType, NULL);
	}
	return 0;
}

static int
DoSetup(int iWinType)
{
	HWND hwnd;
	int index = 0;
	int iResult = 0;
	int iWinMask = Type2Mask(iWinType);
	char szWindowName[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = NULL;

	if(WinInfo(index, iWinMask, szWindowName, &hwnd)) {
		//sprintf(gszBuffer,"SetWindowExt is initializing Igor window type #%d.\r", iWinType);
		//XOPNotice(gszBuffer);
			
		wpIgorWndProc = (WNDPROC) GetWindowLong(hwnd, GWL_WNDPROC);
		SetIgorWndProc(iWinType, wpIgorWndProc );
		//sprintf(gszBuffer,"SetWindowExt found Igor window type #%d to have its window procedure at address 0x%08X.\r", iWinType, wpIgorWndProc);
		//XOPNotice(gszBuffer);

		SetLastError(0);
		SetClassLong(hwnd, GCL_STYLE, GetClassLong(hwnd, GCL_STYLE) | CS_DBLCLKS);
		iResult = GetLastError();
		if (iResult) return WindowsErrorToIgorError(iResult);

		SetLastError(0);
		SetClassLong(hwnd, GCL_WNDPROC, (LONG) GetSWXInactiveWndProc(iWinType));
		iResult = GetLastError();
		if (iResult) return WindowsErrorToIgorError(iResult);

		do 
		{
			//sprintf(gszBuffer,"SetWindowExt is initializing window #%ld:  %s.\r", index, szWindowName);
			//XOPNotice(gszBuffer);
			SetLastError(0);
			iResult = SetWindowLong(hwnd, GWL_WNDPROC, (LONG) GetSWXInactiveWndProc(iWinType));
				if (iResult != (LONG) wpIgorWndProc) 
				{
					sprintf(gszBuffer,"Warning:  SetWindowExt detected an unexpected window procedure during initialization of window #%ld:  %s.\r", index, szWindowName);
					XOPNotice(gszBuffer);
				}
			iResult = GetLastError();
			if (iResult) return WindowsErrorToIgorError(iResult);
		}
		while(WinInfo(++index, iWinMask, szWindowName, &hwnd));
	}

	return 0;
}

static SWXSettingsHandle
DoLoadSettings(void)
{
	SWXSettingsHandle vsHandle;

	// NOTE: This handle still belongs to IGOR.
	vsHandle = (SWXSettingsHandle)GetXOPItem(0);

	// Copy the handle, so that it belongs to SWX.
	if (vsHandle != NULL) HandToHand(&(Handle)vsHandle);

	return(vsHandle);
}

static void
DoAfterLoad(const SWXSettingsHandle vsHandle)
{
	SWXSettingsPtr vsPtr;
	int index = 0;
	char* pszActiveWindowName;

	if (vsHandle == NULL) return;	// We did not save settings in this experiment.

	vsPtr = *vsHandle;
	pszActiveWindowName = vsPtr + 1;

	//sprintf(gszBuffer,"SetWindowExt SettingsSize %d\r", GetHandleSize(vsHandle));
	//XOPNotice(gszBuffer);
	//sprintf(gszBuffer,"SetWindowExt Version %d\r", vsPtr->iVersion);
	//XOPNotice(gszBuffer);
	//sprintf(gszBuffer,"SetWindowExt Number of Active Windows %d\r", vsPtr->iNumWindows);
	//XOPNotice(gszBuffer);

	for (index = 0; index < vsPtr->iNumWindows; index++)
	{
		//sprintf(gszBuffer,"SetWindowExt %s\r", pszActiveWindowName);
		//XOPNotice(gszBuffer);
		DoCmd2(pszActiveWindowName, 0);
		pszActiveWindowName += MAX_OBJ_NAME + 1;
	}
	return;
}

static SWXSettingsHandle
DoSaveSettings(void)
{
	int numBytes;
	int iWinType = 0;
	int index = 0;
	int iNumWindows = 0;
	char szCurrentWindowName[MAX_OBJ_NAME+1];
	char* pszActiveWindowName;
	HWND hwnd;
	SWXSettingsHandle vsHandle;
	SWXSettingsPtr vsPtr;

	iWinType = WinInfo(0, IGORTARGETWINDOWS, szCurrentWindowName, &hwnd);
	while(iWinType)
	{
		if ((WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC) == GetSWXWndProc(iWinType)) iNumWindows++;
		iWinType = WinInfo(++index, IGORTARGETWINDOWS, szCurrentWindowName, &hwnd);
	}

	numBytes = sizeof(SWXSettings) + iNumWindows * (MAX_OBJ_NAME + 1);
	vsHandle = (SWXSettingsHandle)NewHandle(numBytes);
	if (vsHandle != NULL)
	{
		vsPtr = *vsHandle;
		pszActiveWindowName = vsPtr + 1;

		MemClear(vsPtr, numBytes);
		vsPtr->iVersion = SWXVERSION;
		vsPtr->iNumWindows = iNumWindows;

		index = 0;
		iWinType = WinInfo(0, IGORTARGETWINDOWS, szCurrentWindowName, &hwnd);
		while(iWinType)
		{
			if ((WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC) == GetSWXWndProc(iWinType)) 
			{
				strcpy(pszActiveWindowName, szCurrentWindowName);
				pszActiveWindowName += MAX_OBJ_NAME + 1;
			}
			iWinType = WinInfo(++index, IGORTARGETWINDOWS, szCurrentWindowName, &hwnd);
		}
	}
	return vsHandle;
}

/*	XOPEntry()

	This is the entry point from the host application to the XOP for all
	messages after the INIT message.
*/
static void
XOPEntry(void)
{	
	long iResult = 0;
	static long lLastXOPMessage = 0;
	long lCurrentXOPMessage = GetXOPMessage();
	static SWXSettingsHandle vsHandle = NULL;
	
	switch (lCurrentXOPMessage) {
		case CMD:
			//XOPNotice("CMD\r");
			iResult = DoCmd();
			break;
		case CLEANUP:
			// XOPNotice("CLEANUP\r");
		case NEW:
			//if (lCurrentXOPMessage == NEW) XOPNotice("NEW\r");
			iResult = DoCleanup(GRAFWIN);
			if (iResult) break;
			iResult = DoCleanup(SSWIN);
			if (iResult) break;
			iResult = DoCleanup(PLWIN);
			if (iResult) break;
			iResult = DoCleanup(MWWIN);
			if (iResult) break;
			iResult = DoCleanup(PANELWIN);
			break;
		case LOADSETTINGS:
			//XOPNotice("LOADSETTINGS\r");			
			vsHandle = DoLoadSettings();
			break;		
		case SAVESETTINGS:
			//XOPNotice("SAVESETTINGS\r");			
			iResult = (long) DoSaveSettings();
			break;
		case CLEAR_MODIFIED:
			/*switch (iLastXOPMessage) {
				case LOADSETTINGS:
					XOPNotice("CLEAR_MODIFIED after LOADSETTINGS\r");
					break;
				case SAVESETTINGS:
					XOPNotice("CLEAR_MODIFIED after SAVESETTINGS\r");
					break;
				case NEW:
					XOPNotice("CLEAR_MODIFIED after NEW\r");
					break;
				case CLEAR_MODIFIED:
					XOPNotice("CLEAR_MODIFIED after CLEAR_MODIFIED\r");
					break;
				default:
					XOPNotice("CLEAR_MODIFIED after other\r");
			}*/
			if (lLastXOPMessage == LOADSETTINGS) 
			{
				DoAfterLoad(vsHandle);
				DisposeHandle(vsHandle);
			}
			giSettingsModified = FALSE;
			break;
		case MODIFIED:
			//XOPNotice("MODIFIED\r");
			iResult = giSettingsModified;
			break;
		case FUNCADDRS:
			switch (GetXOPItem(0))
			{
				case 0:						
					iResult = (long) SetProcedureText;	
					break;
				case 1:						
					iResult = (long) DoRecreationDialog;			
					break;
			}
			break;
		default:
			//XOPNotice("Unhandled message.\r");
			break;
	}
	SetXOPResult(iResult);
	lLastXOPMessage = lCurrentXOPMessage;
	return;
}

/*	main(ioRecHandle)

	This is the initial entry point at which the host application calls XOP.
	The message sent by the host must be INIT.
	main() does any necessary initialization and then sets the XOPEntry field of the
	ioRecHandle to the address to be called for future messages.
*/
HOST_IMPORT void
main(IORecHandle ioRecHandle)
{	
	
	#ifdef XOP_GLOBALS_ARE_A4_BASED
		#ifdef __MWERKS__
			// For CodeWarrior 68K XOPs.
			SetCurrentA4();							// Set up correct A4. This allows globals to work.
			SendXOPA4ToIgor(ioRecHandle, GetA4());	// And communicate it to Igor.
		#endif
	#endif
	
	XOPInit(ioRecHandle);	//do standard XOP initialization
	SetXOPEntry(XOPEntry);	//set entry point for future calls 

	if (igorVersion < 400) 
	{
		SetXOPResult(1L);
		return;
	}

	SetXOPType(RESIDENT);	//this XOP _must_ be resident
	SetXOPResult(0L);
	return;
}

// All structures are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=reset
#endif
#ifdef _WINDOWS_
	#pragma pack()
#endif
