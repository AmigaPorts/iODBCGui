#define ALL_REACTION_MACROS
#include <reaction/reaction_macros.h>
#include <reaction/reaction_prefs.h>

#include <dos/dos.h>
#include <intuition/intuition.h>
#include <classes/window.h>
#include <gadgets/layout.h>
#include <proto/listbrowser.h>
#include <gadgets/clicktab.h>
#include <images/label.h>
#include <gadgets/checkbox.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/clicktab.h>
#include <proto/space.h>
#include <proto/label.h>
#include <proto/bitmap.h>
#include <proto/checkbox.h>
#include <images/bitmap.h>

#include <classes/requester.h>
#include <libraries/gadtools.h>

#include <stdio.h>
#include <stdlib.h>

extern int32 Requester(uint32 img, TEXT *title, TEXT *reqtxt, TEXT *buttons, struct Window* win);
extern int ODBC_Connect(void);
extern int ODBC_Disconnect(void);
extern char *GetString(LONG id);

#include "iODBC_cat.h"
#include "version.h"
#include "debug.h"

#define OBJ(x) NObjects[x]
#define GAD(x) (struct Gadget *)NObjects[x]
#define SPACE    LAYOUT_AddChild, SpaceObject, End

extern struct ColumnInfo sysDrivers;
extern int listDrivers(Object *LBDRV, struct Window *win);

enum
{
    OBJ_WIN_NEWDSN,
    OBJ_WIN_LBDRIVERS,
    OBJ_WIN_STARTWIZARD,
    OBJ_WIN_FINISH,
    OBJ_WIN_CANCEL,
    OBJ_WIN_NUM
};

Object *NObjects[OBJ_WIN_NUM];

