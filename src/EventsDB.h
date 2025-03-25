#ifndef EVENTSDATABASE_H_INCLUDED
#define EVENTSDATABASE_H_INCLUDED

//#include "AM335xPlatform.h"
#include "Platform.h"
#include "PasNewConfig.h"
#include "SerialMT.h"
#include <sqlite3.h>


//extern CArchiveEventsDB xCArchiveEventsDB;

//-----------------------------------------------------------------------------------------------------

class CEventsDB
{
public:
    typedef enum
    {
        // порядок сортировки запрашиваемых событий - по номеру.
        EVENTS_DB_QUERY_ORDER_BY_INDEX = 1,
        // порядок сортировки запрашиваемых событий - по адресу.
        EVENTS_DB_QUERY_ORDER_BY_ADDRESS = 2,
        // порядок сортировки запрашиваемых событий - по дате.
        EVENTS_DB_QUERY_ORDER_BY_DATA = 3,
        // порядок сортировки запрашиваемых событий - по времени.
        EVENTS_DB_QUERY_ORDER_BY_TIME = 4,
        // тип запрашиваемых событий - события.
        EVENTS_DB_QUERY_TYPE_EVENTS = 1,
        // тип запрашиваемых событий - ошибки.
        EVENTS_DB_QUERY_TYPE_ERRORS = 2,
        // тип запрашиваемых событий - архив.
        EVENTS_DB_QUERY_TYPE_ARCHIVE = 3,
        // количество запрашиваемых событий.
        EVENTS_DB_QUERY_ROW_QUANTITY = 4,
        // количество колонок в таблице событий.
        EVENTS_DB_QUERY_COLUMN_QUANTITY = 8,
        // количество байт в данных колонки, в таблице событий.
        EVENTS_DB_QUERY_COLUMN_LENGTH = 60,
        // для получения индекса предыдущей записи.
        EVENTS_DB_PREVIOUS_INDEX_GET = 1,
        // для получения индекса текущей записи.
        EVENTS_DB_CURRENT_INDEX_GET = 1,
        // для получения индекса следующей записи.
        EVENTS_DB_NEXT_INDEX_GET = 1,
        // количество записей в окне перед текущей.
        EVENTS_DB_PREVIOUS_RECORDS_QUANTITY = 1,
        // количество записей в окне после текущей.
        EVENTS_DB_NEXT_RECORDS_QUANTITY = 2,
    };

//-----------------------------------------------------------------------------------------------------
    // Pointer to SQLite data base name.
    const char* pccEventsDataBaseName;
//    // Pointer to SQLite previous data base name.
//    const char* pccEventsDataBaseNamePrevious;
//    const char* pccEventsDataBaseNameMerge;
    // Pointer to SQLite connection
    sqlite3 *db;
    // массив даных получаемых по запросу от СУБД.
    static char acQueryDataExchange[((EVENTS_DB_QUERY_COLUMN_LENGTH * EVENTS_DB_QUERY_COLUMN_QUANTITY) *
                                     EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // указатель на массив даных получаемых по запросу от СУБД.
//    static char *pacQueryDataExchange;
//    // массив указателей на даные получаемые по запросу от СУБД.
//    static char *apcQueryDataExchange[EVENTS_DB_QUERY_ROW_QUANTITY]
//    [EVENTS_DB_QUERY_COLUMN_QUANTITY];
//    static char *ppcQueryDataExchange;
    // массив индексов смещения на строки в даных получаемых по запросу от СУБД.
    // данные по запросу поступают от СУБД в виде строк.
    // для данных каждой колонки  - своя строка.
    static uint16_t aui16QueryDataExchangeIndex[(EVENTS_DB_QUERY_COLUMN_QUANTITY *
            EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // указатель на массив индексов смещения на строки в даных получаемых по запросу от СУБД.
//    // данные по запросу поступают от СУБД в виде строк.
//    // для данных каждой колонки  - своя строка.
//    static uint16_t *pui16QueryDataExchangeIndex;

//    // массив даных получаемых по запросу от СУБД, для отображения на МИНД.
//    static CEvents::TEventData axQueryDataCommon[EVENTS_DB_QUERY_ROW_QUANTITY];
    // указатель на массив даных получаемых по запросу от СУБД, для отображения на МИНД.
//    CEvents::TEventData *pxQueryDataCommon;
    static uint16_t ui16QueryDataExchangeIndex;
    static uint8_t ui8QueryGetRowQuantity;
    static uint8_t ui8QueryGetColumnQuantity;
    uint16_t ui16TotalRecordsNumber;
    uint16_t i16IndexQuery;
    // больший id в предыдущем запросе.
    uint16_t ui16LastFirstID;
    // меньший id в предыдущем запросе.
    uint16_t ui16LastLastID;
    // флаг - размер базы данных достиг максимума.
    bool bDataBaseIsFull;

//-----------------------------------------------------------------------------------------------------
    static int Callback(void *, int, char **, char **);
    int Connect(void);
    void Close(void);
    int SendQuery(char *);
    int DataBaseDataPush(CEvents::TEventDataCommon *);
    int DataBaseDataPush(void);
    int DataBaseRingCreate(void);
    bool DataBaseIsFull(void);
    void DataBaseIsFullReset(void);
    int DataBaseDataGet(void);
    uint8_t LastQueryRecordsQuantityGet(void);
    static int DateAtoi(struct tm *pxDateInt, char *pcDateString);
    static int TimeAtoi(struct tm *pxTimeInt, char *pcTimeString);


};

//-----------------------------------------------------------------------------------------------------
class CArchiveEventsDB : public CEventsDB
{
public:
    typedef enum
    {
        // количество запрашиваемых событий.
        EVENTS_DB_QUERY_ROW_QUANTITY = 8,
        // количество колонок в таблице событий.
        EVENTS_DB_QUERY_COLUMN_QUANTITY = 8,
    };

//-----------------------------------------------------------------------------------------------------
//    // массив даных получаемых по запросу от СУБД.
//    char acQueryDataExchange[((EVENTS_DB_QUERY_COLUMN_LENGTH * EVENTS_DB_QUERY_COLUMN_QUANTITY) *
//                              EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // массив указателей на даные получаемые по запросу от СУБД.
//    static char *apcQueryDataExchange[EVENTS_DB_QUERY_ROW_QUANTITY]
//    [EVENTS_DB_QUERY_COLUMN_QUANTITY];
//    // массив индексов смещения на строки в даных получаемых по запросу от СУБД.
//    // данные по запросу поступают от СУБД в виде строк.
//    // для данных каждой колонки  - своя строка.
//    uint16_t aui16QueryDataExchangeIndex[(EVENTS_DB_QUERY_COLUMN_QUANTITY *
//                                          EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // массив даных получаемых по запросу от СУБД, для отображения на МИНД.
//    CEvents::TEventData axQueryDataCommon[EVENTS_DB_QUERY_ROW_QUANTITY];

//-----------------------------------------------------------------------------------------------------

public:
    CArchiveEventsDB()
    {
        pccEventsDataBaseName = "/home/debian/Pas05ArchiveDataBase.db";
//        pccEventsDataBaseNamePrevious = "/home/debian/Pas05ArchiveDataBasePrevious.db";
//        pccEventsDataBaseNameMerge = "/home/debian/Pas05ArchiveDataBaseMerge.db";
//        pccEventsDataBaseName = "Pas05ArchiveDataBase.db";
//        pacQueryDataExchange = acQueryDataExchange;
//        pui16QueryDataExchangeIndex = aui16QueryDataExchangeIndex;
//        pxQueryDataCommon = axQueryDataCommon;
        ui8QueryGetRowQuantity = 0;
        bDataBaseIsFull = false;
    }

    ~CArchiveEventsDB()
    {
        Close();
    }

};
typedef CArchiveEventsDB CArchiveEventsDB;
//-----------------------------------------------------------------------------------------------------

extern CArchiveEventsDB xCArchiveEventsDB;

//#include <iostream>
//
//using namespace std;
//
//class A
//{
//public:
//    int add(int first, int second)
//    {
//        return first + second;
//    }
//
//    int subtract(int first, int second)
//    {
//        return first - second;
//    }
//
//    int operation(int first, int second, int (A::*functocall)(int, int))
//    {
//        return (this->*functocall)(first, second);
//    }
//};
//
//int main()
//{
//    int  a, b;
//    A a_plus, a_minus;
//    int (A::*plus)(int, int) = &A::add;
//    int (A::*minus)(int, int) = &A::subtract;
//    a = a_plus.operation(7, 5, plus);
//    b = a_minus.operation(20, a, minus);
//    cout << "a = " << a << " and b = " << b << endl;
//    return 0;
//}








#endif // EVENTSDATABASE_H_INCLUDED
