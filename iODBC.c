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

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/clicktab.h>
#include <proto/space.h>
#include <proto/label.h>
#include <proto/locale.h>

#include <classes/requester.h>

#include <stdio.h>
#include <stdlib.h>
#include <odbcinst.h>

#include "iODBC_cat.h"
#include "version.h"
#include "debug.h"

extern int listDSN(Object *LBDSN, struct Window *Win);
extern int listDrivers(Object *LBDRV, struct Window *win);
extern void addAboutList(Object *LBDRV, struct Window *win);
extern CONST_STRPTR newDSN(int *useWizard);
extern int createDSN(CONST_STRPTR driver, STRPTR ServerName, STRPTR DatabaseName, STRPTR DSNName, STRPTR DSNDescription);
extern int modifyDSN(CONST_STRPTR dsn);
extern int32 Requester(uint32 img, TEXT *title, TEXT *reqtxt, TEXT *buttons, struct Window* win);
extern int ODBC_Connect(void);
extern int ODBC_Disconnect(void);
extern char *testDSN(CONST_STRPTR dsn);
extern char *GetString(LONG id);
extern BOOL newDSNWizard(CONST_STRPTR Driver, STRPTR *ServerName, STRPTR *DatabaseName, STRPTR *DSNName, STRPTR *DSNDescription);

struct Catalog	*catalog = NULL;		/* The program catalog */
struct Locale   *locale  = NULL;		/* The locale pointer */

extern struct List DSNlist;
extern struct List DriverList;
extern struct List AboutList;

__attribute__ ((used)) static const char *version = PROGRAM_VERSION;
__attribute__ ((used)) static const char *sc = "$STACK: 512000";

#define OBJ(x) Objects[x]
#define GAD(x) (struct Gadget *)Objects[x]
#define SPACE    LAYOUT_AddChild, SpaceObject, End

Object *win;

struct MsgPort *AppPort;

struct ColumnInfo sysDSN[] =
{
    { 60, "Name",         CIF_DRAGGABLE },
    { 140, "Description", CIF_DRAGGABLE },
    { 200, "Driver",      CIF_DRAGGABLE },
    { -1, (STRPTR)~0, -1 }
};

struct ColumnInfo sysDrivers[] =
{
    { 100, "Name",   CIF_DRAGGABLE },
    { 200, "File",   CIF_DRAGGABLE },
    { 80, "Version", CIF_DRAGGABLE },
    { 60, "Size",    CIF_DRAGGABLE },
    { -1, (STRPTR)~0, -1 }
};

struct ColumnInfo about[] =
{
    { 150, "Name",    CIF_DRAGGABLE },
    { 100, "File",    CIF_DRAGGABLE },
    { 70,  "Date",    CIF_DRAGGABLE },
    { 70,  "Size",    CIF_DRAGGABLE },
    { 70,  "Version", CIF_DRAGGABLE },
    { -1, (STRPTR)~0, -1 }
};

enum
{
    OBJ_MAINLAYOUT,
    OBJ_SYSTEMDSN,
    OBJ_FILEDSN,
    OBJ_DRIVERS,
    OBJ_ABOUT,
    OBJ_OK,
    OBJ_CANCEL,
    OBJ_LB_SYSTEMDSN,
    OBJ_LB_SYSTEMDRIVERS,
    OBJ_LB_ABOUT,
    OBJ_SDSN_ADD,
    OBJ_SDSN_REMOVE,
    OBJ_SDSN_CONFIGURE,
    OBJ_SDSN_TEST,
    OBJ_NUM
};

Object *Objects[OBJ_NUM];

STRPTR PageLabels_1[] = {"System DSN", "ODBC Drivers", "About", NULL};

