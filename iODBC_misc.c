#include <proto/intuition.h>
#include <proto/requester.h>
#include <classes/requester.h>

#include <sql.h>
#include <sqlext.h>
#include <sqlucode.h>
#include <iodbcext.h>
#include <odbcinst.h>
#include <dlf.h>

#include "iODBC_cat.h"
#include "version.h"
#include "debug.h"

#define NUMTCHAR(X)	(sizeof (X) / sizeof (SQLTCHAR))

/*
** This is a general purpose requester function.
** No varargs or anything fancy.
*/
int32 Requester( uint32 img, TEXT *title, TEXT *reqtxt, TEXT *buttons, struct Window* win )
{
    Object *req_obj;
    int32 n;

    req_obj = (Object *)IIntuition->NewObject(IRequester->REQUESTER_GetClass(), NULL,
                REQ_TitleText,  title,
                REQ_BodyText,   reqtxt,
                REQ_GadgetText, buttons,
                REQ_Image,      img,
                TAG_DONE );

    if( !req_obj ) return 0;

    n = IIntuition->IDoMethod( req_obj, RM_OPENREQ, NULL, win, NULL);
    IIntuition->DisposeObject( req_obj );

    return n;
}

void DriverError(struct Window *win, HENV henv, HDBC hdbc, HSTMT hstmt)
{
    SQLCHAR buf[250] 		= {0};
    SQLCHAR sqlstate[15] 	= {0};
    char error[300] 		= {0};
	SQLINTEGER native_error = 0;
	SQLRETURN retcode 		= SQL_SUCCESS;

    /* Get statement errors */
	while (hstmt)
	{
		retcode = SQLError(henv, hdbc, hstmt, sqlstate, &native_error,	buf, NUMTCHAR(buf), NULL);
		if (!SQL_SUCCEEDED(retcode))
			break;

        snprintf(error, 299, "%s: %s",(const char *)sqlstate, (const char *)buf);
        Requester(REQIMAGE_ERROR, PROGRAM_TITLE, (char *)error,"OK", win);
    }

    /* Get connection errors */
	while (hdbc)
	{
		retcode = SQLError(henv, hdbc, SQL_NULL_HSTMT, sqlstate, &native_error, buf, NUMTCHAR(buf), NULL);
		if (!SQL_SUCCEEDED(retcode))
			break;

        snprintf(error, 299, "%s: %s",(const char *)sqlstate, (const char *)buf);
        Requester(REQIMAGE_ERROR, PROGRAM_TITLE, (char *)error,"OK", win);
    }

    /* Get environmental errors */
	while (henv)
	{
		retcode =  SQLError(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT, sqlstate, &native_error, buf, NUMTCHAR(buf), NULL);
		if (!SQL_SUCCEEDED (retcode))
			break;

        snprintf(error, 299, "%s: %s",(const char *)sqlstate, (const char *)buf);
        Requester(REQIMAGE_ERROR, PROGRAM_TITLE, (char *)error,"OK", win);
    }
}
