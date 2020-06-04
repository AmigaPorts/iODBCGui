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
#include <gadgets/string.h>

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
#include <proto/string.h>

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

#define OBJ(x) WObjects[x]
#define GAD(x) (struct Gadget *)WObjects[x]
#define SPACE    LAYOUT_AddChild, SpaceObject, End

int StepNum = 0; /* The Wizard Step */

enum
{
    OBJ_WIN_WIZ_NEWDSN,
    OBJ_WIN_WIZ_WIZARD,
    OBJ_WIN_WIZ_SPLASH,
    OBJ_WIN_WIZ_SERVERNAME,
    OBJ_WIN_WIZ_DATABASENAME,
    OBJ_WIN_WIZ_DSNNAME,
    OBJ_WIN_WIZ_DSNDESCRIPTION,
    OBJ_WIN_WIZ_BUTTONS,
	OBJ_WIN_WIZ_NEXT,
    OBJ_WIN_WIZ_FINISH,
    OBJ_WIN_WIZ_CANCEL,
    OBJ_WIN_WIZ_NUM
};

Object *WObjects[OBJ_WIN_WIZ_NUM];

void ChangeStep(struct Window *tempWin, STRPTR ServerName, STRPTR DatabaseName, STRPTR DSNName, STRPTR DSNDescription)
{
	Object *tempNext = NULL;
	Object *tempSTEP1 = NULL;

	IIntuition->SetGadgetAttrs(GAD(OBJ_WIN_WIZ_BUTTONS), tempWin, NULL,
		LAYOUT_ModifyChild, OBJ(OBJ_WIN_WIZ_NEXT),
			CHILD_ReplaceObject, tempNext =  ButtonObject,
					GA_Text, 		   (StepNum==0) ? GetString(MSG_BUTTON_NEXT) : GetString(MSG_BUTTON_BACK),
					GA_ID,             OBJ_WIN_WIZ_NEXT,
					GA_RelVerify, 	   TRUE,
			ButtonEnd,
		TAG_DONE);

	if (tempNext)
		OBJ(OBJ_WIN_WIZ_NEXT) = tempNext;

	switch (StepNum)
	{
		case 0:
		{
			IIntuition->SetGadgetAttrs(GAD(OBJ_WIN_WIZ_SPLASH), tempWin, NULL,
				LAYOUT_ModifyChild, OBJ(OBJ_WIN_WIZ_WIZARD),
					CHILD_ReplaceObject, tempSTEP1 = VLayoutObject,
							LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_AddChild,    VLayoutObject,
										LAYOUT_AddImage, LabelObject,
										   LABEL_WordWrap, 72,
										   LABEL_Text, GetString(MSG_WIZARD_STEP1_DESC),
										LabelEnd,
								End,
								CHILD_WeightedHeight,  9,
								LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,  BVS_GROUP,
								LAYOUT_Label,		GetString(MSG_INSERT_SERVERNAME),
									LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_SERVERNAME) = (Object *) StringObject ,
										GA_ID,              OBJ_WIN_WIZ_SERVERNAME,
										GA_RelVerify,       TRUE,
										GA_TabCycle,        TRUE,
										GA_HintInfo,		GetString(MSG_HINT_WIZARD_SERVERNAME),
										STRINGA_MaxChars,   254,
										STRINGA_MinVisible, 13,
										STRINGA_TextVal,	ServerName==NULL ? "" : ServerName,
									End,
									CHILD_MaxWidth, 300,
									LAYOUT_AddChild,    VLayoutObject,
											LAYOUT_AddImage, LabelObject,
											   LABEL_WordWrap, 72,
											   LABEL_Text, GetString(MSG_SERVERNAME_DESC),
											LabelEnd,
									End,
								End,
								LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_Label,		GetString(MSG_INSERT_DATABASE),
									LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_DATABASENAME) = (Object *) StringObject ,
										GA_ID,              OBJ_WIN_WIZ_DATABASENAME,
										GA_RelVerify,       TRUE,
										GA_TabCycle,        TRUE,
										GA_HintInfo,		GetString(MSG_HINT_WIZARD_DATABASENAME),
										STRINGA_MaxChars,   254,
										STRINGA_MinVisible, 13,
										STRINGA_TextVal,	DatabaseName==NULL ? "" : DatabaseName,
									End,
									CHILD_MaxWidth, 300,
									LAYOUT_AddChild,    VLayoutObject,
											LAYOUT_AddImage, LabelObject,
											   LABEL_WordWrap, 72,
											   LABEL_Text, GetString(MSG_DATABASE_DESC),
											LabelEnd,
									End,
								End,
							End,
				TAG_DONE);
		}
		break;
		case 1:
		{
			IIntuition->SetGadgetAttrs(GAD(OBJ_WIN_WIZ_SPLASH), tempWin, NULL,
				LAYOUT_ModifyChild, OBJ(OBJ_WIN_WIZ_WIZARD),
					CHILD_ReplaceObject, tempSTEP1 = VLayoutObject,
							LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_AddChild,    VLayoutObject,
										LAYOUT_AddImage, LabelObject,
										   LABEL_WordWrap, 72,
										   LABEL_Text, GetString(MSG_WIZARD_STEP2_DESC),
										LabelEnd,
								End,
								CHILD_WeightedHeight,  9,
								LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,  BVS_GROUP,
								LAYOUT_Label,		GetString(MSG_INSERT_DSN_NAME),
									LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_DSNNAME) = (Object *) StringObject ,
										GA_ID,              OBJ_WIN_WIZ_DSNNAME,
										GA_RelVerify,       TRUE,
										GA_TabCycle,        TRUE,
										GA_HintInfo,		GetString(MSG_HINT_WIZARD_DSNNAME),
										STRINGA_MaxChars,   254,
										STRINGA_MinVisible, 13,
										STRINGA_TextVal,	DSNName==NULL ? "" : DSNName,
									End,
									CHILD_MaxWidth, 300,
									LAYOUT_AddChild,    VLayoutObject,
											LAYOUT_AddImage, LabelObject,
											   LABEL_WordWrap, 72,
											   LABEL_Text, GetString(MSG_DSN_NAME_DESC),
											LabelEnd,
									End,
								End,
								LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_Label,		GetString(MSG_INSERT_DSN_DESC),
									LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_DSNDESCRIPTION) = (Object *) StringObject ,
										GA_ID,              OBJ_WIN_WIZ_DSNDESCRIPTION,
										GA_RelVerify,       TRUE,
										GA_TabCycle,        TRUE,
										GA_HintInfo,		GetString(MSG_HINT_WIZARD_DSNDESCRIPTION),
										STRINGA_MaxChars,   254,
										STRINGA_MinVisible, 13,
										STRINGA_TextVal,	DSNDescription==NULL ? "" : DSNDescription,
									End,
									CHILD_MaxWidth, 300,
									LAYOUT_AddChild,    VLayoutObject,
											LAYOUT_AddImage, LabelObject,
											   LABEL_WordWrap, 72,
											   LABEL_Text, GetString(MSG_DSN_DESCRIPTION),
											LabelEnd,
									End,
								End,
							End,
				TAG_DONE);
		}
		break;
	}
	if (tempSTEP1)
		OBJ(OBJ_WIN_WIZ_WIZARD) = tempSTEP1;

	ILayout->RethinkLayout(GAD(OBJ_WIN_WIZ_SPLASH), tempWin, NULL, TRUE);
	ILayout->RethinkLayout(GAD(OBJ_WIN_WIZ_BUTTONS), tempWin, NULL, TRUE);
}

