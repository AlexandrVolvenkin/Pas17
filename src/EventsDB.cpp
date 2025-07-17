//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include <typeinfo>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <cstdlib>

#include "iconvlite.h"
#include <iconv.h>
#include "Configuration.h"
#include "Events.h"
#include "EventsDB.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
CArchiveEventsDB xCArchiveEventsDB;
char CEventsDB::acQueryDataExchange[];
uint16_t CEventsDB::aui16QueryDataExchangeIndex[];
uint16_t CEventsDB::ui16QueryDataExchangeIndex;
uint8_t CEventsDB::ui8QueryGetRowQuantity;
uint8_t CEventsDB::ui8QueryGetColumnQuantity;

//-------------------------------------------------------------------------------
CEventsDB::CEventsDB()
{

}

//-------------------------------------------------------------------------------
CEventsDB::~CEventsDB()
{

}

//-----------------------------------------------------------------------------------------------------
int CEventsDB::Callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value
    uint8_t ui8Length;

    for(int i = 0; i < argc; i++)
    {
        ui8Length = strlen(argv[i]);
        if (ui8Length > EVENTS_DB_QUERY_COLUMN_LENGTH)
        {
            // Return error.
            return 1;
        }
        // ������� ������ ������ ������� �������.
        strcpy(&acQueryDataExchange[ui16QueryDataExchangeIndex], argv[i]);
        // �������� ������ ������ ������ ������� �������.
        aui16QueryDataExchangeIndex[ui8QueryGetColumnQuantity] =
            ui16QueryDataExchangeIndex;
        // ��������� �������.
        ui16QueryDataExchangeIndex += (ui8Length + END_OF_STRING_LENGTH);
        ui8QueryGetColumnQuantity++;
    }
    ui8QueryGetRowQuantity++;
    // Return successful.
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ��������� ���� ������.
int CEventsDB::Connect(void)
{
    int rc;
    char* zErrMsg = 0;
    char acQuery[512];
    const char *pccQuery;

    rc = sqlite3_open(pccEventsDataBaseName, &db);
    if (rc)
    {
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // ��� ��������, �������� ������ ������� � ���� ������.
    // ���� ������� �� ����������, ��������.
    Cp1251ToUtf8(acQuery, \
                 "SELECT rowid FROM Events WHERE rowid = 1;");
    rc = sqlite3_exec(db,
                      (const char*)acQuery,
                      Callback,
                      0,
                      &zErrMsg);
    if (rc != SQLITE_OK)
    {
        cout << "SQLite error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);

        // �������� ������� � ���� ������.
        Cp1251ToUtf8(acQuery, \
                     "CREATE TABLE Events \
                     (TextData text, \
                     StateText text, \
                     Address INTEGER, \
                     Date date, \
                     Time time, \
                     Type INTEGER, \
                     State INTEGER);");
        rc = sqlite3_exec(db,
                          acQuery,
                          Callback,
                          0,
                          &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cout << "CREATE TABLE Events SQLite error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
            return 1;
        }
        else
        {
            cout << "SQLite table create ok!" << endl;
            return 0;
        }
    }
    else
    {
        cout << "SQLite open ok!" << endl;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ���� ������.
void CEventsDB::Close(void)
{
    if (db)
    {
        sqlite3_close(db);
        db = 0;
    }
}

//-----------------------------------------------------------------------------------------------------
int CEventsDB::SendQuery(char *pcQuery)
{
// ��� ��������, �������� ���������� ������� � ������� ���� ������.
    int rc;
    char* zErrMsg = 0;
    char acQuery[512];

    Cp1251ToUtf8(acQuery, pcQuery);
    // ���������� �������� � ����� ������ ������, ������ ������� ������ �������.
    ui16QueryDataExchangeIndex = 0;
    ui8QueryGetColumnQuantity = 0;
    ui8QueryGetRowQuantity = 0;

    rc = sqlite3_exec(db,
                      (const char*)Cp1251ToUtf8(acQuery, acQuery),
                      Callback,
                      0,
                      &zErrMsg);
    if( rc != SQLITE_OK )
    {
        cout << "SQLite error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << "DataBase answer ok" << endl;
    }

    // ������� ����� �� ������ � ����.
    return atoi(&acQueryDataExchange[0]);
}

//-----------------------------------------------------------------------------------------------------
// �������� ������� � ���� ������.
int CEventsDB::DataBaseDataPush(CEvents::TEventDataCommon *pxSource)
{
//    if (Connect())
//    {
//        return 1;
//    }

    int rc;
    char* zErrMsg = 0;
    char acQuery[512];
    char accDateStr[16];
    char accTimeStr[16];

    // �������� ���������� ������� � ������� ���� ������.
    // ���������� ������� � ������� ���� ������ ������ ��� ����� 2000?
    if (SendQuery("SELECT COUNT(*) FROM Events;") >= 2000)
    {
        // ��������� ���� - ������ ���� ������ ������ ���������.
        bDataBaseIsFull = true;
    }

    // ����������� ���� � ����� � ������ ������.
    strftime(accDateStr, 100, "%Y-%m-%d", &(pxSource -> xCurrentTime));// date;
    strftime(accTimeStr, 100, "%H:%M:%S", &(pxSource -> xCurrentTime));// time

    // �������� ������ ��� ������ � ������� ���� ������.
    sprintf((char*)acQuery,
            "%s ('%s', '%s', '%s', '%s', '%s', '%s', '%s') %s ('%s', '%s', '%d', '%s', '%s', '%d', '%d');",
            "INSERT INTO Events",
            "TextData",
            "StateText",
            "Address",
            "Date",
            "Time",
            "Type",
            "State",
            "VALUES",
            pxSource -> acTextDescriptor,
            pxSource -> acTextDescriptorAdditional,
            pxSource -> ui16Address,
            accDateStr,
            accTimeStr,
            pxSource -> ui8Type,
            pxSource -> ui8State
           );

    rc = sqlite3_exec(db,
                      (const char*)Cp1251ToUtf8(acQuery, acQuery),
                      Callback,
                      0,
                      &zErrMsg);

    if( rc != SQLITE_OK )
    {
        cout << "SQLite error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << "Records created successfully" << endl;

//        printf("SELECT last_insert_rowid %d\n\r", (SendQuery("SELECT last_insert_rowid();")));
//        printf("SELECT last_insert_rowid %d\n\r", (SendQuery("SELECT rowid FROM Events ORDER BY ROWID ASC LIMIT 1;")));
//        printf("SELECT last_insert_rowid %d\n\r", (SendQuery("SELECT rowid FROM Events ORDER BY ROWID DESC LIMIT 1;")));



    }

//    Close();
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// �������������� ���� ������.
int CEventsDB::DataBaseRingCreate(void)
{
    char acCommand[128];

    // ������ 1000 ����� ������ �������.
    SendQuery("DELETE FROM Events WHERE rowid <= 1000;");
    // ������ ������.
    SendQuery("VACUUM;");

    return 0;
}

//-----------------------------------------------------------------------------------------------------
bool CEventsDB::DataBaseIsFull(void)
{
    return bDataBaseIsFull;
}

//-----------------------------------------------------------------------------------------------------
void CEventsDB::DataBaseIsFullReset(void)
{
    bDataBaseIsFull = false;
}

////-----------------------------------------------------------------------------------------------------
//int CEventsDB::DataBaseDataGet(void)
//{
////    if (Connect())
////    {
////        return 1;
////    }
//
//    int rc;
//    char* zErrMsg = 0;
//    char acQuery[512];
//    char accDateStr[16];
//    char accTimeStr[16];
//    uint16_t *pui16QueryDataExchangeIndex;
//    char *pacQueryDataExchange;
//    CEvents::TEventDataCommon *pxDestination;
//
//    uint16_t ui16ID;
//
//    ui16ID = ui16LastFirstID;
//
//    switch(CEvents::xEventsLogQueryList.ui8Order)
//    {
//    case CEvents::REQUESTED_EVENTS_ORDER_DEFAULT:
//        printf("REQUESTED_EVENTS_ORDER_DEFAULT i16Index %d\n\r", (CEvents::xEventsLogQueryList.i16Index));
////        printf("REQUESTED_EVENTS_ORDER_DEFAULT ui16LastFirstID %d\n\r", (ui16LastFirstID));
////        printf("REQUESTED_EVENTS_ORDER_DEFAULT ui16LastLastID %d\n\r", (ui16LastLastID));
//        // �������� ������ SQL �������.
//        if (CEventLogQuery::ui8DirectionIsForward)
//        {
//            sprintf((char*)acQuery,
//                    "%s '%d' %s %s '%d';",
//                    "SELECT rowid, TextData, StateText, Address, Date, Time, Type, State FROM Events WHERE rowid <",
//                    ui16LastFirstID,
//                    "ORDER BY rowid DESC",
//                    "LIMIT",
//                    EVENTS_DB_QUERY_ROW_QUANTITY);
//        }
//        else
//        {
//            sprintf((char*)acQuery,
//                    "%s '%d' %s %s '%d';",
//                    "SELECT rowid, TextData, StateText, Address, Date, Time, Type, State FROM Events WHERE rowid >",
//                    ui16LastLastID,
//                    "ORDER BY rowid ASC",
//                    "LIMIT",
//                    EVENTS_DB_QUERY_ROW_QUANTITY);
//        }
//
//        break;
//    case CEvents::REQUESTED_EVENTS_ORDER_BY_NUMBER:
//        printf("REQUESTED_EVENTS_ORDER_BY_NUMBER ui16ID %d\n\r", (ui16ID));
//        printf("REQUESTED_EVENTS_ORDER_BY_NUMBER ui16Address %d\n\r", (CEvents::xEventsLogQueryList.ui16Address));
//        // �������� ������ SQL �������.
//        // ����������� ������ �����(�� �������� ID � �������� ID, �� �������� ������� � ��������)?
//        if (CEventLogQuery::ui8DirectionIsForward)
//        {
//            sprintf((char*)acQuery,
//                    "%s '%d' %s '%d' %s '%d' %s '%d' %s %s '%d' %s %s '%d';",
//                    "SELECT rowid, TextData, StateText, Address, Date, Time, Type, State FROM Events WHERE Address ==",
//                    CEvents::xEventsLogQueryList.ui16Address,
//                    "AND (Type ==",
//                    CEvents::HANDLED_EVENTS_ALARM_TYPE,
//                    "OR Type ==",
//                    CEvents::HANDLED_EVENTS_DISCRETE_INPUTS_TYPE,
//                    "OR Type ==",
//                    CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE,
//                    ")",
//                    "AND rowid <",
//                    ui16LastFirstID,
//                    "ORDER BY rowid DESC",
//                    "LIMIT",
//                    EVENTS_DB_QUERY_ROW_QUANTITY);
//        }
//        else
//        {
//            sprintf((char*)acQuery,
//                    "%s '%d' %s '%d' %s '%d' %s '%d' %s %s '%d' %s %s '%d';",
//                    "SELECT rowid, TextData, StateText, Address, Date, Time, Type, State FROM Events WHERE Address ==",
//                    CEvents::xEventsLogQueryList.ui16Address,
//                    "AND (Type ==",
//                    CEvents::HANDLED_EVENTS_ALARM_TYPE,
//                    "OR Type ==",
//                    CEvents::HANDLED_EVENTS_DISCRETE_INPUTS_TYPE,
//                    "OR Type ==",
//                    CEvents::HANDLED_EVENTS_FUNCTION_BLOCKS_TYPE,
//                    ")",
//                    "AND rowid >",
//                    ui16LastLastID,
//                    "ORDER BY rowid ASC",
//                    "LIMIT",
//                    EVENTS_DB_QUERY_ROW_QUANTITY);
//        }
//
//        break;
//    case CEvents::REQUESTED_EVENTS_ORDER_BY_DATE:
//        struct tm xTime;
//        xTime.tm_mday = CEvents::xEventsLogQueryList.ui8MonthDay;
//        xTime.tm_mon = CEvents::xEventsLogQueryList.ui8Month - 1;
//        xTime.tm_year = CEvents::xEventsLogQueryList.ui8Year + 100;
//        // ����������� ���� � ����� � ������ ������.
//        strftime(accDateStr, 100, "%Y-%m-%d", &xTime);// date;
////        strftime(accTimeStr, 100, "%H:%M:%S", &(pxSource -> xCurrentTime));// time
//        // �������� ������ SQL �������.
//        // ����������� ������ �����(�� �������� ID � �������� ID, �� �������� ������� � ��������)?
//        if (CEventLogQuery::ui8DirectionIsForward)
//        {
//            sprintf((char*)acQuery,
//                    "%s '%s' %s '%d' %s %s '%d';",
//                    "SELECT rowid, TextData, StateText, Address, Date, Time, Type, State FROM Events WHERE Date ==",
//                    accDateStr,
//                    "AND rowid <",
//                    ui16LastFirstID,
//                    "ORDER BY rowid DESC",
//                    "LIMIT",
//                    EVENTS_DB_QUERY_ROW_QUANTITY);
//        }
//        else
//        {
//            sprintf((char*)acQuery,
//                    "%s '%s' %s '%d' %s %s '%d';",
//                    "SELECT rowid, TextData, StateText, Address, Date, Time, Type, State FROM Events WHERE Date ==",
//                    accDateStr,
//                    "AND rowid >",
//                    ui16LastLastID,
//                    "ORDER BY rowid ASC",
//                    "LIMIT",
//                    EVENTS_DB_QUERY_ROW_QUANTITY);
//        }
//        break;
//    default:
//        break;
//    };
//
//    // ���������� �������� � ����� ������ ������, ������ ������� ������ �������.
//    ui16QueryDataExchangeIndex = 0;
//    ui8QueryGetColumnQuantity = 0;
//    ui8QueryGetRowQuantity = 0;
//    pacQueryDataExchange = acQueryDataExchange;
//
//    rc = sqlite3_exec(db,
//                      (const char*)Cp1251ToUtf8(acQuery, acQuery),
//                      Callback,
//                      0,
//                      &zErrMsg);
//
////    printf("DataBaseDataGet acQueryDataExchange[0] %d\n\r", (atoi(&acQueryDataExchange[0])));
//
//    if( rc != SQLITE_OK )
//    {
//        cout << "SQLite error: " << zErrMsg << endl;
//        sqlite3_free(zErrMsg);
//    }
//    else
//    {
//        cout << "DataBaseDataGet successfully" << endl;
//        pacQueryDataExchange = acQueryDataExchange;
//        for(int i = 0;
//                ((i < ui8QueryGetRowQuantity) &&
//                 (i < EVENTS_DB_QUERY_ROW_QUANTITY));
//                i++)
//        {
//            // ������� ��������� ��� ���������� ������ �������,
//            // ���������� � ������ �� ����.
//            pxDestination = xCPlcArchiveEvent.EventDataPush();
//            // ������� ������ �������� �� ������ � ����� ���������� �� ������� �� ����.
//            // ������� ������ �� ������ ������ ������� � ������ �������.
//            pui16QueryDataExchangeIndex =
//                &aui16QueryDataExchangeIndex[i * EVENTS_DB_QUERY_COLUMN_QUANTITY];
//
//            // ��� �� �������� ID � ��������?
//            if (CEventLogQuery::ui8DirectionIsForward)
//            {
//                // ��� �� �������� ID � ��������.
//                // �������� ������� ID � ������ �� ������.
//                ui16LastLastID =
//                    atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//                pxDestination -> ui16ID = ui16LastLastID;
//                if (i == 0)
//                {
//                    // �������� ������� ID � ������ �� ������.
//                    ui16LastFirstID = ui16LastLastID;
//                }
//            }
//            else
//            {
//                // ��� �� �������� ID � ��������.
//                // �������� ������� ID � ������ �� ������.
//                ui16LastFirstID =
//                    atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//                pxDestination -> ui16ID = ui16LastFirstID;
//                if (i == 0)
//                {
//                    // �������� ������� ID � ������ �� ������.
//                    ui16LastLastID = ui16LastFirstID;
//                }
//            }
//
//            Utf8ToCp1251(pxDestination -> acTextDescriptor,
//                         &pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            Utf8ToCp1251(pxDestination -> acTextDescriptorAdditional,
//                         &pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            pxDestination -> ui16Address =
//                atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            DateAtoi(&(pxDestination -> xCurrentTime),
//                     &pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            TimeAtoi(&(pxDestination -> xCurrentTime),
//                     &pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            pxDestination -> ui8Type =
//                atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            pxDestination -> ui8State =
//                atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//            pxDestination -> i16Index =
//                ((i16IndexQuery) + i);
////            pui16QueryDataExchangeIndex++;
//        }
//    }
//
////    Close();
//    return 0;
//}

//-----------------------------------------------------------------------------------------------------
// ���������� ���������� �������� �������, ��� ��������� �������.
uint8_t CEventsDB::LastQueryRecordsQuantityGet(void)
{
    return ui8QueryGetRowQuantity;
}

//-----------------------------------------------------------------------------------------------------
int CEventsDB::TimeAtoi(struct tm *pxTimeInt, char *pcTimeString)
{
    char acTempString[8];
    uint8_t i;
    uint8_t ui8TimeStringIndex;

    ui8TimeStringIndex = 0;
    // 2 - ���������� �������� � ������������� ����.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcTimeString[ui8TimeStringIndex++];
    }
    // �������� ������� ����� ������.
    acTempString[i] =  0;
    pxTimeInt -> tm_hour = atoi(acTempString);

    // ��������� ������ - ':'.
    ui8TimeStringIndex++;

    // 2 - ���������� �������� � ������������� ������.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcTimeString[ui8TimeStringIndex++];
    }
    // �������� ������� ����� ������.
    acTempString[i] =  0;
    pxTimeInt -> tm_min = atoi(acTempString);

    // ��������� ������ - ':'.
    ui8TimeStringIndex++;

    // 2 - ���������� �������� � ������������� �������.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcTimeString[ui8TimeStringIndex++];
    }
    // �������� ������� ����� ������.
    acTempString[i] =  0;
    pxTimeInt -> tm_sec = atoi(acTempString);
}

//-----------------------------------------------------------------------------------------------------
int CEventsDB::DateAtoi(struct tm *pxDateInt, char *pcDateString)
{
    char acTempString[8];
    uint8_t i;
    uint8_t ui8DateStringIndex;


    // ����� � �������� �������� ����.
    ui8DateStringIndex = 2;
    // 2 - ������ ���������� �������� � ������������� ����.
    // ����� � ��������.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcDateString[ui8DateStringIndex++];
    }
    // �������� ������� ����� ������.
    acTempString[i] =  0;
    pxDateInt -> tm_year = (atoi(acTempString) + 100);

    // ��������� ������ - '-'.
    ui8DateStringIndex++;

    // 2 - ���������� �������� � ������������� ������.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcDateString[ui8DateStringIndex++];
    }
    // �������� ������� ����� ������.
    acTempString[i] =  0;
    pxDateInt -> tm_mon = (atoi(acTempString) - 1);

    // ��������� ������ - '-'.
    ui8DateStringIndex++;

    // 2 - ���������� �������� � ������������� ���.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcDateString[ui8DateStringIndex++];
    }
    // �������� ������� ����� ������.
    acTempString[i] =  0;
    pxDateInt -> tm_mday = atoi(acTempString);
}

//-----------------------------------------------------------------------------------------------------

//One more way to select between dates in SQLite is to use the powerful strftime function:
//
//SELECT * FROM test WHERE strftime('%Y-%m-%d', date) BETWEEN "11-01-2011" AND "11-08-2011"
//
//These are equivalent according to https://sqlite.org/lang_datefunc.html:
//
//date(...)
//
//strftime('%Y-%m-%d', ...)
//
//but if you want more choice, you have it.

//-----------------------------------------------------------------------------------------------------
