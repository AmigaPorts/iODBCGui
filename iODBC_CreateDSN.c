#define ALL_REACTION_MACROS
#include <reaction/reaction_macros.h>
#include <reaction/reaction_prefs.h>

#include <dos/dos.h>
#include <intuition/intuition.h>
#include <classes/window.h>
#include <gadgets/layout.h>
#include <proto/listbrowser.h>
#include <gadgets/string.h>
#include <images/label.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/space.h>
#include <proto/label.h>
#include <proto/bitmap.h>
#include <images/bitmap.h>
#include <proto/string.h>

#include <classes/requester.h>

#include <stdio.h>
#include <stdlib.h>

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <iodbcext.h>
#include <odbcinst.h>
#include <dlf.h>

#include "iODBC_cat.h"
#include "version.h"
#include "debug.h"

extern BOOL checkKey(struct List *list, CONST_STRPTR key);
extern void addRow2(struct List *list, CONST_STRPTR label1, CONST_STRPTR label2);
extern int32 Requester( uint32 img, TEXT *title, TEXT *reqtxt, TEXT *buttons, struct Window* win);
extern int ODBC_Connect(void);
extern int ODBC_Disconnect(void);
extern char *GetString(LONG id);

#define OBJ(x) CObjects[x]
#define GAD(x) (struct Gadget *)CObjects[x]
#define SPACE    LAYOUT_AddChild, SpaceObject, End

struct ColumnInfo DSNkeys[] =
{
    { 200, "Keyword", CIF_DRAGGABLE },
    { 200, "Value",   CIF_DRAGGABLE },
    { -1, (STRPTR)~0, -1 }
};

enum
{
    OBJ_NEWDSN_WIN,
    OBJ_NEWDSN_DSNNAME,
    OBJ_NEWDSN_DSNDESCRIPTION,
    OBJ_NEWDSN_LB_KEYWORDS,
    OBJ_NEWDSN_KEYWORD,
    OBJ_NEWDSN_ADD_KEY,
    OBJ_NEWDSN_VALUE,
    OBJ_NEWDSN_UPDATE_KEY,
    OBJ_NEWDSN_DELETE_KEY,
    OBJ_NEWDSN_OK,
    OBJ_NEWDSN_CANCEL,
    OBJ_NEWDSN_NUM
};

Object *CObjects[OBJ_NEWDSN_NUM];
struct List    KeywordsList;

/* Function used to localize all key pairs */
static void Localize( void )
{
	DSNkeys[0].ci_Title = GetString(MSG_CI_KEYS_0);
	DSNkeys[1].ci_Title = GetString(MSG_CI_KEYS_1);
}

