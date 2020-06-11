// All structures are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=mac68k
#endif
#ifdef _WINDOWS_
	#pragma pack(2)
#endif

#include "XOPStandardHeaders.h"			// Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "WindowProcedures.h"

// This function is in SetWindowExt.c
int DoCleanup(int iWinType);

static WNDPROC gwpIgorWndProcs[5] = {NULL, NULL, NULL, NULL, NULL};

WNDPROC
GetIgorWndProc(int iWinType)
{
	switch (iWinType)
	{
	case GRAFWIN:	//GRAF_MASK:
		return gwpIgorWndProcs[0];
	case SSWIN:	//SS_MASK:
		return gwpIgorWndProcs[1];
	case PLWIN:	//PL_MASK:
		return gwpIgorWndProcs[2];
	case MWWIN:	//MW_MASK:
		return gwpIgorWndProcs[3];
	case PANELWIN:	//PANEL_MASK:
		return gwpIgorWndProcs[4];
	}
	return NULL;
}

void
SetIgorWndProc(int iWinType, WNDPROC wpIgorWndProc)
{
	switch (iWinType)
	{
	case GRAFWIN:
		gwpIgorWndProcs[0] = wpIgorWndProc;
		break;
	case SSWIN:
		gwpIgorWndProcs[1] = wpIgorWndProc;
		break;
	case PLWIN:
		gwpIgorWndProcs[2] = wpIgorWndProc;
		break;
	case MWWIN:
		gwpIgorWndProcs[3] = wpIgorWndProc;
		break;
	case PANELWIN:
		gwpIgorWndProcs[4] = wpIgorWndProc;
		break;
	}
	return;
}

WNDPROC
GetSWXWndProc(int iWinType)
{
	switch (iWinType)
	{
	case GRAFWIN:
		return (WNDPROC)SWXGraphProc;
	case SSWIN:
		return (WNDPROC)SWXTableProc;
	case PLWIN:
		return (WNDPROC)SWXPageLayoutProc;
	case MWWIN:
		return (WNDPROC)SWXNotebookProc;
	case PANELWIN:
		return (WNDPROC)SWXPanelProc;
	}
	return NULL;
}

WNDPROC
GetSWXInactiveWndProc(int iWinType)
{
	switch (iWinType)
	{
	case GRAFWIN:
		return (WNDPROC)SWXInactiveGraphProc;
	case SSWIN:
		return (WNDPROC)SWXInactiveTableProc;
	case PLWIN:
		return (WNDPROC)SWXInactivePageLayoutProc;
	case MWWIN:
		return (WNDPROC)SWXInactiveNotebookProc;
	case PANELWIN:
		return (WNDPROC)SWXInactivePanelProc;
	}
	return NULL;
}

static LRESULT APIENTRY
SWXGraphProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//give Igor a crack at this message
	if (SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 0)) return 0;

	if (!SWXActiveProc(uMsg, wParam, lParam))
	{
		return CallWindowProc((WNDPROC)SWXInactiveGraphProc, hwnd, uMsg, wParam, lParam);
	}
	else
	{
		// give Igor another crack at it
		SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 1);
		return 0;
	}
}

static LRESULT APIENTRY
SWXTableProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//give Igor a crack at this message
	if (SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 0)) return 0;

	if (!SWXActiveProc(uMsg, wParam, lParam))
	{
		return CallWindowProc((WNDPROC)SWXInactiveTableProc, hwnd, uMsg, wParam, lParam);
	}
	else
	{
		// give Igor another crack at it
		SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 1);
		return 0;
	}
}

static LRESULT APIENTRY
SWXPageLayoutProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//give Igor a crack at this message
	if (SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 0)) return 0;

	if (!SWXActiveProc(uMsg, wParam, lParam))
	{
		return CallWindowProc((WNDPROC)SWXInactivePageLayoutProc, hwnd, uMsg, wParam, lParam);
	}
	else
	{
		// give Igor another crack at it
		SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 1);
		return 0;
	}
}

static LRESULT APIENTRY
SWXNotebookProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//give Igor a crack at this message
	if (SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 0)) return 0;

	if (!SWXActiveProc(uMsg, wParam, lParam))
	{
		return CallWindowProc((WNDPROC)SWXInactiveNotebookProc, hwnd, uMsg, wParam, lParam);
	}
	else
	{
		// give Igor another crack at it
		SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 1);
		return 0;
	}
}