BOOL newDSNWizard(CONST_STRPTR Driver, STRPTR *ServerName, STRPTR *DatabaseName, STRPTR *DSNName, STRPTR *DSNDescription)
{
    Object *DSNwin;
    struct Window *DSNWindow;
    struct Screen *tempScreen;
	BOOL returnValue = FALSE;

	tempScreen = IIntuition->LockPubScreen("Workbench");

	DSNwin = (Object *)WindowObject,
			WA_ScreenTitle,        GetString(MSG_WIZARD_TITLE),
			WA_Title,              GetString(MSG_WIZARD_TITLE),
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
				LAYOUT_AddChild,       OBJ(OBJ_WIN_WIZ_NEWDSN) = (Object *)VLayoutObject,
					LAYOUT_BevelStyle,    BVS_GROUP,
					LAYOUT_Label,         GetString(MSG_WIZARD_WELCOME),
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
						LAYOUT_AddChild,  OBJ(OBJ_WIN_WIZ_SPLASH) = (Object *)VLayoutObject,
							LAYOUT_AddChild,  OBJ(OBJ_WIN_WIZ_WIZARD) = (Object *)VLayoutObject,
							LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_AddChild,    VLayoutObject,
										LAYOUT_AddImage, LabelObject,
										   LABEL_WordWrap, 72,
										   LABEL_Text, GetString(MSG_WIZARD_STEP1_DESC),
										LabelEnd,
								End,
								CHILD_WeightedHeight,  9,
								LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,  BVS_GROUP,
								LAYOUT_Label,		GetString(MSG_INSERT_SERVERNAME),
									LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_SERVERNAME) = (Object *) StringObject ,
										GA_ID,              OBJ_WIN_WIZ_SERVERNAME,
										GA_RelVerify,       TRUE,
										GA_TabCycle,        TRUE,
										GA_HintInfo,		GetString(MSG_HINT_WIZARD_SERVERNAME),
										STRINGA_MaxChars,   254,
										STRINGA_MinVisible, 13,
									End,
									CHILD_MaxWidth, 300,
									LAYOUT_AddChild,    VLayoutObject,
											LAYOUT_AddImage, LabelObject,
											   LABEL_WordWrap, 72,
											   LABEL_Text, GetString(MSG_SERVERNAME_DESC),
											LabelEnd,
									End,
								End,
								LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,    BVS_GROUP,
								LAYOUT_Label,		GetString(MSG_INSERT_DATABASE),
									LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_DATABASENAME) = (Object *) StringObject ,
										GA_ID,              OBJ_WIN_WIZ_DATABASENAME,
										GA_RelVerify,       TRUE,
										GA_TabCycle,        TRUE,
										GA_HintInfo,		GetString(MSG_HINT_WIZARD_DATABASENAME),
										STRINGA_MaxChars,   254,
										STRINGA_MinVisible, 13,
									End,
									CHILD_MaxWidth, 300,
									LAYOUT_AddChild,    VLayoutObject,
											LAYOUT_AddImage, LabelObject,
											   LABEL_WordWrap, 72,
											   LABEL_Text, GetString(MSG_DATABASE_DESC),
											LabelEnd,
									End,
								End,
							End,
							CHILD_WeightedWidth, 91,
						End, //VLayoutObject
					End, //HLayoutObject
				End,
				CHILD_WeightedHeight,  95,
				LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_BUTTONS) = (Object *) HLayoutObject,
					SPACE,
					SPACE,
					SPACE,
					LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_NEXT) = ButtonObject,
										GA_Text,  		   GetString(MSG_BUTTON_NEXT),
										GA_ID,             OBJ_WIN_WIZ_NEXT,
										GA_RelVerify, 	   TRUE,
					ButtonEnd,
					CHILD_WeightedHeight,  0,
					LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_FINISH) = ButtonObject,
										GA_Text, 		   GetString(MSG_BUTTON_FINISH),
										GA_ID,             OBJ_WIN_WIZ_FINISH,
										GA_RelVerify, 	   TRUE,
					ButtonEnd,
					CHILD_WeightedHeight,  0,
					LAYOUT_AddChild, OBJ(OBJ_WIN_WIZ_CANCEL) = ButtonObject,
										GA_Text, 		   GetString(MSG_BUTTON_CANCEL),
										GA_ID,             OBJ_WIN_WIZ_CANCEL,
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

		IIntuition->GetAttr(WINDOW_SigMask, DSNwin, &sigmask);
		while (!done)
		{
				siggot = IExec->Wait(sigmask | SIGBREAKF_CTRL_C);
				if (siggot & SIGBREAKF_CTRL_C) done = TRUE;
				while ((result = IIntuition->IDoMethod(DSNwin, WM_HANDLEINPUT, &code)))
				{
					ULONG _DatabaseName;
					ULONG _ServerName;
					ULONG _DSNName;
					ULONG _DSNDescription;

					switch(result & WMHI_CLASSMASK)
					{
						case WMHI_CLOSEWINDOW:
							done = TRUE;
						break;
						case WMHI_GADGETUP:
							switch (result & WMHI_GADGETMASK)
							{
								case OBJ_WIN_WIZ_CANCEL:
								{
									done = TRUE;
									returnValue = FALSE;
								}
								break;
								case OBJ_WIN_WIZ_NEXT:
								{
									switch (StepNum)
									{
										case 0:
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_SERVERNAME), &_ServerName);
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_DATABASENAME), &_DatabaseName);

											if (strcmp((const char*)_ServerName,"")!=0)
											{
												if (*ServerName) IExec->FreeVec(*ServerName);
												*ServerName = (STRPTR) IExec->AllocVecTags(strlen((const char*)_ServerName)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*ServerName, (const char*)_ServerName, strlen((const char*)_ServerName));
											}
											if (strcmp((const char*)_ServerName,"")!=0)
											{
												if (*DatabaseName) IExec->FreeVec(*DatabaseName);
												*DatabaseName = (STRPTR) IExec->AllocVecTags(strlen((const char*)_DatabaseName)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*DatabaseName, (const char*)_DatabaseName, strlen((const char*)_DatabaseName));
											}
										break;
										case 1:
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_DSNNAME), &_DSNName);
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_DSNDESCRIPTION), &_DSNDescription);

											if (strcmp((const char*)_DSNName,"")!=0)
											{
												if (*DSNName) IExec->FreeVec(*DSNName);
												*DSNName = (STRPTR) IExec->AllocVecTags(strlen((const char*)_DSNName)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*DSNName, (const char*)_DSNName, strlen((const char*)_DSNName));
											}
											if (strcmp((const char*)_DSNDescription,"")!=0)
											{
												if (*DSNDescription) IExec->FreeVec(*DSNDescription);
												*DSNDescription = (STRPTR)IExec->AllocVecTags(strlen((const char*)_DSNDescription)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*DSNDescription, (const char*)_DSNDescription, strlen((const char*)_DSNDescription));
											}
										break;
									}
									StepNum++;
									if (StepNum>1) StepNum=0;
									ChangeStep(DSNWindow, *ServerName, *DatabaseName, *DSNName, *DSNDescription);
								}
								break;
								case OBJ_WIN_WIZ_FINISH:
								{
									switch (StepNum)
									{
										case 0:
										{
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_SERVERNAME), &_ServerName);
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_DATABASENAME), &_DatabaseName);
											if (strcmp((const char*)_ServerName,"")!=0)
											{
												if (*ServerName) IExec->FreeVec(*ServerName);
												*ServerName = (STRPTR)IExec->AllocVecTags(strlen((const char*)_ServerName)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*ServerName, (const char*)_ServerName, strlen((const char*)_ServerName));
											}
											if (strcmp((const char*)_ServerName,"")!=0)
											{
												if (*DatabaseName) IExec->FreeVec(*DatabaseName);
												*DatabaseName = (STRPTR)IExec->AllocVecTags(strlen((const char*)_DatabaseName)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*DatabaseName, (const char*)_DatabaseName, strlen((const char*)_DatabaseName));
											}
										}
										break;
										case 1:
										{
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_DSNNAME), &_DSNName);
											IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_WIN_WIZ_DSNDESCRIPTION), &_DSNDescription);
											if (strcmp((const char*)_DSNName,"")!=0)
											{
												if (*DSNName) IExec->FreeVec(*DSNName);
												*DSNName = (STRPTR)IExec->AllocVecTags(strlen((const char*)_DSNName)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*DSNName, (const char*)_DSNName, strlen((const char*)_DSNName));
											}
											if (strcmp((const char*)_DSNDescription,"")!=0)
											{
												if (*DSNDescription) IExec->FreeVec(*DSNDescription);
												*DSNDescription = (STRPTR)IExec->AllocVecTags(strlen((const char*)_DSNDescription)+1, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE);
												strncpy(*DSNDescription, (const char*)_DSNDescription, strlen((const char*)_DSNDescription));
											}
										}
										break;
									}
                                    if ((strcmp((const char*)_ServerName,"")==0) && (strcmp((const char*)_DatabaseName,"")==0))
                                    {
									}
									done = TRUE;
									returnValue = TRUE;
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
		return FALSE;
	}

	IIntuition->UnlockPubScreen(NULL, tempScreen);

	/* Return the selected driver */
	return returnValue;
}