int createDSN(CONST_STRPTR driver, STRPTR ServerName, STRPTR DatabaseName, STRPTR DSNName, STRPTR DSNDescription)
{
    Object        *newDSNwin;
    struct Window *newDSNWindow;
    char           winTitle[128] = {0};
    BOOL           retcode = FALSE;

	if (ODBC_Connect()==0)
	{
		Localize();

		snprintf(winTitle, 128, GetString(MSG_DSN_SETUP), driver);

		newDSNwin = (Object *)WindowObject,
				WA_ScreenTitle,        winTitle,
				WA_Title,              winTitle,
				WA_DragBar,            TRUE,
				WA_CloseGadget,        TRUE,
				WA_SizeGadget,         FALSE,
				WA_DepthGadget,        TRUE,
				WA_Activate,           TRUE,
				WA_InnerWidth,         300,
				WA_InnerHeight,        400,
				WINDOW_GadgetHelp,	   TRUE,
				WINDOW_IconifyGadget,  FALSE,
				WINDOW_Position,       WPOS_CENTERWINDOW,
				WINDOW_Layout,         VLayoutObject,
					LAYOUT_AddChild,       OBJ(OBJ_NEWDSN_WIN) = (Object *) VLayoutObject,
						LAYOUT_BevelStyle,    BVS_NONE,
						LAYOUT_Label,         GetString(MSG_GENERIC_SETUP),
						LAYOUT_SpaceInner,    FALSE,
						LAYOUT_AddChild, HLayoutObject,
							LAYOUT_AddChild,    VLayoutObject,
								LAYOUT_BevelStyle,    BVS_NONE,
								LAYOUT_AddChild, OBJ(OBJ_NEWDSN_DSNNAME) = (Object *) StringObject ,
									GA_ID,              OBJ_NEWDSN_DSNNAME,
									GA_RelVerify,       TRUE,
									GA_TabCycle,        TRUE,
									GA_HintInfo,		GetString(MSG_HINT_NEWDSN_NAME),
									STRINGA_TextVal,	DSNName == NULL ? "" : DSNName,
									STRINGA_MaxChars,   254,
									STRINGA_MinVisible, 15,
								End,
								CHILD_Label, LabelObject, LABEL_Text, GetString(MSG_DATA_SOURCE_NAME), LabelEnd,
								CHILD_WeightedHeight,  10,
								LAYOUT_AddChild, OBJ(OBJ_NEWDSN_DSNDESCRIPTION) = (Object *) StringObject ,
									GA_ID,              OBJ_NEWDSN_DSNDESCRIPTION,
									GA_RelVerify,       TRUE,
									GA_TabCycle,        TRUE,
	                                GA_HintInfo,		GetString(MSG_HINT_NEWDSN_DESC),
									STRINGA_TextVal,	DSNDescription == NULL ? "" : DSNDescription,
									STRINGA_MaxChars,   254,
									STRINGA_MinVisible, 15,
								End,
								CHILD_Label, LabelObject, LABEL_Text, GetString(MSG_DESCRIPTION), LabelEnd,
								CHILD_WeightedHeight,  10,
								LAYOUT_AddChild, OBJ(OBJ_NEWDSN_LB_KEYWORDS) = ListBrowserObject,
									GA_ID, 			OBJ_NEWDSN_LB_KEYWORDS,
									GA_RelVerify, 	TRUE,
                                	GA_HintInfo,	GetString(MSG_HINT_NEWDSN_LISTVIEW),
									LISTBROWSER_ColumnInfo,       &DSNkeys,
									LISTBROWSER_ColumnTitles,     TRUE,
									LISTBROWSER_Labels,           NULL,
									LISTBROWSER_ShowSelected,     TRUE,
									LISTBROWSER_HorizontalProp,   TRUE,
									LISTBROWSER_Separators,       TRUE,
								End, //ListBrowserObject
								CHILD_WeightedHeight,  69,
								SPACE,
								CHILD_WeightedHeight,  1,
								LAYOUT_AddChild, VLayoutObject,
									LAYOUT_BevelStyle,    BVS_NONE,
									LAYOUT_AddChild, VLayoutObject,
										LAYOUT_BevelStyle,  BVS_NONE,
										LAYOUT_EvenSize, TRUE,
										LAYOUT_AddChild, OBJ(OBJ_NEWDSN_KEYWORD) = (Object *) StringObject ,
											GA_ID,              OBJ_NEWDSN_KEYWORD,
											GA_RelVerify,       TRUE,
											GA_TabCycle,        TRUE,
			                                GA_HintInfo,		GetString(MSG_HINT_NEWDSN_KEYWORD),
											STRINGA_MaxChars,   254,
											STRINGA_MinVisible, 13,
										End,
										CHILD_Label, LabelObject, LABEL_Text, GetString(MSG_KEYWORD), LabelEnd,
										LAYOUT_BevelStyle,    BVS_NONE,
										LAYOUT_EvenSize, TRUE,
										LAYOUT_AddChild, OBJ(OBJ_NEWDSN_VALUE) = (Object *) StringObject ,
											GA_ID,              OBJ_NEWDSN_VALUE,
											GA_RelVerify,       TRUE,
											GA_TabCycle,        TRUE,
			                                GA_HintInfo,		GetString(MSG_HINT_NEWDSN_VALUE),
											STRINGA_MaxChars,   254,
											STRINGA_MinVisible, 13,
										End,
										CHILD_Label, LabelObject, LABEL_Text, GetString(MSG_VALUE), LabelEnd,
									End,
										LAYOUT_AddChild, HLayoutObject,
											LAYOUT_BevelStyle,    BVS_NONE,
											LAYOUT_AddChild, OBJ(OBJ_NEWDSN_ADD_KEY) = ButtonObject,
												GA_Text, GetString(MSG_BUTTON_ADD),
												GA_ID,             OBJ_NEWDSN_ADD_KEY,
												GA_RelVerify, 	   TRUE,
												GA_HintInfo,	   GetString(MSG_HINT_NEWDSN_ADD),
											ButtonEnd,
											CHILD_MaxWidth, 80,
											LAYOUT_AddChild, OBJ(OBJ_NEWDSN_DELETE_KEY) = ButtonObject,
												GA_Text, GetString(MSG_BUTTON_REMOVE),
												GA_ID,             OBJ_NEWDSN_DELETE_KEY,
												GA_RelVerify, 	   TRUE,
												GA_HintInfo,	   GetString(MSG_HINT_NEWDSN_REMOVE),
											ButtonEnd,
											CHILD_MaxWidth, 80,
											LAYOUT_AddChild, OBJ(OBJ_NEWDSN_UPDATE_KEY) = ButtonObject,
												GA_Text, GetString(MSG_BUTTON_UPDATE),
												GA_ID,             OBJ_NEWDSN_UPDATE_KEY,
												GA_RelVerify, 	   TRUE,
												GA_HintInfo,	   GetString(MSG_HINT_NEWDSN_UPDATE),
											ButtonEnd,
											CHILD_MaxWidth, 80,
											SPACE,
											SPACE,
										End,
									SPACE,
								End,
								CHILD_WeightedHeight,  10,
						  End,
						End, //HLayoutObject
					End,
					CHILD_WeightedHeight,  95,
					LAYOUT_AddChild,       HLayoutObject,
						SPACE,
						SPACE,
						SPACE,
						LAYOUT_AddChild,       Button(GetString(MSG_BUTTON_FINISH), OBJ_NEWDSN_OK),
						CHILD_WeightedHeight,  0,
						LAYOUT_AddChild,       Button(GetString(MSG_BUTTON_CANCEL), OBJ_NEWDSN_CANCEL),
						CHILD_WeightedHeight,  0,
					End,
					CHILD_WeightedHeight,  5,
				End,       //HLayoutObject
			WindowEnd;     //WindowObject

		if ((newDSNWindow = (struct Window*)IIntuition->IDoMethod(newDSNwin, WM_OPEN)))
		{
			uint32
				sigmask     = 0,
				siggot      = 0,
				result      = 0;
			uint16
				code        = 0;
			BOOL
				done        = FALSE;


			IExec->NewList(&KeywordsList);

			if (ServerName != NULL)
				addRow2(&KeywordsList, "Server", ServerName);
			if (DatabaseName != NULL)
				addRow2(&KeywordsList, "Database", DatabaseName);

			IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, &KeywordsList, TAG_DONE);

			/* Disable Update and Delete Buttons */
			IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_UPDATE_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);
			IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_DELETE_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);

			IIntuition->GetAttr(WINDOW_SigMask, newDSNwin, &sigmask);
			while (!done)
			{
					siggot = IExec->Wait(sigmask | SIGBREAKF_CTRL_C);
					if (siggot & SIGBREAKF_CTRL_C) done = TRUE;
					while ((result = IIntuition->IDoMethod(newDSNwin, WM_HANDLEINPUT, &code)))
					{
						switch(result & WMHI_CLASSMASK)
						{
							case WMHI_CLOSEWINDOW:
								done = TRUE;
							break;
							case WMHI_GADGETUP:
								switch (result & WMHI_GADGETMASK)
								{
									case OBJ_NEWDSN_LB_KEYWORDS:
									{
										struct Node *selectedNode = NULL;
										IIntuition->GetAttrs(OBJ(OBJ_NEWDSN_LB_KEYWORDS), LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
										if (NULL!=selectedNode)
										{
											STRPTR tKEY, tVALUE;
											IListBrowser->GetListBrowserNodeAttrs(selectedNode, LBNA_Column, 0, LBNCA_Text, &tKEY, TAG_DONE);
											IListBrowser->GetListBrowserNodeAttrs(selectedNode, LBNA_Column, 1, LBNCA_Text, &tVALUE, TAG_DONE);
											IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_KEYWORD), newDSNWindow, NULL, STRINGA_TextVal, tKEY, TAG_DONE);
											IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_VALUE), newDSNWindow, NULL, STRINGA_TextVal, tVALUE, TAG_DONE);
											IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_UPDATE_KEY), newDSNWindow, NULL, GA_Disabled, FALSE, TAG_DONE);
											IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_DELETE_KEY), newDSNWindow, NULL, GA_Disabled, FALSE, TAG_DONE);
											IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_ADD_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);
											D(bug("KEY=%s - VALUE=%s\n",tKEY,tVALUE));
										}
									}
									break;
									case OBJ_NEWDSN_CANCEL:
										done = TRUE;
									break;
									case OBJ_NEWDSN_OK:
									{
										ULONG DSNNAME;
										ULONG DSNDESC;
										char dsnread[4096];

										IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_NEWDSN_DSNNAME), &DSNNAME);
										IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_NEWDSN_DSNDESCRIPTION), &DSNDESC);
										if (strcmp((const char*)DSNNAME,"")!=0)
										{
											struct Node *n = IExec->GetHead(&KeywordsList);

											if (!n || (checkKey(&KeywordsList,"Server")==0) || (checkKey(&KeywordsList,"Database")==0))
												Requester(REQIMAGE_WARNING, PROGRAM_TITLE, GetString(MSG_WARN_PARAMETERS),GetString(MSG_OK), newDSNWindow);

											struct Node *o = NULL;
											STRPTR NodeKey, NodeValue;
											D(bug("driver=%s\n",driver));
											if (SQLGetPrivateProfileString("ODBC Data Sources", (const char*)DSNNAME, "", (char *) dsnread, sizeof(dsnread), NULL)!=0)
											{
												if (Requester(REQIMAGE_QUESTION, PROGRAM_TITLE, GetString(MSG_WARN_OVERWRITE_DSN),GetString(MSG_YESNO), newDSNWindow)!=1)
												{
													done = TRUE;
													goto quit;
												}
											}
											D(bug("driver=%s\n",driver));
											SQLSetConfigMode(ODBC_SYSTEM_DSN);
											if (!SQLWriteDSNToIni((const char*)DSNNAME,driver))
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_WRITE_DSN),GetString(MSG_OK), newDSNWindow);
												D(bug("error on SQLWriteDSNToIni\n"));
												goto quit;
											}

											if (!SQLWritePrivateProfileString((const char*)DSNNAME, "Description", (const char*)DSNDESC, "odbc.ini"))
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_WRITE_DSN),GetString(MSG_OK), newDSNWindow);
												D(bug("error on SQLWritePrivateProfileString\n"));
												goto quit;
											}

											while (n)
											{
												IListBrowser->GetListBrowserNodeAttrs(n,
																					  LBNA_Column, 0, LBNCA_Text, &NodeKey,
																					  LBNA_Column, 1, LBNCA_Text, &NodeValue,
																				  TAG_DONE);

												if (!SQLWritePrivateProfileString((const char*)DSNNAME, NodeKey, NodeValue,  "odbc.ini"))
												{
													Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_WRITE_DSN),GetString(MSG_OK), newDSNWindow);
													D(bug("error on SQLWritePrivateProfileString\n"));
													goto quit;
												}

												o = n;
												n = IExec->GetSucc(n);
											}
											retcode = TRUE;
											done = TRUE;
										}
										else
											Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_MISSING_NAME),GetString(MSG_OK), newDSNWindow);
									}
									break;
									case OBJ_NEWDSN_ADD_KEY:
									{
										ULONG KEY;
										ULONG VALUE;

										IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_NEWDSN_KEYWORD), &KEY);
										IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_NEWDSN_VALUE), &VALUE);
										if ((strcmp((const char*)KEY,"")!=0) && (strcmp((const char*)VALUE,"")!=0))
										{
											if (strcasecmp((const char*)KEY, "DSN")==0)
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_INVALID_PARAMETER),GetString(MSG_OK), newDSNWindow);
												break;
											}

											if (strcasecmp((const char*)KEY, "Driver")==0)
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_INVALID_PARAMETER),GetString(MSG_OK), newDSNWindow);
												break;
											}

											if (strcasecmp((const char*)KEY, "Description")==0)
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_INVALID_PARAMETER),GetString(MSG_OK), newDSNWindow);
												break;
											}

											if (!checkKey(&KeywordsList, (CONST_STRPTR) KEY))
											{
												IIntuition->SetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, ~0, TAG_DONE);

												addRow2(&KeywordsList, (const char*)KEY, (const char *)VALUE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, &KeywordsList, TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_KEYWORD), newDSNWindow, NULL, STRINGA_TextVal, "", TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_VALUE), newDSNWindow, NULL, STRINGA_TextVal, "", TAG_DONE);
											}
											else
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_PARAMETER_ALREADY_EXISTS),GetString(MSG_OK), newDSNWindow);
										}
										else
											Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_KEY_PAIRS),GetString(MSG_OK), newDSNWindow);
									}
									break;
									case OBJ_NEWDSN_UPDATE_KEY:
									{
										ULONG KEY;
										ULONG VALUE;

										IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_NEWDSN_KEYWORD), &KEY);
										IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_NEWDSN_VALUE), &VALUE);
										if ((strcmp((const char*)KEY,"")!=0) && (strcmp((const char*)VALUE,"")!=0))
										{
											if (strcasecmp((const char*)KEY, "DSN")==0)
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_INVALID_PARAMETER),GetString(MSG_OK), newDSNWindow);
												break;
											}

											if (strcasecmp((const char*)KEY, "Driver")==0)
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_INVALID_PARAMETER),GetString(MSG_OK), newDSNWindow);
												break;
											}

											if (strcasecmp((const char*)KEY, "Description")==0)
											{
												Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_INVALID_PARAMETER),GetString(MSG_OK), newDSNWindow);
												break;
											}

											struct Node *selectedNode = NULL;
											IIntuition->GetAttrs(OBJ(OBJ_NEWDSN_LB_KEYWORDS), LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
											if (NULL!=selectedNode)
											{
												IIntuition->SetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, ~0, TAG_DONE);
												IListBrowser->SetListBrowserNodeAttrs(selectedNode, LBNA_Column, 0, LBNCA_Text, (const char*)KEY, TAG_DONE);
												IListBrowser->SetListBrowserNodeAttrs(selectedNode, LBNA_Column, 1, LBNCA_Text, (const char*)VALUE, TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, &KeywordsList, TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_KEYWORD), newDSNWindow, NULL, STRINGA_TextVal, "", TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_VALUE), newDSNWindow, NULL, STRINGA_TextVal, "", TAG_DONE);
												IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_UPDATE_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);
												IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_DELETE_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);
												IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_ADD_KEY), newDSNWindow, NULL, GA_Disabled, FALSE, TAG_DONE);
											}
										}
										else
											Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_KEY_PAIRS),GetString(MSG_OK), newDSNWindow);
									}
									break;
									case OBJ_NEWDSN_DELETE_KEY:
									{
										if (Requester(REQIMAGE_QUESTION, PROGRAM_TITLE,GetString(MSG_WARN_REMOVE_KEY),GetString(MSG_YESNO), newDSNWindow)==1)
										{
											struct Node *selectedNode = NULL;
											IIntuition->GetAttrs(OBJ(OBJ_NEWDSN_LB_KEYWORDS), LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
											if (NULL!=selectedNode)
											{
												IIntuition->SetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, ~0, TAG_DONE);
												IExec->Remove(selectedNode);
												IListBrowser->FreeListBrowserNode(selectedNode);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_LB_KEYWORDS), newDSNWindow, NULL, LISTBROWSER_Labels, &KeywordsList, TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_KEYWORD), newDSNWindow, NULL, STRINGA_TextVal, "", TAG_DONE);
												IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_NEWDSN_VALUE), newDSNWindow, NULL, STRINGA_TextVal, "", TAG_DONE);
												IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_UPDATE_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);
												IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_DELETE_KEY), newDSNWindow, NULL, GA_Disabled, TRUE, TAG_DONE);
												IIntuition->SetGadgetAttrs((struct Gadget *)OBJ(OBJ_NEWDSN_ADD_KEY), newDSNWindow, NULL, GA_Disabled, FALSE, TAG_DONE);
											}
										}
									}
									break;
								}
							break;
						}
					}
			}
quit:
			IIntuition->DisposeObject(newDSNwin);
			IListBrowser->FreeListBrowserList(&KeywordsList);
		}
		else
		{
			ODBC_Disconnect();
			return retcode;
		}

		ODBC_Disconnect();
		return retcode;
	}
	else
	{
		Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADING_ODBC),GetString(MSG_OK), NULL);
		return retcode;
	}
}