/* Function used to localize all tabled objects */
static void Localize( void )
{
	PageLabels_1[0] = GetString(MSG_TAB_SYSTEM_DSN);
	PageLabels_1[1] = GetString(MSG_TAB_ODBC_DRIVERS);
	PageLabels_1[2] = GetString(MSG_TAB_ABOUT);

	sysDSN[0].ci_Title = GetString(MSG_CI_SYSDSN_0);
	sysDSN[1].ci_Title = GetString(MSG_CI_SYSDSN_1);
	sysDSN[2].ci_Title = GetString(MSG_CI_SYSDSN_2);

	sysDrivers[0].ci_Title = GetString(MSG_CI_SYSDRIVERS_0);
	sysDrivers[1].ci_Title = GetString(MSG_CI_SYSDRIVERS_1);
	sysDrivers[2].ci_Title = GetString(MSG_CI_SYSDRIVERS_2);
	sysDrivers[3].ci_Title = GetString(MSG_CI_SYSDRIVERS_3);

	about[0].ci_Title = GetString(MSG_CI_ABOUT_0);
	about[1].ci_Title = GetString(MSG_CI_ABOUT_1);
	about[2].ci_Title = GetString(MSG_CI_ABOUT_2);
	about[3].ci_Title = GetString(MSG_CI_ABOUT_3);
	about[4].ci_Title = GetString(MSG_CI_ABOUT_4);
}