static LRESULT APIENTRY
SWXPanelProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//give Igor a crack at this message
	if (SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 0)) return 0;

	if (!SWXActiveProc(uMsg, wParam, lParam))
	{
		return CallWindowProc((WNDPROC)SWXInactivePanelProc, hwnd, uMsg, wParam, lParam);
	}
	else
	{
		// give Igor another crack at it
		SendWinMessageToIgor(hwnd, FixDblClk(uMsg), wParam, lParam, 1);
		return 0;
	}
}

static LRESULT APIENTRY
SWXInactiveGraphProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	HWND hwnd2;
	char window_name[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = gwpIgorWndProcs[0];

	if ((uMsg == WM_DESTROY) && (WinInfo(1, GRAF_MASK, window_name, &hwnd2) == 0))
	{
		DoCleanup(GRAFWIN);	
	}

	return CallWindowProc(wpIgorWndProc, hwnd, FixDblClk(uMsg), wParam, lParam);	
}

static LRESULT APIENTRY
SWXInactiveTableProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	HWND hwnd2;
	char window_name[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = gwpIgorWndProcs[1];
	
	if ((uMsg == WM_DESTROY) && (WinInfo(1, SS_MASK, window_name, &hwnd2) == 0))
	{
		DoCleanup(SSWIN);
	}

	return CallWindowProc(wpIgorWndProc, hwnd, FixDblClk(uMsg), wParam, lParam);	
}

static LRESULT APIENTRY
SWXInactivePageLayoutProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	HWND hwnd2;
	char window_name[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = gwpIgorWndProcs[2];
	
	if ((uMsg == WM_DESTROY) && (WinInfo(1, PL_MASK, window_name, &hwnd2) == 0))
	{
		DoCleanup(PLWIN);	
	}

	return CallWindowProc(wpIgorWndProc, hwnd, FixDblClk(uMsg), wParam, lParam);	
}

static LRESULT APIENTRY
SWXInactiveNotebookProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	HWND hwnd2;
	char window_name[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = gwpIgorWndProcs[3];
	
	if ((uMsg == WM_DESTROY) && (WinInfo(1, MW_MASK, window_name, &hwnd2) == 0))
	{
		DoCleanup(MWWIN);	
	}

	return CallWindowProc(wpIgorWndProc, hwnd, FixDblClk(uMsg), wParam, lParam);	
}

static LRESULT APIENTRY
SWXInactivePanelProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	HWND hwnd2;
	char window_name[MAX_OBJ_NAME+1];
	WNDPROC wpIgorWndProc = gwpIgorWndProcs[4];
	
	if ((uMsg == WM_DESTROY) && (WinInfo(1, PANEL_MASK, window_name, &hwnd2) == 0))
	{
		DoCleanup(PANELWIN);	
	}

	return CallWindowProc(wpIgorWndProc, hwnd, FixDblClk(uMsg), wParam, lParam);	
}

