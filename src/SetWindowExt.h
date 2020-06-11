#define SWXVERSION			100
#define IGORTARGETWINDOWS	GRAF_MASK | SS_MASK | PL_MASK | MW_MASK | PANEL_MASK

#include "WindowProcedures.h"
#include "functions.h"

struct SWXSettings {	// Settings stored for SWX as a whole
	int iVersion;		// Version number of this structure
	int iNumWindows;	// Number of windows actively using SWX
	int iSpare[30];		// Reserved for future
};
// Immediately following the SWXSettings structure is an "array"
// of iNumWindows char [MAX_OBJ_NAME + 1] strings with the names
// of windows actively using SWX.  This "array" is dynamic, hence
// it cannot be included in a C struct.

typedef struct SWXSettings SWXSettings;
typedef SWXSettings* SWXSettingsPtr;
typedef SWXSettingsPtr* SWXSettingsHandle;

/* Prototypes */
static int DoCmd(void);
static int DoCmd2(char szInputWindowName[MAX_OBJ_NAME+1], int flag);
static int DoDebugCmd(void);

int DoCleanup(int iWinType);
static int DoSetup(int iWinType);

static SWXSettingsHandle DoLoadSettings(void);
static void DoAfterLoad(const SWXSettingsHandle vsHandle);
static SWXSettingsHandle DoSaveSettings(void);
static void XOPEntry(void);
HOST_IMPORT void main(IORecHandle ioRecHandle);