#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <typeinfo>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

//-----------------------------------------------------------------------------------------------------
class CParse
{
public:
    typedef enum
    {
        LSBLK_COMMAND_ANSWER_TABLE_COLUMN_NUMBER = 7,
        LSBLK_COMMAND_ANSWER_TABLE_NAME_COLUMN = 0,
        LSBLK_COMMAND_ANSWER_TABLE_SIZE_COLUMN = 3,
        LSBLK_COMMAND_ANSWER_TABLE_TYPE_COLUMN = 5,
        LSBLK_COMMAND_ANSWER_TABLE_MOUNTPOINT_COLUMN = 6,
        LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH = 100,
        LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH = 16,
        MAX_PARSE_DISK_NUMBER = 8,
        MAX_COLUMN_TEXT_LENGTH = 16,
        EMPTY_SPASE_SIGHN = 0x20
    };

    struct TDiskInfo
    {
        char acName[16];
        char acSize[16];
        char acMountPoint[16];
    };

    int8_t StartSignCheck(FILE *);
    int8_t EndSignCheck(FILE *);
    int8_t GetColumnSrtingData(FILE *, char *);
    void GetDiskInfo(void);
    void GetDiskInfoNew(void);
    int8_t CheckMountedDiskMmc0(void);
    char* BashCommandHandler(char* );

    std::ifstream indata;
    const char *pccParseFileName;
    static uint8_t aui8PartitionNameBeginSign[];
    TDiskInfo axTDiskInfo[MAX_PARSE_DISK_NUMBER];
    uint8_t ui8DiskIndex;
    // положение автомата.
    uint8_t ui8FsmState;

public:
    CParse()
    {
        pccParseFileName = "/tmp/TempParseFile.txt";
    }

    ~CParse()
    {

    }
};

//-----------------------------------------------------------------------------------------------------
//extern CParse xCArchiveSaveParse;

//-----------------------------------------------------------------------------------------------------
// sudo mkdir /mnt/usb
// sudo mount /dev/sda1 /mnt/usb
// sudo mount /dev/sdb1 /mnt/usb
// sudo mount /dev/mmcblk0p1 /mnt/sd0
// sudo mount /dev/mmcblk1p1 /mnt/sd1
//    open("/dev/sda/Archive.db", O_RDWR);

//sudo cp -f /tmp/TempParseFile.txt /mnt/usb/TempParseFile.txt

//-----------------------------------------------------------------------------------------------------
#endif // PARSE_H_INCLUDED