static int
SWXActiveProc(UINT uMsg,WPARAM wParam,LPARAM lParam) 
{ 
	char szHookname[256];
	char szCommand[256];
	char szEvent[256];
	char szData[256];
	char szModifiers[256];
	long lMod = 0;
	long lKey = 0;
	short sWheelDelta = 0;

	double dReal = 0;
	double dImag = 0;
	int iResult = 1;

	switch(uMsg) {
		case WM_CLOSE:
			sprintf(szEvent, "EVENT:close;");
			sprintf(szData, "");
			lMod |= (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
			lMod |= (GetAsyncKeyState(VK_SHIFT  ) & 0x8000) ? 2 : 0;
			lMod |= (GetAsyncKeyState(VK_MENU   ) & 0x8000) ? 4 : 0;
			lMod |= (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? 8 : 0;
			lMod |= (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 16: 0;
			lMod |= (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? 32: 0;
			sprintf(szModifiers, "MODIFIERS:%ld;",lMod);
			break;
		case WM_KEYDOWN:
			sprintf(szEvent, "EVENT:keydown;");
			goto allkey;	//Spaghetti time!
		case WM_KEYUP:
			sprintf(szEvent, "EVENT:keyup;");
		allkey:
			sprintf(szData, "KEY:%ld;PREV:%ld;",wParam,((lParam & 0x40000000) ? 1 : 0));
			lMod |= (GetKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
			lMod |= (GetKeyState(VK_SHIFT  ) & 0x8000) ? 2 : 0;
			lMod |= (GetKeyState(VK_MENU   ) & 0x8000) ? 4 : 0;
			lMod |= (GetKeyState(VK_CONTROL) & 0x8000) ? 8 : 0;
			lMod |= (GetKeyState(VK_RBUTTON) & 0x8000) ? 16: 0;
			lMod |= (GetKeyState(VK_MBUTTON) & 0x8000) ? 32: 0;
			sprintf(szModifiers, "MODIFIERS:%ld;",lMod);
			break;
		case WM_LBUTTONDOWN:
			sprintf(szEvent, "EVENT:mousedown;");
			goto allmouse;
		case WM_MBUTTONDOWN:
			sprintf(szEvent, "EVENT:mmousedown;");
			goto allmouse;
		case WM_RBUTTONDOWN:
			sprintf(szEvent, "EVENT:rmousedown;");
			goto allmouse;
		case WM_LBUTTONUP:
			sprintf(szEvent, "EVENT:mouseup;");
			goto allmouse;
		case WM_MBUTTONUP:
			sprintf(szEvent, "EVENT:mmouseup;");
			goto allmouse;
		case WM_RBUTTONUP:
			sprintf(szEvent, "EVENT:rmouseup;");
			goto allmouse;
		case WM_LBUTTONDBLCLK:
			sprintf(szEvent, "EVENT:mousedblclk;");
			goto allmouse;
		case WM_MBUTTONDBLCLK:
			sprintf(szEvent, "EVENT:mmousedblclk;");
			goto allmouse;
		case WM_RBUTTONDBLCLK:
			sprintf(szEvent, "EVENT:rmousedblclk;");
			goto allmouse;
		//case MSH_MOUSEWHEEL:
		case WM_MOUSEWHEEL:
			sWheelDelta = wParam >> 16;
			sprintf(szEvent, "EVENT:mousewheel;DELTA:%ld;",sWheelDelta);
		allmouse:
			sprintf(szData, "MOUSEX:%ld;MOUSEY:%ld;",GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
			lMod |= (wParam & MK_LBUTTON) ? 1 : 0;
			lMod |= (wParam & MK_SHIFT  ) ? 2 : 0;
			lMod |= (GetAsyncKeyState(VK_MENU) & 0x8000)  ? 4 : 0;
			lMod |= (wParam & MK_CONTROL) ? 8 : 0;
			lMod |= (wParam & MK_RBUTTON) ? 16: 0;
			lMod |= (wParam & MK_MBUTTON) ? 32: 0;
			sprintf(szModifiers, "MODIFIERS:%ld;",lMod);
			break;

		default:
			iResult = 0;
	}

	//sprintf(szCommand, "uMsg = 0x%08X\r", uMsg); XOPNotice(szCommand);

	if (iResult) {
		XOPSilentCommand("GetWindow kwTopWin, hook");
		FetchStrVar("S_Value",szHookname);
		if (strcmp(szHookname, "")!=0) 
		{
			// The Execute/Z is necessary in order to supress error messages.
			sprintf(szCommand, "Execute/Z \"Variable/G V_Value = %s(\\\"WINDOW:\\\"+WinName(0,1)+\\\";%s%sTICKS:%ld;%s\\\")\"",szHookname,szEvent,szData,TickCount(),szModifiers);
			XOPSilentCommand(szCommand);

			FetchNumVar("V_Flag", &dReal,&dImag);
			iResult = (int) (dReal);	// This cast seems to work okay

			if (iResult) 
			{
				//XOPNotice(szCommand);
				XOPNotice("Warning:  SetWindowExt cannot reach hook function.\r");
				iResult = 0;
			}
			else
			{
				FetchNumVar("V_Value", &dReal,&dImag);
				iResult = (int) (dReal);	// This cast seems to work okay
			}
		}
	}

	return iResult;
} 

static UINT FixDblClk(UINT uMsg)
{
	switch(uMsg) { 
		case WM_LBUTTONDBLCLK:
			return WM_LBUTTONDOWN;
		case WM_MBUTTONDBLCLK:
			return WM_MBUTTONDOWN;
		case WM_RBUTTONDBLCLK:
			return WM_RBUTTONDOWN;
		default:
			return uMsg;
	}
}

int Type2Mask(iWinType)
{
	switch(iWinType)
	{
	case GRAFWIN:
		return GRAF_MASK;
	case SSWIN:
		return SS_MASK;
	case PLWIN:
		return PL_MASK;
	case MWWIN:
		return MW_MASK;
	case PANELWIN:
		return PANEL_MASK;
	default:
		return 0;
	}
}

// All structures are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=reset
#endif
#ifdef _WINDOWS_
	#pragma pack()
#endif