CONST_STRPTR newDSN(int *useWizard)
{
    Object *DSNwin;
    struct Window *DSNWindow;
    struct Screen *tempScreen;
    STRPTR tempDriver = NULL;

	if (ODBC_Connect()==0)
	{
		tempScreen = IIntuition->LockPubScreen("Workbench");

		DSNwin = (Object *)WindowObject,
				WA_ScreenTitle,        GetString(MSG_CHOOSE_DRIVER),
				WA_Title,              GetString(MSG_CHOOSE_DRIVER),
				WA_DragBar,            TRUE,
				WA_CloseGadget,        TRUE,
				WA_SizeGadget,         FALSE,
				WA_DepthGadget,        TRUE,
				WA_Activate,           TRUE,
				WA_InnerWidth,         500,
				WA_InnerHeight,        300,
				WINDOW_GadgetHelp,	   TRUE,
				WINDOW_IconifyGadget,  FALSE,
				WINDOW_Position,       WPOS_CENTERWINDOW,
				WINDOW_Layout,         VLayoutObject,
					LAYOUT_AddChild,       OBJ(OBJ_WIN_NEWDSN) = (Object *) VLayoutObject,
						LAYOUT_BevelStyle,    BVS_GROUP,
						LAYOUT_Label,         GetString(MSG_SELECT_DRIVER),
						LAYOUT_SpaceInner,    FALSE,
						LAYOUT_AddChild, HLayoutObject,
							LAYOUT_AddChild,    HLayoutObject,
								LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_AddImage, BitMapObject,
									BITMAP_SourceFile, "PROGDIR:images/ODBC_Splash",
									BITMAP_Screen,     tempScreen,
									BITMAP_Masking,    TRUE,
								BitMapEnd,
							End,
							CHILD_WeightedWidth,  9,
							LAYOUT_AddChild, OBJ(OBJ_WIN_LBDRIVERS) = ListBrowserObject,
								 GA_ID, 		OBJ_WIN_LBDRIVERS,
								 GA_RelVerify, 	TRUE,
								 GA_HintInfo,	GetString(MSG_HINT_LBDRIVERS),
								 LISTBROWSER_ColumnInfo,       &sysDrivers,
								 LISTBROWSER_ColumnTitles,     TRUE,
								 LISTBROWSER_Labels,           NULL,
								 LISTBROWSER_ShowSelected,     TRUE,
								 LISTBROWSER_HorizontalProp,   TRUE,
								 LISTBROWSER_Separators,       TRUE,
							End, //ListBrowserObject
							CHILD_WeightedWidth, 91,
						End, //HLayoutObject
					End,
					CHILD_WeightedHeight,  95,
					LAYOUT_AddChild,       HLayoutObject,
						LAYOUT_AddChild, OBJ(OBJ_WIN_STARTWIZARD) = (Object*) CheckBoxObject,
							GA_ID, 				OBJ_WIN_STARTWIZARD,
							GA_RelVerify, 		TRUE,
							GA_Text, 			GetString(MSG_CONNECTION_WIZARD),
							GA_HintInfo,		GetString(MSG_HINT_CONNECTION_WIZARD),
							CHECKBOX_TextPlace, PLACETEXT_RIGHT,
							CHECKBOX_Checked, 	FALSE,
						End,
						SPACE,
						SPACE,
						SPACE,
						LAYOUT_AddChild, OBJ(OBJ_WIN_FINISH) = ButtonObject,
											GA_Text, GetString(MSG_BUTTON_FINISH),
											GA_ID,             OBJ_WIN_FINISH,
											GA_Disabled,	   TRUE,
											GA_RelVerify, 	   TRUE,
					    ButtonEnd,
						CHILD_WeightedHeight,  0,
						LAYOUT_AddChild, OBJ(OBJ_WIN_CANCEL) = ButtonObject,
											GA_Text, GetString(MSG_BUTTON_CANCEL),
											GA_ID,             OBJ_WIN_CANCEL,
											GA_RelVerify, 	   TRUE,
					    ButtonEnd,
						CHILD_WeightedHeight,  0,
					End,
					CHILD_WeightedHeight,  5,
				End,       //HLayoutObject
			WindowEnd;     //WindowObject

		if ((DSNWindow = (struct Window*)IIntuition->IDoMethod(DSNwin, WM_OPEN)))
		{
			uint32
				sigmask     = 0,
				siggot      = 0,
				result      = 0;
			uint16
				code        = 0;
			BOOL
				done        = FALSE;

			listDrivers(OBJ(OBJ_WIN_LBDRIVERS), DSNWindow);

			IIntuition->GetAttr(WINDOW_SigMask, DSNwin, &sigmask);
			while (!done)
			{
					siggot = IExec->Wait(sigmask | SIGBREAKF_CTRL_C);
					if (siggot & SIGBREAKF_CTRL_C) done = TRUE;
					while ((result = IIntuition->IDoMethod(DSNwin, WM_HANDLEINPUT, &code)))
					{
						switch(result & WMHI_CLASSMASK)
						{
							case WMHI_CLOSEWINDOW:
								done = TRUE;
							break;
							case WMHI_GADGETUP:
								switch (result & WMHI_GADGETMASK)
								{
									case OBJ_WIN_LBDRIVERS:
							            IIntuition->SetGadgetAttrs(GAD(OBJ_WIN_FINISH), DSNWindow, NULL, GA_Disabled, FALSE, TAG_DONE);
									break;
									case OBJ_WIN_CANCEL:
										done = TRUE;
									break;
									case OBJ_WIN_FINISH:
									{
										struct Node *selectedNode = NULL;
										int dwSelected = 0;

										/* Check if driver wizard checkbox has been selected */
										IIntuition->GetAttrs(OBJ(OBJ_WIN_STARTWIZARD), CHECKBOX_Checked, &dwSelected, TAG_DONE);
										/* Pass back the result value */
										*useWizard = dwSelected;

										/* Get the selected driver */
										IIntuition->GetAttrs(OBJ(OBJ_WIN_LBDRIVERS), LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
										if (NULL!=selectedNode)
											IListBrowser->GetListBrowserNodeAttrs(selectedNode, LBNCA_Text, &tempDriver, TAG_DONE);
										done = TRUE;
									}
									break;
								}
							break;
						}
					}
			}
			IIntuition->DisposeObject(DSNwin);
		}
		else
		{
			Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_OPENING_DSNWIN), GetString(MSG_OK), NULL);
			return (CONST_STRPTR) NULL;
		}

		IIntuition->UnlockPubScreen(NULL, tempScreen);
		ODBC_Disconnect();

		/* Return the selected driver */
		return (CONST_STRPTR) tempDriver;
	}
	else
	{
		Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADING_ODBC), GetString(MSG_OK), NULL);
		return NULL;
	}
}
