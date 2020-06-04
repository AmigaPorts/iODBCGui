#include "iODBCGui.h"

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/listbrowser.h>

#include <string.h>
#include <sys/stat.h>
#include <classes/requester.h>

#include "iODBC_cat.h"
#include "version.h"
#include "debug.h"

extern int32 Requester(uint32 img, TEXT *title, TEXT *reqtxt, TEXT *buttons, struct Window* win);
extern char *GetString(LONG id);

BOOL checkKey(struct List *list, CONST_STRPTR key)
{
	BOOL found = FALSE;
	STRPTR NodeKey;

	struct Node *n = IExec->GetHead(list);
	while (n && !found)
	{
		IListBrowser->GetListBrowserNodeAttrs(n,
											  LBNA_Column, 0, LBNCA_Text, &NodeKey,
										  	  TAG_DONE);
		if (strcasecmp((const char*)NodeKey, key) == 0)
			found = TRUE;

		n = IExec->GetSucc(n);
	}
	return found;
}

void addRow2(struct List *list, CONST_STRPTR label1, CONST_STRPTR label2)
{
    if (NULL == list)
        return;

    struct Node *node = IListBrowser->AllocListBrowserNode(2,
                                        LBNA_Column, 0, LBNCA_CopyText, TRUE, LBNCA_Text, label1 == NULL ? "" : label1,
                                        LBNA_Column, 1, LBNCA_CopyText, TRUE, LBNCA_Text, label2 == NULL ? "" : label2,
                                        TAG_DONE);

    if (node)
        IExec->AddTail(list, node);
}

void addRow3(struct List *list, CONST_STRPTR label1, CONST_STRPTR label2, CONST_STRPTR label3)
{
    if (NULL == list)
        return;

    struct Node *node = IListBrowser->AllocListBrowserNode(3,
                                        LBNA_Column, 0, LBNCA_CopyText, TRUE, LBNCA_Text, label1 == NULL ? "" : label1,
                                        LBNA_Column, 1, LBNCA_CopyText, TRUE, LBNCA_Text, label2 == NULL ? "" : label2,
                                        LBNA_Column, 2, LBNCA_CopyText, TRUE, LBNCA_Text, label3 == NULL ? "" : label3,
                                        TAG_DONE);
    if (node)
        IExec->AddTail(list, node);
}

void addRow4(struct List *list, CONST_STRPTR label1, CONST_STRPTR label2, CONST_STRPTR label3, CONST_STRPTR label4)
{
    if (NULL == list)
        return;

    struct Node *node = IListBrowser->AllocListBrowserNode(4,
                                        LBNA_Column, 0, LBNCA_CopyText, TRUE, LBNCA_Text, label1 == NULL ? "" : label1,
                                        LBNA_Column, 1, LBNCA_CopyText, TRUE, LBNCA_Text, label2 == NULL ? "" : label2,
                                        LBNA_Column, 2, LBNCA_CopyText, TRUE, LBNCA_Text, label3 == NULL ? "" : label3,
                                        LBNA_Column, 3, LBNCA_CopyText, TRUE, LBNCA_Text, label4 == NULL ? "" : label4,
                                        TAG_DONE);
    if (node)
        IExec->AddTail(list, node);
}

void addRow5(struct List *list, CONST_STRPTR label1, CONST_STRPTR label2, CONST_STRPTR label3, CONST_STRPTR label4,  CONST_STRPTR label5)
{
    if (NULL == list)
        return;

    struct Node *node = IListBrowser->AllocListBrowserNode(5,
                                        LBNA_Column, 0, LBNCA_CopyText, TRUE, LBNCA_Text, label1 == NULL ? "" : label1,
                                        LBNA_Column, 1, LBNCA_CopyText, TRUE, LBNCA_Text, label2 == NULL ? "" : label2,
                                        LBNA_Column, 2, LBNCA_CopyText, TRUE, LBNCA_Text, label3 == NULL ? "" : label3,
                                        LBNA_Column, 3, LBNCA_CopyText, TRUE, LBNCA_Text, label4 == NULL ? "" : label4,
                                        LBNA_Column, 4, LBNCA_CopyText, TRUE, LBNCA_Text, label5 == NULL ? "" : label5,
                                        TAG_DONE);
    if (node)
        IExec->AddTail(list, node);
}


int ODBC_Connect(void)
{
    if (SQLAllocHandle (SQL_HANDLE_ENV, NULL, &henv) != SQL_SUCCESS)
        return -1;

    SQLSetEnvAttr (henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);

    if (SQLAllocHandle (SQL_HANDLE_DBC, henv, &hdbc) != SQL_SUCCESS)
        return -1;

    SQLSetConnectOption (hdbc, SQL_APPLICATION_NAME, (SQLULEN) TEXT ("iODBC Source Administrator"));

    return 0;
}

int ODBC_Disconnect(void)
{
    if (hstmt)
    {
        SQLCloseCursor (hstmt);
        SQLFreeHandle (SQL_HANDLE_STMT, hstmt);
    }

    if (connected)
        SQLDisconnect (hdbc);

    if (hdbc)
        SQLFreeHandle (SQL_HANDLE_DBC, hdbc);

    if (henv)
        SQLFreeHandle (SQL_HANDLE_ENV, henv);

    return 0;
}

