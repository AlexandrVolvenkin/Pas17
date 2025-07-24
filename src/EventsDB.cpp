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

#include "Task.h"
#include "iconvlite.h"
#include <iconv.h>
#include "Configuration.h"
#include "Resources.h"
#include "Events.h"
#include "EventsDB.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
//CArchiveEventsDB xCArchiveEventsDB;
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
        // получим строку данных текущей колонки.
        strcpy(&acQueryDataExchange[ui16QueryDataExchangeIndex], argv[i]);
        // сохраним индекс строки данных текущей колонки.
        aui16QueryDataExchangeIndex[ui8QueryGetColumnQuantity] =
            ui16QueryDataExchangeIndex;
        // следующая колонка.
        ui16QueryDataExchangeIndex += (ui8Length + END_OF_STRING_LENGTH);
        ui8QueryGetColumnQuantity++;
    }
    ui8QueryGetRowQuantity++;
    // Return successful.
    return 0;
}

////-----------------------------------------------------------------------------------------------------
//// открывает базу данных.
//int CEventsDB::Connect(void)
//{
//    //std::cout << "CEventsDB::Connect 1"  << std::endl;
//    int rc;
//    char* zErrMsg = 0;
//    char acQuery[512];
//    const char *pccQuery;
//
//    // Получим указатель на буфер с серийным номером и идентификатором прибора.
//    m_puiSerialAndId =
//        (GetResources() -> m_puiSerialAndId);
//
//    std::string cSerialAndIdStr;
//    // Копируем данные из m_puiSerialAndId в cSerialAndIdStr
//    cSerialAndIdStr.assign((const char*)m_puiSerialAndId, SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH);
//    // Создаем пути к папкам и файлу
//    std::string sArchveFlashFile = "/home/debian/EventsArchive_" + cSerialAndIdStr + "_" + ".db";
//
//    rc = sqlite3_open(sArchveFlashFile.c_str(), &db);
//    if (rc)
//    {
//        //std::cout << "CEventsDB::Connect 2"  << std::endl;
//        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
//        sqlite3_close(db);
//        return 1;
//    }
//
//    //std::cout << "CEventsDB::Connect 3"  << std::endl;
//    // для проверки, запросим первое событие в базе данных.
//    // если таблицы не существует, создадим.
//    Cp1251ToUtf8(acQuery, \
//                 "SELECT rowid FROM Events WHERE rowid = 1;");
//    rc = sqlite3_exec(db,
//                      (const char*)acQuery,
//                      Callback,
//                      0,
//                      &zErrMsg);
//    if (rc != SQLITE_OK)
//    {
//        //std::cout << "CEventsDB::Connect 4"  << std::endl;
//        cout << "SQLite error: " << zErrMsg << endl;
//        sqlite3_free(zErrMsg);
//
//        // создадим таблицу в базе данных.
//        Cp1251ToUtf8(acQuery, \
//                     "CREATE TABLE Events \
//                     (TextData text, \
//                     StateText text, \
//                     Address INTEGER, \
//                     Date date, \
//                     Time time, \
//                     Type INTEGER, \
//                     State INTEGER);");
//        rc = sqlite3_exec(db,
//                          acQuery,
//                          Callback,
//                          0,
//                          &zErrMsg);
//
//        if (rc != SQLITE_OK)
//        {
//            //std::cout << "CEventsDB::Connect 5"  << std::endl;
//            cout << "CREATE TABLE Events SQLite error: " << zErrMsg << endl;
//            sqlite3_free(zErrMsg);
//            return 1;
//        }
//        else
//        {
//            //std::cout << "CEventsDB::Connect 6"  << std::endl;
//            cout << "SQLite table create ok!" << endl;
//            return 0;
//        }
//    }
//    else
//    {
//        //std::cout << "CEventsDB::Connect 7"  << std::endl;
//        cout << "SQLite open ok!" << endl;
//        return 0;
//    }
//}