Object *
make_window(void)
{
    Object
            *page1 = NULL,
            *page2 = NULL,
            *page3 = NULL;

    page1 = (Object *) VLayoutObject,
        LAYOUT_BevelStyle,    BVS_GROUP,
        LAYOUT_Label,         GetString(MSG_SYSTEM_DATA_SOURCES),
        LAYOUT_SpaceInner,    FALSE,
        LAYOUT_AddChild, HLayoutObject,
                LAYOUT_AddChild, Objects[OBJ_LB_SYSTEMDSN] = ListBrowserObject,
                             GA_ID, 		OBJ_LB_SYSTEMDSN,
                             GA_RelVerify, 	TRUE,
							 GA_HintInfo,	GetString(MSG_HINT_AVAILABLEDSN),
                             LISTBROWSER_ColumnInfo,       &sysDSN,
                             LISTBROWSER_ColumnTitles,     TRUE,
                             LISTBROWSER_Labels,           NULL,
                             LISTBROWSER_ShowSelected,     TRUE,
                             LISTBROWSER_HorizontalProp,   TRUE,
                             LISTBROWSER_Separators,       TRUE,
                End, //ListBrowserObject
                CHILD_WeightedWidth, 89,

                SPACE,
                CHILD_WeightedWidth, 1,

                LAYOUT_AddChild, VLayoutObject,
                    LAYOUT_ShrinkWrap, TRUE,
                    LAYOUT_AddChild, OBJ(OBJ_SDSN_ADD) = ButtonObject,
                        				GA_Text, GetString(MSG_BUTTON_ADD),
										GA_ID,             OBJ_SDSN_ADD,
										GA_RelVerify, 	   TRUE,
										GA_HintInfo,	   GetString(MSG_HINT_ADD),
									 ButtonEnd,
	                CHILD_WeightedHeight,  0,
                    SPACE,
                    LAYOUT_AddChild, OBJ(OBJ_SDSN_REMOVE) = ButtonObject,
                        				GA_Text, GetString(MSG_BUTTON_REMOVE),
										GA_ID,             OBJ_SDSN_REMOVE,
										GA_RelVerify, 	   TRUE,
										GA_HintInfo,	   GetString(MSG_HINT_REMOVE),
 									 ButtonEnd,
                    CHILD_WeightedHeight,  0,
                    SPACE,
                    LAYOUT_AddChild, OBJ(OBJ_SDSN_CONFIGURE) = ButtonObject,
                        				GA_Text, GetString(MSG_BUTTON_CONFIGURE),
										GA_ID,             OBJ_SDSN_CONFIGURE,
										GA_RelVerify, 	   TRUE,
										GA_HintInfo,	   GetString(MSG_HINT_CONFIGURE),
 									 ButtonEnd,
                    CHILD_WeightedHeight,  0,
                    SPACE,
                    LAYOUT_AddChild, OBJ(OBJ_SDSN_TEST) = ButtonObject,
                        				GA_Text, GetString(MSG_BUTTON_TEST),
										GA_ID,             OBJ_SDSN_TEST,
										GA_RelVerify, 	   TRUE,
										GA_HintInfo,	   GetString(MSG_HINT_TEST),
 									 ButtonEnd,
                    CHILD_WeightedHeight,  0,
                    SPACE,
                    SPACE,
                    SPACE,
                End, //VLayoutObject
                CHILD_WeightedWidth, 10,
        End, //HLayoutObject
        CHILD_WeightedHeight,  80,
        SPACE,
        CHILD_WeightedHeight,  1,
        LAYOUT_AddChild,    HLayoutObject,
        LAYOUT_BevelStyle,    BVS_GROUP,
                LAYOUT_AddImage, LabelObject,
                   LABEL_WordWrap, 72,
                   LABEL_Text, GetString(MSG_SYSTEMDSN_LABEL),
                LabelEnd,
        End,
        CHILD_WeightedHeight,  9,
        End;

    page2 = (Object *) VLayoutObject,
        LAYOUT_BevelStyle,    BVS_GROUP,
        LAYOUT_Label,         GetString(MSG_ODBC_DRIVERS),
        LAYOUT_SpaceInner,    FALSE,
        LAYOUT_AddChild, HLayoutObject,
                LAYOUT_AddChild, Objects[OBJ_LB_SYSTEMDRIVERS] = ListBrowserObject,
                             GA_ID, OBJ_LB_SYSTEMDRIVERS,
                             GA_RelVerify, TRUE,
							 GA_HintInfo,	GetString(MSG_HINT_AVAILABLEDRIVERS),
                             LISTBROWSER_ColumnInfo,       &sysDrivers,
                             LISTBROWSER_ColumnTitles,     TRUE,
                             LISTBROWSER_Labels,           NULL,
                             LISTBROWSER_ShowSelected,     TRUE,
                             LISTBROWSER_HorizontalProp,   TRUE,
                             LISTBROWSER_Separators,       TRUE,
                End, //ListBrowserObject
                CHILD_WeightedWidth, 100,
        End, //HLayoutObject
        CHILD_WeightedHeight,  80,
        SPACE,
        CHILD_WeightedHeight,  1,
        LAYOUT_AddChild,    HLayoutObject,
        LAYOUT_BevelStyle,    BVS_GROUP,
                LAYOUT_AddImage, LabelObject,
                   LABEL_WordWrap, 72,
                   LABEL_Text, GetString(MSG_ODBCDRIVERS_LABEL),
                LabelEnd,
        End,
        CHILD_WeightedHeight,  9,
        End;

    page3 = (Object *) VLayoutObject,
        LAYOUT_BevelStyle,    BVS_GROUP,
        LAYOUT_Label,         GetString(MSG_ODBC_COMPONENTS),
        LAYOUT_SpaceInner,    FALSE,
        LAYOUT_AddChild, HLayoutObject,
                LAYOUT_AddChild, Objects[OBJ_LB_ABOUT] = ListBrowserObject,
                             GA_ID, OBJ_LB_ABOUT,
                             GA_RelVerify, TRUE,
							 GA_HintInfo,	GetString(MSG_HINT_AVAILABLECOMPONENTS),
                             LISTBROWSER_ColumnInfo,       &about,
                             LISTBROWSER_ColumnTitles,     TRUE,
                             LISTBROWSER_Labels,           NULL,
                             LISTBROWSER_ShowSelected,     TRUE,
                             LISTBROWSER_HorizontalProp,   TRUE,
                             LISTBROWSER_Separators,       TRUE,
                End, //ListBrowserObject
                CHILD_WeightedWidth, 100,
        End, //HLayoutObject
        CHILD_WeightedHeight,  80,
        SPACE,
        CHILD_WeightedHeight,  1,
        LAYOUT_AddChild,    HLayoutObject,
        LAYOUT_BevelStyle,    BVS_GROUP,
                LAYOUT_AddImage, LabelObject,
                   LABEL_WordWrap, 72,
                   LABEL_Text, GetString(MSG_ODBCCOMPONENTS_LABEL),
                LabelEnd,
        End,
        CHILD_WeightedHeight,  9,
        End;

    OBJ(OBJ_SYSTEMDSN) = IIntuition->NewObject(NULL, "clicktab.gadget",
        GA_Text,            PageLabels_1,
        CLICKTAB_Current,   0,
        CLICKTAB_PageGroup, IIntuition->NewObject(NULL, "page.gadget",
            PAGE_Add,       page1,
            PAGE_Add,       page2,
            PAGE_Add,       page3,
        TAG_DONE),
    TAG_DONE);

    return (Object *)WindowObject,
        WA_ScreenTitle,        GetString(MSG_WINDOW_TITLE),
        WA_Title,              GetString(MSG_WINDOW_TITLE),
        WA_DragBar,            TRUE,
        WA_CloseGadget,        TRUE,
        WA_SizeGadget,         FALSE,
        WA_DepthGadget,        TRUE,
        WA_Activate,           TRUE,
        WA_InnerWidth,         500,
        WA_InnerHeight,        300,
		WINDOW_GadgetHelp,	   TRUE,
        WINDOW_IconifyGadget,  TRUE,
        WINDOW_IconTitle,      "iODBC",
        WINDOW_AppPort,        AppPort,
        WINDOW_Position,       WPOS_CENTERSCREEN,
        WINDOW_Layout,         Objects[OBJ_MAINLAYOUT] = VLayoutObject,
            LAYOUT_AddChild,       OBJ(OBJ_SYSTEMDSN),
            CHILD_WeightedHeight,  95,
            LAYOUT_AddChild,       HLayoutObject,
                SPACE,
                SPACE,
                SPACE,
                LAYOUT_AddChild,       Button(GetString(MSG_BUTTON_OK), OBJ_OK),
                CHILD_WeightedHeight,  0,
                LAYOUT_AddChild,       Button(GetString(MSG_BUTTON_CANCEL), OBJ_CANCEL),
                CHILD_WeightedHeight,  0,
            End,
            CHILD_WeightedHeight,  5,
        End,       //HLayoutObject
    WindowEnd;     //WindowObject
}