int listDSN(Object *LBDSN, struct Window *win)
{
	if (ODBC_Connect()==0)
	{
		SQLTCHAR dsn[33]      = {0};
		SQLTCHAR desc[255]    = {0};
		SQLTCHAR driver[1024] = {0};
		SQLSMALLINT len1, len2;
		int retcode = -1;

		IExec->NewList(&DSNlist);

		if (SQLDataSources (henv, SQL_FETCH_FIRST, dsn, NUMTCHAR (dsn), &len1, desc, NUMTCHAR (desc), &len2) == SQL_SUCCESS)
		{
			do
			{
				char *tempDSN = (char*)calloc(1,len1+1);
				char *tempDriver = (char*)calloc(1,1024);
				char tempDes[4096] = { 0 };

				strncpy(tempDSN, (const char *) dsn, len1);

				SQLSetConfigMode(ODBC_SYSTEM_DSN);
				SQLGetPrivateProfileString((const char *) dsn, "Driver", "", (char *) driver, sizeof(driver), "odbc.ini");
				SQLGetPrivateProfileString((const char *) dsn, "Description", "", (char *) tempDes, sizeof(tempDes), "odbc.ini");

				if (driver!=NULL)
					strncpy(tempDriver, (const char *) driver, 1023);
				else
					strcpy(tempDriver, "-");

				addRow3(&DSNlist,(CONST_STRPTR) tempDSN, tempDes, tempDriver);

			}
			while (SQLDataSources (henv, SQL_FETCH_NEXT, dsn, NUMTCHAR (dsn), &len1, desc, NUMTCHAR (desc), &len2) == SQL_SUCCESS);
			retcode = 0;
		}

		IIntuition->RefreshSetGadgetAttrs((struct Gadget*)LBDSN, win, NULL, LISTBROWSER_Labels, &DSNlist, TAG_DONE);
		return retcode;
	}
	else
	{
		Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADING_ODBC),GetString(MSG_OK), NULL);
		return -1;
	}
}

