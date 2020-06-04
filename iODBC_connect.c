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

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <iodbcext.h>
#include <odbcinst.h>
#include <dlf.h>

#include <stdio.h>
#include <stdlib.h>

#include "iODBC_cat.h"
#include "version.h"
#include "debug.h"

#define OBJ(x) TObjects[x]
#define GAD(x) (struct Gadget *)TObjects[x]
#define SPACE    LAYOUT_AddChild, SpaceObject, End

extern void DriverError(struct Window *win, HENV henv, HDBC hdbc,HSTMT hstmt);
extern int32 Requester(uint32 img, TEXT *title, TEXT *reqtxt, TEXT *buttons, struct Window* win);
extern char *GetString(LONG id);
extern int ODBC_Connect(void);
extern int ODBC_Disconnect(void);

extern HDBC  hdbc;
extern HENV  henv;
extern HSTMT hstmt;

enum
{
    OBJ_TESTDSN_WIN,
    OBJ_TESTDSN_UID,
    OBJ_TESTDSN_PWD,
    OBJ_TESTDSN_OK,
    OBJ_TESTDSN_CANCEL,
    OBJ_TESTDSN_NUM
};

Object *TObjects[OBJ_TESTDSN_NUM];

char *testDSN(CONST_STRPTR dsn)
{
	Object 			*testDSNwin 	= NULL;
	struct Window 	*testDSNWindow	= NULL;
	char 			 winTitle[255] 	= {0};
	char 			*returnvalue 	= NULL;
	SQLTCHAR 		 testDSN[255]	= {0};

	memset (testDSN,0x0,255);
	snprintf(winTitle, 254, GetString(MSG_LOGIN_TO_DSN),dsn);

    testDSNwin = (Object *)WindowObject,
            WA_ScreenTitle,        winTitle,
            WA_Title,              winTitle,
            WA_DragBar,            TRUE,
            WA_CloseGadget,        TRUE,
            WA_SizeGadget,         FALSE,
            WA_DepthGadget,        TRUE,
            WA_Activate,           TRUE,
            WA_InnerWidth,         180,
            WA_InnerHeight,        80,
            WINDOW_IconifyGadget,  FALSE,
            WINDOW_Position,       WPOS_CENTERWINDOW,
            WINDOW_Layout,         VLayoutObject,
                LAYOUT_AddChild,       OBJ(OBJ_TESTDSN_WIN) = (Object *) VLayoutObject,
                    LAYOUT_BevelStyle,    BVS_NONE,
                    LAYOUT_SpaceInner,    FALSE,
                    LAYOUT_AddChild, HLayoutObject,
                        LAYOUT_AddChild,    VLayoutObject,
                            LAYOUT_BevelStyle,    BVS_NONE,
                            LAYOUT_AddChild, OBJ(OBJ_TESTDSN_UID) = (Object *) StringObject ,
                                GA_ID,              OBJ_TESTDSN_UID,
                                GA_RelVerify,       FALSE,
                                GA_TabCycle,        TRUE,
                                STRINGA_MaxChars,   254,
                                STRINGA_MinVisible, 15,
                                STRINGA_TextVal,    "",
                            End,
                            CHILD_Label, LabelObject, LABEL_Text, GetString(MSG_USERNAME), LabelEnd,
                            CHILD_WeightedHeight,  10,
                            LAYOUT_AddChild, OBJ(OBJ_TESTDSN_PWD) = (Object *) StringObject ,
                                GA_ID,              OBJ_TESTDSN_PWD,
                                GA_RelVerify,       TRUE,
                                GA_TabCycle,        TRUE,
                                STRINGA_MaxChars,   254,
                                STRINGA_MinVisible, 15,
                                STRINGA_TextVal,    "",
                                STRINGA_HookType,   SHK_PASSWORD,
                            End,
                            CHILD_Label, LabelObject, LABEL_Text, GetString(MSG_PASSWORD), LabelEnd,
                            CHILD_WeightedHeight,  10,
                        End,
                    End, //HLayoutObject
                End,
                CHILD_WeightedHeight,  95,
                LAYOUT_AddChild,       HLayoutObject,
                    SPACE,
                    SPACE,
                    SPACE,
                    LAYOUT_AddChild,       Button(GetString(MSG_BUTTON_FINISH), OBJ_TESTDSN_OK),
                    CHILD_WeightedHeight,  0,
                    LAYOUT_AddChild,       Button(GetString(MSG_BUTTON_CANCEL), OBJ_TESTDSN_CANCEL),
                    CHILD_WeightedHeight,  0,
                End,
                CHILD_WeightedHeight,  5,
            End,       //HLayoutObject
        WindowEnd;     //WindowObject

    if ((testDSNWindow = (struct Window*) IIntuition->IDoMethod(testDSNwin, WM_OPEN)))
    {
        uint32
            sigmask     = 0,
            siggot      = 0,
            result      = 0;
        uint16
            code        = 0;
        BOOL
            done        = FALSE;

        IIntuition->GetAttr(WINDOW_SigMask, testDSNwin, &sigmask);
        while (!done)
        {
                siggot = IExec->Wait(sigmask | SIGBREAKF_CTRL_C);
                if (siggot & SIGBREAKF_CTRL_C) done = TRUE;
                while ((result = IIntuition->IDoMethod(testDSNwin, WM_HANDLEINPUT, &code)))
                {
                    switch(result & WMHI_CLASSMASK)
                    {
                        case WMHI_CLOSEWINDOW:
                            done = TRUE;
                        break;
                        case WMHI_GADGETUP:
                            switch (result & WMHI_GADGETMASK)
                            {
                                case OBJ_TESTDSN_CANCEL:
                                    done = TRUE;
                                break;
                                case OBJ_TESTDSN_OK:
                                {
                                    ULONG UID, PWD;
                                    int status;
                                    SQLTCHAR outDSN[4096] = {0};
                                    short buflen;

								    IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_TESTDSN_UID), &UID);
                                    IIntuition->GetAttr(STRINGA_TextVal, (Object *) OBJ(OBJ_TESTDSN_PWD), &PWD);
									snprintf((char *)testDSN, 254, "DSN=%s;UID=%s;PWD=%s",dsn,(const char*)UID,(const char*)PWD);
                                    D(bug("testDSN=%s\n",testDSN));

									if (ODBC_Connect()==0)
									{
										status = SQLDriverConnect(hdbc, testDSNWindow, (SQLCHAR *)testDSN, SQL_NTS,
																		   (SQLCHAR *)outDSN,  4096,
																		   &buflen, SQL_DRIVER_NOPROMPT);
										D(bug("status=%d\n",status));
										if (status != SQL_SUCCESS && status != SQL_SUCCESS_WITH_INFO)
										{
											DriverError(testDSNWindow, henv, hdbc, hstmt);
										}
										else
										{
											if (status==SQL_SUCCESS)
												Requester(REQIMAGE_INFO, PROGRAM_TITLE, GetString(MSG_CONNECTION_OK),GetString(MSG_OK), NULL);
											else
												Requester(REQIMAGE_INFO, PROGRAM_TITLE, GetString(MSG_CONNECTION_OK_WITH_INFO),GetString(MSG_OK), NULL);
										}
										ODBC_Disconnect();
									}
									else
										Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADING_ODBC),GetString(MSG_OK), NULL);

                                    done = TRUE;
                                }
                                break;
                            }
                        break;
                    }
                }
        }

        IIntuition->DisposeObject(testDSNwin);
    }
    else
        return returnvalue;

    return returnvalue;
}

