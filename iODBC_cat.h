#ifndef IODBC_CAT_H
#define IODBC_CAT_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#ifndef CATCOMP_NUMBERS
#define CATCOMP_NUMBERS
#endif
#ifndef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif
#endif

#ifdef CATCOMP_BLOCK
#ifndef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define MSG_ERROR 1000
#define MSG_OK 1001
#define MSG_SAVE 1002
#define MSG_USE 1003
#define MSG_CANCEL 1004
#define MSG_ABORT 1005
#define MSG_ADD 1006
#define MSG_REMOVE 1007
#define MSG_SORT 1008
#define MSG_YESNO 1009
#define MSG_YESNOCANCEL 1010
#define MSG_ERROR_LOADING_ODBC 1011
#define MSG_TAB_SYSTEM_DSN 1012
#define MSG_TAB_ODBC_DRIVERS 1013
#define MSG_TAB_ABOUT 1014
#define MSG_CI_SYSDSN_0 1015
#define MSG_CI_SYSDSN_1 1016
#define MSG_CI_SYSDSN_2 1017
#define MSG_CI_SYSDRIVERS_0 1018
#define MSG_CI_SYSDRIVERS_1 1019
#define MSG_CI_SYSDRIVERS_2 1020
#define MSG_CI_SYSDRIVERS_3 1021
#define MSG_CI_ABOUT_0 1022
#define MSG_CI_ABOUT_1 1023
#define MSG_CI_ABOUT_2 1024
#define MSG_CI_ABOUT_3 1025
#define MSG_CI_ABOUT_4 1026
#define MSG_SYSTEM_DATA_SOURCES 1027
#define MSG_BUTTON_ADD 1028
#define MSG_BUTTON_REMOVE 1029
#define MSG_BUTTON_CONFIGURE 1030
#define MSG_BUTTON_TEST 1031
#define MSG_SYSTEMDSN_LABEL 1032
#define MSG_ODBC_DRIVERS 1033
#define MSG_ODBCDRIVERS_LABEL 1034
#define MSG_ODBC_COMPONENTS 1035
#define MSG_ODBCCOMPONENTS_LABEL 1036
#define MSG_WINDOW_TITLE 1037
#define MSG_BUTTON_OK 1038
#define MSG_BUTTON_CANCEL 1039
#define MSG_ERROR_LOADDRIVERS 1040
#define MSG_REMOVE_DSN 1041
#define MSG_ERROR_REMOVE_DSN 1042
#define MSG_ERROR_NODRIVERS_INSTALLED 1043
#define MSG_LOGIN_TO_DSN 1044
#define MSG_USERNAME 1045
#define MSG_PASSWORD 1046
#define MSG_BUTTON_FINISH 1047
#define MSG_CONNECTION_OK 1048
#define MSG_CI_KEYS_0 1049
#define MSG_CI_KEYS_1 1050
#define MSG_DSN_SETUP 1051
#define MSG_GENERIC_SETUP 1052
#define MSG_DATA_SOURCE_NAME 1053
#define MSG_DESCRIPTION 1054
#define MSG_KEYWORD 1055
#define MSG_VALUE 1056
#define MSG_BUTTON_UPDATE 1057
#define MSG_WARN_PARAMETERS 1058
#define MSG_WARN_OVERWRITE_DSN 1059
#define MSG_ERROR_WRITE_DSN 1060
#define MSG_ERROR_MISSING_NAME 1061
#define MSG_ERROR_KEY_PAIRS 1062
#define MSG_WARN_REMOVE_KEY 1063
#define MSG_ERROR_CONFIGURE_DSN 1064
#define MSG_ERROR_UPDATE_DSN 1065
#define MSG_CHOOSE_DRIVER 1066
#define MSG_SELECT_DRIVER 1067
#define MSG_ERROR_OPENING_DSNWIN 1068
#define MSG_CONNECTION_WIZARD 1069
#define MSG_WIZARD_TITLE 1070
#define MSG_BUTTON_NEXT 1071
#define MSG_BUTTON_BACK 1072
#define MSG_WIZARD_STEP2_DESC 1073
#define MSG_INSERT_DSN_NAME 1074
#define MSG_DSN_NAME_DESC 1075
#define MSG_INSERT_DSN_DESC 1076
#define MSG_DSN_DESCRIPTION 1077
#define MSG_WIZARD_WELCOME 1078
#define MSG_WIZARD_STEP1_DESC 1079
#define MSG_SERVERNAME_DESC 1080
#define MSG_INSERT_SERVERNAME 1081
#define MSG_DATABASE_DESC 1082
#define MSG_INSERT_DATABASE 1083
#define MSG_HINT_ADD 1084
#define MSG_HINT_REMOVE 1085
#define MSG_HINT_CONFIGURE 1086
#define MSG_HINT_TEST 1087
#define MSG_HINT_LBDRIVERS 1088
#define MSG_HINT_CONNECTION_WIZARD 1089
#define MSG_HINT_WIZARD_SERVERNAME 1090
#define MSG_HINT_WIZARD_DATABASENAME 1091
#define MSG_HINT_WIZARD_DSNNAME 1092
#define MSG_HINT_WIZARD_DSNDESCRIPTION 1093
#define MSG_HINT_AVAILABLEDSN 1094
#define MSG_HINT_AVAILABLEDRIVERS 1095
#define MSG_HINT_AVAILABLECOMPONENTS 1096
#define MSG_HINT_NEWDSN_DESC 1097
#define MSG_HINT_NEWDSN_LISTVIEW 1098
#define MSG_HINT_NEWDSN_KEYWORD 1099
#define MSG_HINT_NEWDSN_VALUE 1100
#define MSG_HINT_NEWDSN_ADD 1101
#define MSG_HINT_NEWDSN_REMOVE 1102
#define MSG_HINT_NEWDSN_UPDATE 1103
#define MSG_HINT_NEWDSN_NAME 1104
#define MSG_ERROR_PARAMETER_ALREADY_EXISTS 1105
#define MSG_ERROR_INVALID_PARAMETER 1106
#define MSG_CONNECTION_OK_WITH_INFO 1107

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define MSG_ERROR_STR "iODBC Error"
#define MSG_OK_STR "Ok"
#define MSG_SAVE_STR "Save"
#define MSG_USE_STR "Use"
#define MSG_CANCEL_STR "Cancel"
#define MSG_ABORT_STR "Abort"
#define MSG_ADD_STR "Add"
#define MSG_REMOVE_STR "Remove"
#define MSG_SORT_STR "Sort"
#define MSG_YESNO_STR "Yes|No"
#define MSG_YESNOCANCEL_STR "Yes|No|Cancel"
#define MSG_ERROR_LOADING_ODBC_STR "Error while connecting to ODBC!"
#define MSG_TAB_SYSTEM_DSN_STR "System DSN"
#define MSG_TAB_ODBC_DRIVERS_STR "ODBC Drivers"
#define MSG_TAB_ABOUT_STR "About"
#define MSG_CI_SYSDSN_0_STR "Name"
#define MSG_CI_SYSDSN_1_STR "Description"
#define MSG_CI_SYSDSN_2_STR "Driver"
#define MSG_CI_SYSDRIVERS_0_STR "Name"
#define MSG_CI_SYSDRIVERS_1_STR "File"
#define MSG_CI_SYSDRIVERS_2_STR "Version"
#define MSG_CI_SYSDRIVERS_3_STR "Size"
#define MSG_CI_ABOUT_0_STR "Name"
#define MSG_CI_ABOUT_1_STR "File"
#define MSG_CI_ABOUT_2_STR "Date"
#define MSG_CI_ABOUT_3_STR "Size"
#define MSG_CI_ABOUT_4_STR "Version"
#define MSG_SYSTEM_DATA_SOURCES_STR "System Data Sources"
#define MSG_BUTTON_ADD_STR "_Add"
#define MSG_BUTTON_REMOVE_STR "_Remove"
#define MSG_BUTTON_CONFIGURE_STR "_Configure"
#define MSG_BUTTON_TEST_STR "_Test"
#define MSG_SYSTEMDSN_LABEL_STR "An ODBC System data source stores information about how to connect to the indicated data provider. A System data source is visible to all users on this machines, including daemons."
#define MSG_ODBC_DRIVERS_STR "ODBC Drivers that are installed on your system"
#define MSG_ODBCDRIVERS_LABEL_STR "An ODBC driver allows ODBC-enabled programs to get information from ODBC data sources. Check the documentation to know how to add a new driver to the system."
#define MSG_ODBC_COMPONENTS_STR "ODBC components installed on your system"
#define MSG_ODBCCOMPONENTS_LABEL_STR "ODBC is a programming interface that enables applications to access data in database management systems that use Structured Query Language (SQL) as a data access standard."
#define MSG_WINDOW_TITLE_STR "iODBC Data Source Administrator"
#define MSG_BUTTON_OK_STR "_Ok"
#define MSG_BUTTON_CANCEL_STR "_Cancel"
#define MSG_ERROR_LOADDRIVERS_STR "Cannot load ODBC drivers"
#define MSG_REMOVE_DSN_STR "Are you sure you want to remove this DSN?"
#define MSG_ERROR_REMOVE_DSN_STR "Error when removing the DSN"
#define MSG_ERROR_NODRIVERS_INSTALLED_STR "No ODBC drivers installed!"
#define MSG_LOGIN_TO_DSN_STR "Login to DSN %s"
#define MSG_USERNAME_STR "Username :"
#define MSG_PASSWORD_STR "Password :"
#define MSG_BUTTON_FINISH_STR "_Finish"
#define MSG_CONNECTION_OK_STR "The connection DSN was tested successfully\nand can be used at this time."
#define MSG_CI_KEYS_0_STR "Keyword"
#define MSG_CI_KEYS_1_STR "Value"
#define MSG_DSN_SETUP_STR "DSN Setup for %s"
#define MSG_GENERIC_SETUP_STR "Generic ODBC Driver Setup"
#define MSG_DATA_SOURCE_NAME_STR "Data Source Name (DSN) :"
#define MSG_DESCRIPTION_STR "Description :"
#define MSG_KEYWORD_STR "Keyword :"
#define MSG_VALUE_STR "Value :"
#define MSG_BUTTON_UPDATE_STR "_Update"
#define MSG_WARN_PARAMETERS_STR "The Server and Database keywords are required for correct operations. You should define them in the DSN"
#define MSG_WARN_OVERWRITE_DSN_STR "Are you sure you want to overwrite this DSN?"
#define MSG_ERROR_WRITE_DSN_STR "Error writing DSN to file!"
#define MSG_ERROR_MISSING_NAME_STR "You must specify a name for the new DSN!"
#define MSG_ERROR_KEY_PAIRS_STR "Please specify both a Key and a Value!"
#define MSG_WARN_REMOVE_KEY_STR "Are you sure you want to remove the selected Key?"
#define MSG_ERROR_CONFIGURE_DSN_STR "An error occured when trying to configure the DSN!"
#define MSG_ERROR_UPDATE_DSN_STR "Error updating the DSN!"
#define MSG_CHOOSE_DRIVER_STR "Choose an ODBC Driver"
#define MSG_SELECT_DRIVER_STR "Select a driver for which you want to setup a data source"
#define MSG_ERROR_OPENING_DSNWIN_STR "Cannot open the Add DSN window! Make sure all components are installed correctly!"
#define MSG_CONNECTION_WIZARD_STR "Use the connection wizard"
#define MSG_WIZARD_TITLE_STR "Connection Wizard"
#define MSG_BUTTON_NEXT_STR "_Next"
#define MSG_BUTTON_BACK_STR "_Back"
#define MSG_WIZARD_STEP2_DESC_STR "A data source name (DSN) is a data structure that contains the information about a specific database that an Open Database Connectivity ( ODBC ) driver needs in order to connect to it"
#define MSG_INSERT_DSN_NAME_STR "Insert the DSN Name"
#define MSG_DSN_NAME_DESC_STR "The DSN Name is used by ODBC to access connection (f.e. MySQLConnection)"
#define MSG_INSERT_DSN_DESC_STR "Insert the DSN Description"
#define MSG_DSN_DESCRIPTION_STR "The DSN Description allows you to remember which connection is used in the DSN (f.e. Connection to MySQL Server)"
#define MSG_WIZARD_WELCOME_STR "Welcome to Connection Wizard"
#define MSG_WIZARD_STEP1_DESC_STR "The Database Wizard allows you to specify the minimum requirements for a Database connection.\nYou should specify at least a Server Name (or its IP Address) and a Database Name to produce a valid DSN connection.Note that not all ODBC Drivers use the same parameters. For example SQLite connections doesn't needs a Server parameter but only a Database name that in this case will be a physical path (f.e. DH2:mydb.db)"
#define MSG_SERVERNAME_DESC_STR "The Server name should be an IP Address (f.e. 192.168.0.254)"
#define MSG_INSERT_SERVERNAME_STR "Insert the Server Name"
#define MSG_DATABASE_DESC_STR "The Database name is the Database you want connect to (f.e. Northwind)"
#define MSG_INSERT_DATABASE_STR "Insert the Database Name"
#define MSG_HINT_ADD_STR "Add a new DSN connection"
#define MSG_HINT_REMOVE_STR "Remove the selected DSN"
#define MSG_HINT_CONFIGURE_STR "Configure the selected DSN"
#define MSG_HINT_TEST_STR "Test the selected DSN"
#define MSG_HINT_LBDRIVERS_STR "This window show the current installed ODBC drivers"
#define MSG_HINT_CONNECTION_WIZARD_STR "Checking this check box, when you click on Finish button, the iODBC manager will start the Connection Wizard that will help you in the DSN creation."
#define MSG_HINT_WIZARD_SERVERNAME_STR "Insert the server you want to connect to"
#define MSG_HINT_WIZARD_DATABASENAME_STR "Insert the Database name you want to use in your programs"
#define MSG_HINT_WIZARD_DSNNAME_STR "Insert the name of DSN you want to create. This will be used by software to access the data."
#define MSG_HINT_WIZARD_DSNDESCRIPTION_STR "Insert the description of DSN you want to create. This will remember you for which connection the DSN is used."
#define MSG_HINT_AVAILABLEDSN_STR "This window show the current available DSN"
#define MSG_HINT_AVAILABLEDRIVERS_STR "This window show the current available ODBC drivers in the system"
#define MSG_HINT_AVAILABLECOMPONENTS_STR "This window show the current version of iODBC components"
#define MSG_HINT_NEWDSN_DESC_STR "Insert the description of DSN you want to create. This will remember you for which connection the DSN is used."
#define MSG_HINT_NEWDSN_LISTVIEW_STR "This listview contains all ODBC driver parameters actually used by this DSN"
#define MSG_HINT_NEWDSN_KEYWORD_STR "Insert the Parameter name (f.e. Database or Port)"
#define MSG_HINT_NEWDSN_VALUE_STR "Insert the Parameter value"
#define MSG_HINT_NEWDSN_ADD_STR "Insert the current Parameter name/value"
#define MSG_HINT_NEWDSN_REMOVE_STR "Remove the selected parameter"
#define MSG_HINT_NEWDSN_UPDATE_STR "Update the current Parameter name/value"
#define MSG_HINT_NEWDSN_NAME_STR "Insert the name of DSN you want to create. This will be used by software to access the data."
#define MSG_ERROR_PARAMETER_ALREADY_EXISTS_STR "The specified parameter already exists in the DSN."
#define MSG_ERROR_INVALID_PARAMETER_STR "Invalid parameter. DSN, Description and Driver are not accepted has parameters."
#define MSG_CONNECTION_OK_WITH_INFO_STR "The connection DSN was tested successfully\nbut the Driver has returned SQL_SUCCESS_WITH_INFO."

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG         cca_ID;
    CONST_STRPTR cca_Str;
};