int listDrivers(Object *LBDRV, struct Window *win)
{
	if (ODBC_Connect()==0)
	{
		SQLTCHAR drvdesc[1024] = {0};
		SQLTCHAR drvattr[1024] = {0};
		SQLTCHAR drvattrs[1024] = {0};
		SQLTCHAR driver[1024] = {0};
		SQLSMALLINT len, len1, len2;
		void *handle = NULL;
		HENV drv_henv;
		HDBC drv_hdbc;
		SQLRETURN ret;
		pSQLGetInfoFunc funcHdl;
		pSQLAllocHandle allocHdl;
		pSQLAllocEnv allocEnvHdl = NULL;
		pSQLAllocConnect allocConnectHdl = NULL;
		pSQLFreeHandle freeHdl;
		pSQLFreeEnv freeEnvHdl;
		pSQLFreeConnect freeConnectHdl;
		int retcode = -1;

		IExec->NewList(&DriverList);

		if (SQLDrivers(henv, SQL_FETCH_FIRST, drvdesc, sizeof(drvdesc), &len1, drvattr, sizeof(drvattr), &len2) != SQL_SUCCESS)
		{
			Requester(REQIMAGE_ERROR, GetString(MSG_WINDOW_TITLE), GetString(MSG_ERROR_NODRIVERS_INSTALLED),GetString(MSG_OK), NULL);
			return retcode;
		}
		else
		{
			do
			{
				char *tempDesc   = (char*)calloc(1,1024);
				char *tempDrvVer = (char*)calloc(1,1024);
				char *tempDrv    = (char*)calloc(1,sizeof(driver));
				char *tempSize   = (char*)calloc(1,1024);
				struct stat _stat;

				strncpy(tempDesc, (const char *) drvdesc, len1);

				SQLSetConfigMode(ODBC_BOTH_DSN);
				SQLGetPrivateProfileString((const char *)  drvdesc, "Driver", "", (char *) driver, sizeof(driver), "odbcinst.ini");
				if (driver[0] == '\0')
					SQLGetPrivateProfileString("Default", "Driver","", (char *) driver, sizeof(driver), "odbcinst.ini");

				if (driver[0] != '\0')
				{
					strncpy(tempDrv,  (const char *) driver, 1023);

					drv_hdbc = NULL;
					drv_henv = NULL;

					if ((handle = (void *) DLL_OPEN (driver)) != NULL)
					{
						if ((allocHdl = (pSQLAllocHandle) DLL_PROC (handle, "SQLAllocHandle")) != NULL)
						{
							ret = allocHdl (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &drv_henv);
							if (ret == SQL_ERROR)
								goto nodriverver;
							ret = allocHdl (SQL_HANDLE_DBC, drv_henv, &drv_hdbc);
							if (ret == SQL_ERROR)
								goto nodriverver;
						}
						else
						{
							if ((allocEnvHdl = (pSQLAllocEnv) DLL_PROC (handle, "SQLAllocEnv")) != NULL)
							{
								ret = allocEnvHdl (&drv_henv);
								if (ret == SQL_ERROR)
									goto nodriverver;
							}
							else
								goto nodriverver;

							if ((allocConnectHdl = (pSQLAllocConnect) DLL_PROC (handle, "SQLAllocConnect")) != NULL)
							{
								ret = allocConnectHdl (drv_henv, &drv_hdbc);
								if (ret == SQL_ERROR)
									goto nodriverver;
							}
							else
								goto nodriverver;
						}

						if ((funcHdl = (pSQLGetInfoFunc) DLL_PROC (handle, "SQLGetInfo")) != NULL)
						{
							/* Retrieve some information */
							ret = funcHdl (drv_hdbc, SQL_DRIVER_VER, drvattrs, sizeof (drvattrs), &len);
							if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
							{
								unsigned int z;
								/* Drop the description if one provided */
								for (z = 0; ((char *) drvattrs)[z]; z++)
									if (((char *) drvattrs)[z] == ' ')
										((char *) drvattrs)[z] = '\0';

								strncpy(tempDrvVer, (const char *) drvattrs, 1023);
							}
							else
								goto nodriverver;
						}
						else
							goto nodriverver;
					}
					else
					{
						nodriverver:
						strcpy(tempDrvVer,"-");
					}

					if (drv_hdbc || drv_henv)
					{
						if (allocConnectHdl && (freeConnectHdl = (pSQLFreeConnect) DLL_PROC (handle, "SQLFreeConnect")) != NULL)
						{
							freeConnectHdl (drv_hdbc);
							drv_hdbc = NULL;
						}

						if (allocEnvHdl && (freeEnvHdl = (pSQLFreeEnv) DLL_PROC (handle, "SQLFreeEnv")) != NULL)
						{
							freeEnvHdl (drv_henv);
							drv_henv = NULL;
						}
					}

					if ((drv_hdbc || drv_henv) && (freeHdl = (pSQLFreeHandle) DLL_PROC (handle, "SQLFreeHandle")) != NULL)
					{
						if (drv_hdbc)
							freeHdl (SQL_HANDLE_DBC, drv_hdbc);
						if (drv_henv)
							freeHdl (SQL_HANDLE_ENV, drv_henv);
					}

					if (handle)
						DLL_CLOSE (handle);

					if (!stat((const char *) driver, &_stat))
						snprintf(tempSize, 1023, "%d Kb", (int) (_stat.st_size / 1024));
					else
						strcpy(tempSize, "-");
				}
				else
				{
					strcpy(tempDrv,"-");
					strcpy(tempDrvVer, "-");
					strcpy(tempSize, "-");
				}

				addRow4(&DriverList, tempDesc, tempDrv, tempDrvVer, tempSize);
			}
			while (SQLDrivers(henv, SQL_FETCH_NEXT, drvdesc, NUMTCHAR(drvdesc), &len1, drvattr, NUMTCHAR(drvattr), &len2) == SQL_SUCCESS);
		}

		IIntuition->RefreshSetGadgetAttrs((struct Gadget*)LBDRV, win, NULL, LISTBROWSER_Labels, &DriverList, TAG_DONE);
		return retcode;
	}
	else
	{
		Requester(REQIMAGE_ERROR, PROGRAM_TITLE, GetString(MSG_ERROR_LOADING_ODBC),GetString(MSG_OK), NULL);
		return -1;
	}
}

void addAboutList(Object *LBDRV, struct Window *win)
{
    char _date[1024] = {0}, _size[1024] = {0};
    char *data[6];
    struct stat _stat;
    void *handle, *proc;
    int i;

    IExec->NewList(&AboutList);

    for (i = 0; i < sizeof(iODBC_Components) / sizeof(iODBC_Components[0]); i++)
    {
        memset(&_stat,0x0, sizeof(stat));

        data[0] = iODBC_Components[i].lib_desc;
        data[1] = VERSION;
        data[2] = iODBC_Components[i].lib_name;
        data[3] = "";
        data[4] = "";
        data[5] = iODBC_Components[i].lib_path;

        if ((handle = dlopen(iODBC_Components[i].lib_name, RTLD_LAZY)))
        {
            if ((proc = dlsym(handle, iODBC_Components[i].lib_ver_sym)))
                data[1] = strdup(*(char **) proc);

            if (!stat(iODBC_Components[i].lib_path, &_stat))
            {
                snprintf(_size, 1023, "%lu Kb", (unsigned long) _stat.st_size / 1024L);
                snprintf(_date, 1023, "%s", ctime(&_stat.st_mtime));
                _date[strlen(_date) - 1] = '\0';
                data[3] = strdup(_date);
                data[4] = strdup(_size);
            }
            else
            {
                data[3] = strdup("-");
                data[4] = strdup("-");
            }
            dlclose(handle);

            addRow5(&AboutList, data[0], data[2], data[3], data[4], data[1]);
        }
    }
    IIntuition->RefreshSetGadgetAttrs((struct Gadget*)LBDRV, win, NULL, LISTBROWSER_Labels, &AboutList, TAG_DONE);
}