int
main()
{
    struct Window *window;

    if (ODBC_Connect()!=0)
    {
        Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADDRIVERS),GetString(MSG_OK), NULL);
        exit(20);
    }

	/* we have all we need. Disconnect from ODBC */
	ODBC_Disconnect();

    if ((AppPort = IExec->AllocSysObjectTags(ASOT_PORT, TAG_DONE)))
    {
		catalog = ILocale->OpenCatalogA( NULL, "iODBC.catalog", NULL );
		locale  = ILocale->OpenLocale( NULL );

		Localize();

        win = make_window();
        if ((window = (struct Window*)IIntuition->IDoMethod(win, WM_OPEN)))
        {
            uint32
                sigmask     = 0,
                siggot      = 0,
                result      = 0;
            uint16
                code        = 0;
            BOOL
                done        = FALSE;

            /* Disable System DSN Buttons */
            IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_REMOVE], window, NULL, GA_Disabled, TRUE, TAG_DONE);
            IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_CONFIGURE], window, NULL, GA_Disabled, TRUE, TAG_DONE);
            IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_TEST], window, NULL, GA_Disabled, TRUE, TAG_DONE);

            listDSN(Objects[OBJ_LB_SYSTEMDSN], window);
            listDrivers(Objects[OBJ_LB_SYSTEMDRIVERS], window);
            addAboutList(Objects[OBJ_LB_ABOUT],window);

            IIntuition->GetAttr(WINDOW_SigMask, win, &sigmask);
            while (!done)
            {
                siggot = IExec->Wait(sigmask | SIGBREAKF_CTRL_C);
                if (siggot & SIGBREAKF_CTRL_C) done = TRUE;
                while ((result = IIntuition->IDoMethod(win, WM_HANDLEINPUT, &code)))
                {
                    switch(result & WMHI_CLASSMASK)
                    {
                        case WMHI_CLOSEWINDOW:
                            done = TRUE;
                            break;
                        case WMHI_GADGETUP:
                            switch (result & WMHI_GADGETMASK)
                            {
                                case OBJ_OK:
                                case OBJ_CANCEL:
                                    done=TRUE;
                                break;
                                case OBJ_LB_SYSTEMDSN:
                                {
                                    int32 selectedDSN = -1;
                                    IIntuition->GetAttrs(Objects[OBJ_LB_SYSTEMDSN], LISTBROWSER_Selected, &selectedDSN, TAG_DONE);
                                    if (selectedDSN>=0)
                                    {
                                        /* If a DSN is selected enable System DSN Buttons */
                                        IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_REMOVE], window, NULL, GA_Disabled, FALSE, TAG_DONE);
                                        IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_CONFIGURE], window, NULL, GA_Disabled, FALSE, TAG_DONE);
                                        IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_TEST], window, NULL, GA_Disabled, FALSE, TAG_DONE);
                                    }
                                    else
                                    {
                                        /* otherwise disable them */
                                        IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_REMOVE], window, NULL, GA_Disabled, TRUE, TAG_DONE);
                                        IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_CONFIGURE], window, NULL, GA_Disabled, TRUE, TAG_DONE);
                                        IIntuition->SetGadgetAttrs((struct Gadget *)Objects[OBJ_SDSN_TEST], window, NULL, GA_Disabled, TRUE, TAG_DONE);
                                    }
                                }
                                break;
                                case OBJ_SDSN_REMOVE:
                                {
									if (ODBC_Connect()==0)
									{
										struct Node *selectedNode = NULL;
										IIntuition->GetAttrs(Objects[OBJ_LB_SYSTEMDSN], LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
										if (NULL!=selectedNode)
										{
											if (Requester(REQIMAGE_QUESTION, PROGRAM_TITLE, GetString(MSG_REMOVE_DSN),GetString(MSG_YESNO), window)==1)
											{
												STRPTR DSNName;

												IListBrowser->GetListBrowserNodeAttrs(selectedNode, LBNCA_Text, &DSNName, TAG_DONE);
												if (!SQLRemoveDSNFromIni(DSNName))
												{
													Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_REMOVE_DSN),GetString(MSG_OK), NULL);
													D(bug("Error when removing %s DSN\n",DSNName));
												}
												else
												{
													IIntuition->RefreshSetGadgetAttrs((struct Gadget*) OBJ(OBJ_LB_SYSTEMDSN), window, NULL, LISTBROWSER_Labels, ~0, TAG_DONE);
													IListBrowser->FreeListBrowserList(&DSNlist);

													listDSN(Objects[OBJ_LB_SYSTEMDSN], window);
												}
											}
										}
										ODBC_Disconnect();
									}
									else
										Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADING_ODBC),GetString(MSG_OK), NULL);
                                }
                                break;
                                case OBJ_SDSN_TEST:
                                {
                                    struct Requester dummyRequester;
                                    char *resultValue = NULL;
                                    struct Node *selectedNode = NULL;

                                    IIntuition->GetAttrs(Objects[OBJ_LB_SYSTEMDSN], LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
                                    if (NULL!=selectedNode)
                                    {
										STRPTR DSNName = NULL;
                                        IListBrowser->GetListBrowserNodeAttrs(selectedNode, LBNCA_Text, &DSNName, TAG_DONE);

										if (DSNName)
										{
											IIntuition->InitRequester(&dummyRequester);
											IIntuition->Request(&dummyRequester, window);
											IIntuition->SetWindowPointer(window, WA_BusyPointer, TRUE, WA_PointerDelay, TRUE, TAG_DONE);
											resultValue = testDSN((const char*)DSNName);

											IIntuition->SetWindowPointer(window, WA_BusyPointer, FALSE, TAG_DONE);
											IIntuition->EndRequest(&dummyRequester, window);
										}
                                    }
                                }
                                break;
                                case OBJ_SDSN_ADD:
                                {
                                    struct Requester dummyRequester;
                                    CONST_STRPTR selectedDriver;
									int wizard = 0;

                                    IIntuition->InitRequester(&dummyRequester);
                                    IIntuition->Request(&dummyRequester, window);
                                    IIntuition->SetWindowPointer(window, WA_BusyPointer, TRUE, WA_PointerDelay, TRUE, TAG_DONE);
                                    selectedDriver = newDSN(&wizard);
                                    if (NULL != selectedDriver)
                                    {
										STRPTR _ServerName = NULL;
										STRPTR _DatabaseName = NULL;
										STRPTR _DSNName = NULL;
										STRPTR _DSNDescription = NULL;
										int DSNcreated = -1;

										if (wizard==1)
										{
											if (newDSNWizard(selectedDriver, &_ServerName, &_DatabaseName, &_DSNName, &_DSNDescription)==TRUE)
											{
												D(bug("ServerName=%s\n",_ServerName));
												D(bug("DatabaseName=%s\n",_DatabaseName));
												D(bug("DSNName=%s\n",_DSNName));
												D(bug("DSNDescription=%s\n",_DSNDescription));
	                                        	DSNcreated = createDSN(selectedDriver, _ServerName, _DatabaseName, _DSNName, _DSNDescription);
	                                        	if (_ServerName) IExec->FreeVec(_ServerName);
	                                        	if (_DatabaseName) IExec->FreeVec(_DatabaseName);
	                                        	if (_DSNName) IExec->FreeVec(_DSNName);
	                                        	if (_DSNDescription) IExec->FreeVec(_DSNDescription);
											}
										}
										else
										{
                                        	DSNcreated = createDSN(selectedDriver, NULL, NULL, NULL, NULL);
										}
                                    }
                                    IIntuition->SetWindowPointer(window, WA_BusyPointer, FALSE, TAG_DONE);
                                    IIntuition->EndRequest(&dummyRequester, window);

                                    IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_LB_SYSTEMDSN), window, NULL, LISTBROWSER_Labels, ~0, TAG_DONE);
                                    IListBrowser->FreeListBrowserList(&DSNlist);
                                    listDSN(Objects[OBJ_LB_SYSTEMDSN], window);
                                }
                                break;
                                case OBJ_SDSN_CONFIGURE:
                                {
                                    struct Node *selectedNode = NULL;
                                    IIntuition->GetAttrs(Objects[OBJ_LB_SYSTEMDSN], LISTBROWSER_SelectedNode, &selectedNode, TAG_DONE);
                                    if (NULL!=selectedNode)
                                    {
                                        struct Requester dummyRequester;
                                        int result = -1;
                                        STRPTR DSNName;

                                        IIntuition->InitRequester(&dummyRequester);
                                        IIntuition->Request(&dummyRequester, window);
                                        IIntuition->SetWindowPointer(window, WA_BusyPointer, TRUE, WA_PointerDelay, TRUE, TAG_DONE);
                                        IListBrowser->GetListBrowserNodeAttrs(selectedNode, LBNCA_Text, &DSNName, TAG_DONE);
                                        result = modifyDSN(DSNName);

                                        IIntuition->SetWindowPointer(window, WA_BusyPointer, FALSE, TAG_DONE);
                                        IIntuition->EndRequest(&dummyRequester, window);

                                        IIntuition->RefreshSetGadgetAttrs((struct Gadget*)OBJ(OBJ_LB_SYSTEMDSN), window, NULL, LISTBROWSER_Labels, ~0, TAG_DONE);
                                        IListBrowser->FreeListBrowserList(&DSNlist);
                                        listDSN(Objects[OBJ_LB_SYSTEMDSN], window);
                                    }
                                }
                            }
                            break;
                        case WMHI_ICONIFY:
                            if (IIntuition->IDoMethod(win, WM_ICONIFY)) window = NULL;
                            break;
                        case WMHI_UNICONIFY:
                            window = (struct Window*)IIntuition->IDoMethod(win, WM_OPEN);
                            break;
                    }
                }
            }
        }

		if (catalog)
		{
			ILocale->CloseCatalog( catalog );
			catalog = NULL;
		}

        IIntuition->DisposeObject(win);

        IExec->FreeSysObject(ASOT_PORT, AppPort);

        IListBrowser->FreeListBrowserList(&DSNlist);
        IListBrowser->FreeListBrowserList(&DriverList);
        IListBrowser->FreeListBrowserList(&AboutList);
    }

    return 0;
}