STATIC CONST struct CatCompArrayType CatCompArray[] =
{
    {MSG_ERROR,(CONST_STRPTR)MSG_ERROR_STR},
    {MSG_OK,(CONST_STRPTR)MSG_OK_STR},
    {MSG_SAVE,(CONST_STRPTR)MSG_SAVE_STR},
    {MSG_USE,(CONST_STRPTR)MSG_USE_STR},
    {MSG_CANCEL,(CONST_STRPTR)MSG_CANCEL_STR},
    {MSG_ABORT,(CONST_STRPTR)MSG_ABORT_STR},
    {MSG_ADD,(CONST_STRPTR)MSG_ADD_STR},
    {MSG_REMOVE,(CONST_STRPTR)MSG_REMOVE_STR},
    {MSG_SORT,(CONST_STRPTR)MSG_SORT_STR},
    {MSG_YESNO,(CONST_STRPTR)MSG_YESNO_STR},
    {MSG_YESNOCANCEL,(CONST_STRPTR)MSG_YESNOCANCEL_STR},
    {MSG_ERROR_LOADING_ODBC,(CONST_STRPTR)MSG_ERROR_LOADING_ODBC_STR},
    {MSG_TAB_SYSTEM_DSN,(CONST_STRPTR)MSG_TAB_SYSTEM_DSN_STR},
    {MSG_TAB_ODBC_DRIVERS,(CONST_STRPTR)MSG_TAB_ODBC_DRIVERS_STR},
    {MSG_TAB_ABOUT,(CONST_STRPTR)MSG_TAB_ABOUT_STR},
    {MSG_CI_SYSDSN_0,(CONST_STRPTR)MSG_CI_SYSDSN_0_STR},
    {MSG_CI_SYSDSN_1,(CONST_STRPTR)MSG_CI_SYSDSN_1_STR},
    {MSG_CI_SYSDSN_2,(CONST_STRPTR)MSG_CI_SYSDSN_2_STR},
    {MSG_CI_SYSDRIVERS_0,(CONST_STRPTR)MSG_CI_SYSDRIVERS_0_STR},
    {MSG_CI_SYSDRIVERS_1,(CONST_STRPTR)MSG_CI_SYSDRIVERS_1_STR},
    {MSG_CI_SYSDRIVERS_2,(CONST_STRPTR)MSG_CI_SYSDRIVERS_2_STR},
    {MSG_CI_SYSDRIVERS_3,(CONST_STRPTR)MSG_CI_SYSDRIVERS_3_STR},
    {MSG_CI_ABOUT_0,(CONST_STRPTR)MSG_CI_ABOUT_0_STR},
    {MSG_CI_ABOUT_1,(CONST_STRPTR)MSG_CI_ABOUT_1_STR},
    {MSG_CI_ABOUT_2,(CONST_STRPTR)MSG_CI_ABOUT_2_STR},
    {MSG_CI_ABOUT_3,(CONST_STRPTR)MSG_CI_ABOUT_3_STR},
    {MSG_CI_ABOUT_4,(CONST_STRPTR)MSG_CI_ABOUT_4_STR},
    {MSG_SYSTEM_DATA_SOURCES,(CONST_STRPTR)MSG_SYSTEM_DATA_SOURCES_STR},
    {MSG_BUTTON_ADD,(CONST_STRPTR)MSG_BUTTON_ADD_STR},
    {MSG_BUTTON_REMOVE,(CONST_STRPTR)MSG_BUTTON_REMOVE_STR},
    {MSG_BUTTON_CONFIGURE,(CONST_STRPTR)MSG_BUTTON_CONFIGURE_STR},
    {MSG_BUTTON_TEST,(CONST_STRPTR)MSG_BUTTON_TEST_STR},
    {MSG_SYSTEMDSN_LABEL,(CONST_STRPTR)MSG_SYSTEMDSN_LABEL_STR},
    {MSG_ODBC_DRIVERS,(CONST_STRPTR)MSG_ODBC_DRIVERS_STR},
    {MSG_ODBCDRIVERS_LABEL,(CONST_STRPTR)MSG_ODBCDRIVERS_LABEL_STR},
    {MSG_ODBC_COMPONENTS,(CONST_STRPTR)MSG_ODBC_COMPONENTS_STR},
    {MSG_ODBCCOMPONENTS_LABEL,(CONST_STRPTR)MSG_ODBCCOMPONENTS_LABEL_STR},
    {MSG_WINDOW_TITLE,(CONST_STRPTR)MSG_WINDOW_TITLE_STR},
    {MSG_BUTTON_OK,(CONST_STRPTR)MSG_BUTTON_OK_STR},
    {MSG_BUTTON_CANCEL,(CONST_STRPTR)MSG_BUTTON_CANCEL_STR},
    {MSG_ERROR_LOADDRIVERS,(CONST_STRPTR)MSG_ERROR_LOADDRIVERS_STR},
    {MSG_REMOVE_DSN,(CONST_STRPTR)MSG_REMOVE_DSN_STR},
    {MSG_ERROR_REMOVE_DSN,(CONST_STRPTR)MSG_ERROR_REMOVE_DSN_STR},
    {MSG_ERROR_NODRIVERS_INSTALLED,(CONST_STRPTR)MSG_ERROR_NODRIVERS_INSTALLED_STR},
    {MSG_LOGIN_TO_DSN,(CONST_STRPTR)MSG_LOGIN_TO_DSN_STR},
    {MSG_USERNAME,(CONST_STRPTR)MSG_USERNAME_STR},
    {MSG_PASSWORD,(CONST_STRPTR)MSG_PASSWORD_STR},
    {MSG_BUTTON_FINISH,(CONST_STRPTR)MSG_BUTTON_FINISH_STR},
    {MSG_CONNECTION_OK,(CONST_STRPTR)MSG_CONNECTION_OK_STR},
    {MSG_CI_KEYS_0,(CONST_STRPTR)MSG_CI_KEYS_0_STR},
    {MSG_CI_KEYS_1,(CONST_STRPTR)MSG_CI_KEYS_1_STR},
    {MSG_DSN_SETUP,(CONST_STRPTR)MSG_DSN_SETUP_STR},
    {MSG_GENERIC_SETUP,(CONST_STRPTR)MSG_GENERIC_SETUP_STR},
    {MSG_DATA_SOURCE_NAME,(CONST_STRPTR)MSG_DATA_SOURCE_NAME_STR},
    {MSG_DESCRIPTION,(CONST_STRPTR)MSG_DESCRIPTION_STR},
    {MSG_KEYWORD,(CONST_STRPTR)MSG_KEYWORD_STR},
    {MSG_VALUE,(CONST_STRPTR)MSG_VALUE_STR},
    {MSG_BUTTON_UPDATE,(CONST_STRPTR)MSG_BUTTON_UPDATE_STR},
    {MSG_WARN_PARAMETERS,(CONST_STRPTR)MSG_WARN_PARAMETERS_STR},
    {MSG_WARN_OVERWRITE_DSN,(CONST_STRPTR)MSG_WARN_OVERWRITE_DSN_STR},
    {MSG_ERROR_WRITE_DSN,(CONST_STRPTR)MSG_ERROR_WRITE_DSN_STR},
    {MSG_ERROR_MISSING_NAME,(CONST_STRPTR)MSG_ERROR_MISSING_NAME_STR},
    {MSG_ERROR_KEY_PAIRS,(CONST_STRPTR)MSG_ERROR_KEY_PAIRS_STR},
    {MSG_WARN_REMOVE_KEY,(CONST_STRPTR)MSG_WARN_REMOVE_KEY_STR},
    {MSG_ERROR_CONFIGURE_DSN,(CONST_STRPTR)MSG_ERROR_CONFIGURE_DSN_STR},
    {MSG_ERROR_UPDATE_DSN,(CONST_STRPTR)MSG_ERROR_UPDATE_DSN_STR},
    {MSG_CHOOSE_DRIVER,(CONST_STRPTR)MSG_CHOOSE_DRIVER_STR},
    {MSG_SELECT_DRIVER,(CONST_STRPTR)MSG_SELECT_DRIVER_STR},
    {MSG_ERROR_OPENING_DSNWIN,(CONST_STRPTR)MSG_ERROR_OPENING_DSNWIN_STR},
    {MSG_CONNECTION_WIZARD,(CONST_STRPTR)MSG_CONNECTION_WIZARD_STR},
    {MSG_WIZARD_TITLE,(CONST_STRPTR)MSG_WIZARD_TITLE_STR},
    {MSG_BUTTON_NEXT,(CONST_STRPTR)MSG_BUTTON_NEXT_STR},
    {MSG_BUTTON_BACK,(CONST_STRPTR)MSG_BUTTON_BACK_STR},
    {MSG_WIZARD_STEP2_DESC,(CONST_STRPTR)MSG_WIZARD_STEP2_DESC_STR},
    {MSG_INSERT_DSN_NAME,(CONST_STRPTR)MSG_INSERT_DSN_NAME_STR},
    {MSG_DSN_NAME_DESC,(CONST_STRPTR)MSG_DSN_NAME_DESC_STR},
    {MSG_INSERT_DSN_DESC,(CONST_STRPTR)MSG_INSERT_DSN_DESC_STR},
    {MSG_DSN_DESCRIPTION,(CONST_STRPTR)MSG_DSN_DESCRIPTION_STR},
    {MSG_WIZARD_WELCOME,(CONST_STRPTR)MSG_WIZARD_WELCOME_STR},
    {MSG_WIZARD_STEP1_DESC,(CONST_STRPTR)MSG_WIZARD_STEP1_DESC_STR},
    {MSG_SERVERNAME_DESC,(CONST_STRPTR)MSG_SERVERNAME_DESC_STR},
    {MSG_INSERT_SERVERNAME,(CONST_STRPTR)MSG_INSERT_SERVERNAME_STR},
    {MSG_DATABASE_DESC,(CONST_STRPTR)MSG_DATABASE_DESC_STR},
    {MSG_INSERT_DATABASE,(CONST_STRPTR)MSG_INSERT_DATABASE_STR},
    {MSG_HINT_ADD,(CONST_STRPTR)MSG_HINT_ADD_STR},
    {MSG_HINT_REMOVE,(CONST_STRPTR)MSG_HINT_REMOVE_STR},
    {MSG_HINT_CONFIGURE,(CONST_STRPTR)MSG_HINT_CONFIGURE_STR},
    {MSG_HINT_TEST,(CONST_STRPTR)MSG_HINT_TEST_STR},
    {MSG_HINT_LBDRIVERS,(CONST_STRPTR)MSG_HINT_LBDRIVERS_STR},
    {MSG_HINT_CONNECTION_WIZARD,(CONST_STRPTR)MSG_HINT_CONNECTION_WIZARD_STR},
    {MSG_HINT_WIZARD_SERVERNAME,(CONST_STRPTR)MSG_HINT_WIZARD_SERVERNAME_STR},
    {MSG_HINT_WIZARD_DATABASENAME,(CONST_STRPTR)MSG_HINT_WIZARD_DATABASENAME_STR},
    {MSG_HINT_WIZARD_DSNNAME,(CONST_STRPTR)MSG_HINT_WIZARD_DSNNAME_STR},
    {MSG_HINT_WIZARD_DSNDESCRIPTION,(CONST_STRPTR)MSG_HINT_WIZARD_DSNDESCRIPTION_STR},
    {MSG_HINT_AVAILABLEDSN,(CONST_STRPTR)MSG_HINT_AVAILABLEDSN_STR},
    {MSG_HINT_AVAILABLEDRIVERS,(CONST_STRPTR)MSG_HINT_AVAILABLEDRIVERS_STR},
    {MSG_HINT_AVAILABLECOMPONENTS,(CONST_STRPTR)MSG_HINT_AVAILABLECOMPONENTS_STR},
    {MSG_HINT_NEWDSN_DESC,(CONST_STRPTR)MSG_HINT_NEWDSN_DESC_STR},
    {MSG_HINT_NEWDSN_LISTVIEW,(CONST_STRPTR)MSG_HINT_NEWDSN_LISTVIEW_STR},
    {MSG_HINT_NEWDSN_KEYWORD,(CONST_STRPTR)MSG_HINT_NEWDSN_KEYWORD_STR},
    {MSG_HINT_NEWDSN_VALUE,(CONST_STRPTR)MSG_HINT_NEWDSN_VALUE_STR},
    {MSG_HINT_NEWDSN_ADD,(CONST_STRPTR)MSG_HINT_NEWDSN_ADD_STR},
    {MSG_HINT_NEWDSN_REMOVE,(CONST_STRPTR)MSG_HINT_NEWDSN_REMOVE_STR},
    {MSG_HINT_NEWDSN_UPDATE,(CONST_STRPTR)MSG_HINT_NEWDSN_UPDATE_STR},
    {MSG_HINT_NEWDSN_NAME,(CONST_STRPTR)MSG_HINT_NEWDSN_NAME_STR},
    {MSG_ERROR_PARAMETER_ALREADY_EXISTS,(CONST_STRPTR)MSG_ERROR_PARAMETER_ALREADY_EXISTS_STR},
    {MSG_ERROR_INVALID_PARAMETER,(CONST_STRPTR)MSG_ERROR_INVALID_PARAMETER_STR},
    {MSG_CONNECTION_OK_WITH_INFO,(CONST_STRPTR)MSG_CONNECTION_OK_WITH_INFO_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/



#endif /* IODBC_CAT_H */
