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
        // ������� ���������� ������������� ������� - �� ������.
        EVENTS_DB_QUERY_ORDER_BY_INDEX = 1,
        // ������� ���������� ������������� ������� - �� ������.
        EVENTS_DB_QUERY_ORDER_BY_ADDRESS = 2,
        // ������� ���������� ������������� ������� - �� ����.
        EVENTS_DB_QUERY_ORDER_BY_DATA = 3,
        // ������� ���������� ������������� ������� - �� �������.
        EVENTS_DB_QUERY_ORDER_BY_TIME = 4,
        // ��� ������������� ������� - �������.
        EVENTS_DB_QUERY_TYPE_EVENTS = 1,
        // ��� ������������� ������� - ������.
        EVENTS_DB_QUERY_TYPE_ERRORS = 2,
        // ��� ������������� ������� - �����.
        EVENTS_DB_QUERY_TYPE_ARCHIVE = 3,
        // ���������� ������������� �������.
        EVENTS_DB_QUERY_ROW_QUANTITY = 4,
        // ���������� ������� � ������� �������.
        EVENTS_DB_QUERY_COLUMN_QUANTITY = 8,
        // ���������� ���� � ������ �������, � ������� �������.
        EVENTS_DB_QUERY_COLUMN_LENGTH = 60,
        // ��� ��������� ������� ���������� ������.
        EVENTS_DB_PREVIOUS_INDEX_GET = 1,
        // ��� ��������� ������� ������� ������.
        EVENTS_DB_CURRENT_INDEX_GET = 1,
        // ��� ��������� ������� ��������� ������.
        EVENTS_DB_NEXT_INDEX_GET = 1,
        // ���������� ������� � ���� ����� �������.
        EVENTS_DB_PREVIOUS_RECORDS_QUANTITY = 1,
        // ���������� ������� � ���� ����� �������.
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
    // ������ ����� ���������� �� ������� �� ����.
    static char acQueryDataExchange[((EVENTS_DB_QUERY_COLUMN_LENGTH * EVENTS_DB_QUERY_COLUMN_QUANTITY) *
                                     EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // ��������� �� ������ ����� ���������� �� ������� �� ����.
//    static char *pacQueryDataExchange;
//    // ������ ���������� �� ����� ���������� �� ������� �� ����.
//    static char *apcQueryDataExchange[EVENTS_DB_QUERY_ROW_QUANTITY]
//    [EVENTS_DB_QUERY_COLUMN_QUANTITY];
//    static char *ppcQueryDataExchange;
    // ������ �������� �������� �� ������ � ����� ���������� �� ������� �� ����.
    // ������ �� ������� ��������� �� ���� � ���� �����.
    // ��� ������ ������ �������  - ���� ������.
    static uint16_t aui16QueryDataExchangeIndex[(EVENTS_DB_QUERY_COLUMN_QUANTITY *
            EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // ��������� �� ������ �������� �������� �� ������ � ����� ���������� �� ������� �� ����.
//    // ������ �� ������� ��������� �� ���� � ���� �����.
//    // ��� ������ ������ �������  - ���� ������.
//    static uint16_t *pui16QueryDataExchangeIndex;

//    // ������ ����� ���������� �� ������� �� ����, ��� ����������� �� ����.
//    static CEvents::TEventData axQueryDataCommon[EVENTS_DB_QUERY_ROW_QUANTITY];
    // ��������� �� ������ ����� ���������� �� ������� �� ����, ��� ����������� �� ����.
//    CEvents::TEventData *pxQueryDataCommon;
    static uint16_t ui16QueryDataExchangeIndex;
    static uint8_t ui8QueryGetRowQuantity;
    static uint8_t ui8QueryGetColumnQuantity;
    uint16_t ui16TotalRecordsNumber;
    uint16_t i16IndexQuery;
    // ������� id � ���������� �������.
    uint16_t ui16LastFirstID;
    // ������� id � ���������� �������.
    uint16_t ui16LastLastID;
    // ���� - ������ ���� ������ ������ ���������.
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
        // ���������� ������������� �������.
        EVENTS_DB_QUERY_ROW_QUANTITY = 8,
        // ���������� ������� � ������� �������.
        EVENTS_DB_QUERY_COLUMN_QUANTITY = 8,
    };

//-----------------------------------------------------------------------------------------------------
//    // ������ ����� ���������� �� ������� �� ����.
//    char acQueryDataExchange[((EVENTS_DB_QUERY_COLUMN_LENGTH * EVENTS_DB_QUERY_COLUMN_QUANTITY) *
//                              EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // ������ ���������� �� ����� ���������� �� ������� �� ����.
//    static char *apcQueryDataExchange[EVENTS_DB_QUERY_ROW_QUANTITY]
//    [EVENTS_DB_QUERY_COLUMN_QUANTITY];
//    // ������ �������� �������� �� ������ � ����� ���������� �� ������� �� ����.
//    // ������ �� ������� ��������� �� ���� � ���� �����.
//    // ��� ������ ������ �������  - ���� ������.
//    uint16_t aui16QueryDataExchangeIndex[(EVENTS_DB_QUERY_COLUMN_QUANTITY *
//                                          EVENTS_DB_QUERY_ROW_QUANTITY)];
//    // ������ ����� ���������� �� ������� �� ����, ��� ����������� �� ����.
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
