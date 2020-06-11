#include <Windowsx.h>
//#include <ZMouse.h>

#define WM_MOUSEWHEEL		0x020A

WNDPROC GetIgorWndProc(int iWinType);
WNDPROC GetSWXWndProc(int iWinType);
WNDPROC GetSWXInactiveWndProc(int iWinType);
void SetIgorWndProc(int iWinType, WNDPROC wpIgorWinProc);

static LRESULT APIENTRY SWXGraphProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
static LRESULT APIENTRY SWXTableProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
static LRESULT APIENTRY SWXPageLayoutProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
static LRESULT APIENTRY SWXNotebookProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
static LRESULT APIENTRY SWXPanelProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

static LRESULT APIENTRY SWXInactiveGraphProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
static LRESULT APIENTRY SWXInactiveTableProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
static LRESULT APIENTRY SWXInactivePageLayoutProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
static LRESULT APIENTRY SWXInactiveNotebookProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
static LRESULT APIENTRY SWXInactivePanelProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 

static int SWXActiveProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
static UINT FixDblClk(UINT uMsg);
int Type2Mask(iWinType);