//-----------------------------------------------------------------------------------------------------
// открывает базу данных.
int CEventsDB::Connect(uint8_t* puiSerialAndId)
{
    //std::cout << "CEventsDB::Connect 1"  << std::endl;
    int rc;
    char* zErrMsg = 0;
    char acQuery[512];
    const char *pccQuery;

//    // Получим указатель на буфер с серийным номером и идентификатором прибора.
//    m_puiSerialAndId =
//        (GetResources() -> m_puiSerialAndId);

    std::string cSerialAndIdStr;
    // Копируем данные из m_puiSerialAndId в cSerialAndIdStr
    cSerialAndIdStr.assign((const char*)puiSerialAndId, SERIAL_AND_ID_DATA_BASE_BLOCK_LENGTH);
    // Создаем пути к папкам и файлу
    std::string sArchveFlashFile = "/home/debian/EventsArchive_" + cSerialAndIdStr + "_" + ".db";

    rc = sqlite3_open(sArchveFlashFile.c_str(), &db);
    if (rc)
    {
        //std::cout << "CEventsDB::Connect 2"  << std::endl;
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    //std::cout << "CEventsDB::Connect 3"  << std::endl;
    // для проверки, запросим первое событие в базе данных.
    // если таблицы не существует, создадим.
    Cp1251ToUtf8(acQuery, \
                 "SELECT rowid FROM Events WHERE rowid = 1;");
    rc = sqlite3_exec(db,
                      (const char*)acQuery,
                      Callback,
                      0,
                      &zErrMsg);
    if (rc != SQLITE_OK)
    {
        //std::cout << "CEventsDB::Connect 4"  << std::endl;
        cout << "SQLite error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);

        // создадим таблицу в базе данных.
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
            //std::cout << "CEventsDB::Connect 5"  << std::endl;
            cout << "CREATE TABLE Events SQLite error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg);
            return 1;
        }
        else
        {
            //std::cout << "CEventsDB::Connect 6"  << std::endl;
            cout << "SQLite table create ok!" << endl;
            return 0;
        }
    }
    else
    {
        //std::cout << "CEventsDB::Connect 7"  << std::endl;
        cout << "SQLite open ok!" << endl;
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
// закрывает базу данных.
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
// для проверки, запросим количество записей в таблице базе данных.
    int rc;
    char* zErrMsg = 0;
    char acQuery[512];

    Cp1251ToUtf8(acQuery, pcQuery);
    // подготовим счётчики к приёму первой строки, первой колонки данных запроса.
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

    // получим ответ на запрос к СУБД.
    return atoi(&acQueryDataExchange[0]);
}

//-----------------------------------------------------------------------------------------------------
// помещает событие в базу данных.
int CEventsDB::DataBaseDataPush(CEvents::TEventDataCommon *pxSource)
{
    //std::cout << "CEventsDB::DataBaseDataPush 1"  << std::endl;
//    if (Connect())
//    {
//        return 1;
//    }

    int rc;
    char* zErrMsg = 0;
    char acQuery[512];
    char accDateStr[16];
    char accTimeStr[16];

    // запросим количество записей в таблице базы данных.
    // количество записей в таблице базы данных больше или равно 2000?
    if (SendQuery("SELECT COUNT(*) FROM Events;") >= 2000)
    {
        //std::cout << "CEventsDB::DataBaseDataPush 2"  << std::endl;
        // установим флаг - размер базы данных достиг максимума.
        bDataBaseIsFull = true;
    }

    // преобразуем дату и время в нужный формат.
    strftime(accDateStr, 100, "%Y-%m-%d", &(pxSource -> xCurrentTime));// date;
    strftime(accTimeStr, 100, "%H:%M:%S", &(pxSource -> xCurrentTime));// time

    // создадим запрос для записи в таблицу базы данных.
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

    //std::cout << "CEventsDB::DataBaseDataPush 3"  << std::endl;
    rc = sqlite3_exec(db,
                      (const char*)Cp1251ToUtf8(acQuery, acQuery),
                      Callback,
                      0,
                      &zErrMsg);

    if( rc != SQLITE_OK )
    {
        //std::cout << "CEventsDB::DataBaseDataPush 4"  << std::endl;
        cout << "SQLite error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else
    {
        //std::cout << "CEventsDB::DataBaseDataPush 5"  << std::endl;
        cout << "Records created successfully" << endl;

//        printf("SELECT last_insert_rowid %d\n\r", (SendQuery("SELECT last_insert_rowid();")));
//        printf("SELECT last_insert_rowid %d\n\r", (SendQuery("SELECT rowid FROM Events ORDER BY ROWID ASC LIMIT 1;")));
//        printf("SELECT last_insert_rowid %d\n\r", (SendQuery("SELECT rowid FROM Events ORDER BY ROWID DESC LIMIT 1;")));



    }

    //std::cout << "CEventsDB::DataBaseDataPush 6"  << std::endl;
//    Close();
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// закольцовывает базу данных.
int CEventsDB::DataBaseRingCreate(void)
{
    char acCommand[128];

    // удалим 1000 самых старых записей.
    SendQuery("DELETE FROM Events WHERE rowid <= 1000;");
    // удалим лишнее.
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
//        // создадим строку SQL запроса.
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
//        // создадим строку SQL запроса.
//        // направление чтения вперёд(от большего ID к меньшему ID, от ближнего события к дальнему)?
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
//        // преобразуем дату и время в нужный формат.
//        strftime(accDateStr, 100, "%Y-%m-%d", &xTime);// date;
////        strftime(accTimeStr, 100, "%H:%M:%S", &(pxSource -> xCurrentTime));// time
//        // создадим строку SQL запроса.
//        // направление чтения вперёд(от большего ID к меньшему ID, от ближнего события к дальнему)?
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
//    // подготовим счётчики к приёму первой строки, первой колонки данных запроса.
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
//            // получим указатель для сохранения данных события,
//            // пришедшего в ответе от СУБД.
//            pxDestination = xCPlcArchiveEvent.EventDataPush();
//            // получим индекс смещения на строки в даных получаемых по запросу от СУБД.
//            // получим индекс на данные первой колонки в строке запроса.
//            pui16QueryDataExchangeIndex =
//                &aui16QueryDataExchangeIndex[i * EVENTS_DB_QUERY_COLUMN_QUANTITY];
//
//            // идём от большего ID к меньшему?
//            if (CEventLogQuery::ui8DirectionIsForward)
//            {
//                // идём от большего ID к меньшему.
//                // сохраним меньший ID в ответе на запрос.
//                ui16LastLastID =
//                    atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//                pxDestination -> ui16ID = ui16LastLastID;
//                if (i == 0)
//                {
//                    // сохраним больший ID в ответе на запрос.
//                    ui16LastFirstID = ui16LastLastID;
//                }
//            }
//            else
//            {
//                // идём от меньшего ID к большему.
//                // сохраним больший ID в ответе на запрос.
//                ui16LastFirstID =
//                    atoi(&pacQueryDataExchange[*pui16QueryDataExchangeIndex++]);
//                pxDestination -> ui16ID = ui16LastFirstID;
//                if (i == 0)
//                {
//                    // сохраним меньший ID в ответе на запрос.
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
// возвращает количество принятых записей, при последнем запросе.
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
    // 2 - количество символов в представлении часа.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcTimeString[ui8TimeStringIndex++];
    }
    // поместим признак конца строки.
    acTempString[i] =  0;
    pxTimeInt -> tm_hour = atoi(acTempString);

    // пропустим символ - ':'.
    ui8TimeStringIndex++;

    // 2 - количество символов в представлении минуты.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcTimeString[ui8TimeStringIndex++];
    }
    // поместим признак конца строки.
    acTempString[i] =  0;
    pxTimeInt -> tm_min = atoi(acTempString);

    // пропустим символ - ':'.
    ui8TimeStringIndex++;

    // 2 - количество символов в представлении секунды.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcTimeString[ui8TimeStringIndex++];
    }
    // поместим признак конца строки.
    acTempString[i] =  0;
    pxTimeInt -> tm_sec = atoi(acTempString);
}

//-----------------------------------------------------------------------------------------------------
int CEventsDB::DateAtoi(struct tm *pxDateInt, char *pcDateString)
{
    char acTempString[8];
    uint8_t i;
    uint8_t ui8DateStringIndex;


    // начнём с символов десятков года.
    ui8DateStringIndex = 2;
    // 2 - нужное количество символов в представлении года.
    // начнём с десятков.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcDateString[ui8DateStringIndex++];
    }
    // поместим признак конца строки.
    acTempString[i] =  0;
    pxDateInt -> tm_year = (atoi(acTempString) + 100);

    // пропустим символ - '-'.
    ui8DateStringIndex++;

    // 2 - количество символов в представлении месяца.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcDateString[ui8DateStringIndex++];
    }
    // поместим признак конца строки.
    acTempString[i] =  0;
    pxDateInt -> tm_mon = (atoi(acTempString) - 1);

    // пропустим символ - '-'.
    ui8DateStringIndex++;

    // 2 - количество символов в представлении дня.
    for (i = 0; i < 2; i++)
    {
        acTempString[i] =  pcDateString[ui8DateStringIndex++];
    }
    // поместим признак конца строки.
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
