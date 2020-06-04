#ifndef __IODBCGUI_H__
#define __IODBCGUI_H__

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <iodbcext.h>
#include <odbcinst.h>
#include <dlf.h>

#include <stdio.h>

#include "version.h"

#define TEXT(x)     (SQLCHAR *) x
#define TEXTC(x)    (SQLCHAR) x
#define TXTLEN(x)   strlen((char *) x)
#define TXTCMP(x1,x2)   strcmp((char *) x1, (char *) x2)

#define NUMTCHAR(X) (sizeof (X) / sizeof (SQLTCHAR))

HENV  henv      = SQL_NULL_HANDLE;
HDBC  hdbc      = SQL_NULL_HANDLE;
HSTMT hstmt     = SQL_NULL_HANDLE;
int   connected = 0;

struct List DSNlist;
struct List DriverList;
struct List AboutList;

typedef SQLRETURN SQL_API (*pSQLGetInfoFunc) (SQLHDBC hdbc, SQLUSMALLINT fInfoType, SQLPOINTER rgbInfoValue, SQLSMALLINT cbInfoValueMax, SQLSMALLINT * pcbInfoValue);
typedef SQLRETURN SQL_API (*pSQLAllocHandle) (SQLSMALLINT hdl_type, SQLHANDLE hdl_in, SQLHANDLE * hdl_out);
typedef SQLRETURN SQL_API (*pSQLAllocEnv) (SQLHENV * henv);
typedef SQLRETURN SQL_API (*pSQLAllocConnect) (SQLHENV henv, SQLHDBC * hdbc);
typedef SQLRETURN SQL_API (*pSQLFreeHandle) (SQLSMALLINT hdl_type, SQLHANDLE hdl_in);
typedef SQLRETURN SQL_API (*pSQLFreeEnv) (SQLHENV henv);
typedef SQLRETURN SQL_API (*pSQLFreeConnect) (SQLHDBC hdbc);

char *iodbcadm_version = VERSION;

static struct
{
    char *lib_name;
    char *lib_desc;
    char *lib_ver_sym;
    char *lib_path;
} iODBC_Components[] =
{
    {"libiodbc.so",      "iODBC Driver Manager", "iodbc_version",     "SYS:Libs/ODBC/libiodbc.so"},
    {"iODBC",            "iODBC Administrator",  "iodbcadm_version",  "iODBC"},
    {"libiodbcinst.so",  "iODBC Installer",      "iodbcinst_version", "SYS:Libs/ODBC/libiodbcinst.so"}
};

#endif
