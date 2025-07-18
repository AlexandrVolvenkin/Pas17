#ifndef EVENTSDATABASE_H_INCLUDED
#define EVENTSDATABASE_H_INCLUDED
//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------


#include <sqlite3.h>

//#include "Events.h"
class CEvents;
class CTask;

//-----------------------------------------------------------------------------------------------------

class CEventsDB : public CTask
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
    // Pointer to SQLite connection
    sqlite3 *db;
    // ������ ����� ���������� �� ������� �� ����.
    static char acQueryDataExchange[((EVENTS_DB_QUERY_COLUMN_LENGTH * EVENTS_DB_QUERY_COLUMN_QUANTITY) *
                                     EVENTS_DB_QUERY_ROW_QUANTITY)];
    // ������ �������� �������� �� ������ � ����� ���������� �� ������� �� ����.
    // ������ �� ������� ��������� �� ���� � ���� �����.
    // ��� ������ ������ �������  - ���� ������.
    static uint16_t aui16QueryDataExchangeIndex[(EVENTS_DB_QUERY_COLUMN_QUANTITY *
            EVENTS_DB_QUERY_ROW_QUANTITY)];

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

    uint8_t* m_puiSerialAndId;

//-----------------------------------------------------------------------------------------------------
    CEventsDB();
    virtual ~CEventsDB();

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
        pccEventsDataBaseName = "/home/debian/Pas17ArchiveDataBase.db";
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



#endif // EVENTSDATABASE_H_INCLUDED
