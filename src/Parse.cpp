
#include "Parse.h"

CParse xCArchiveSaveParse;
// ������� ������ ������ � ������ �����.
uint8_t CParse::aui8PartitionNameBeginSign[] =
{
    0xe2, 0x94, 0x94, 0xe2, 0x94, 0x80
};

//-----------------------------------------------------------------------------------------------------
// ���� ������� ������ ������ � ������ �����.
int8_t CParse::StartSignCheck(FILE *fp)
{
    char cTemp;
    for (uint8_t i = 0;
            ((i < sizeof(aui8PartitionNameBeginSign)) &&
             (!(feof(fp))));
            i++)
    {
        cTemp = (char)fgetc(fp);
        if (cTemp != aui8PartitionNameBeginSign[i])
        {
            return 0;
        }
    }
    return 1;
}

//-----------------------------------------------------------------------------------------------------
// ���� � ��������� ����� � ������.
int8_t CParse::GetColumnSrtingData(FILE *fp, char *pcDestination)
{
    char cTemp;
    uint8_t ui8CharIndex;

    ui8CharIndex = 0;
    while (1)
    {
        cTemp = (char)fgetc(fp);
        // ����� �� ��������?
        if (cTemp == EMPTY_SPASE_SIGHN)
        {
            // ������ �����������?
            // ���� ����������?
            // ����� ����������� ������������?
            if ((feof(fp)) ||
                    (ui8CharIndex >= MAX_COLUMN_TEXT_LENGTH))
            {
                pcDestination[ui8CharIndex] = 0;
                return 0;
            }
        }
        else
        {
            // ����� ��������.
            // ������ �����������?
            // ���� ����������?
            // ����� ����������� ������������?
            if ((cTemp == '\n') ||
                    (feof(fp)) ||
                    (ui8CharIndex >= MAX_COLUMN_TEXT_LENGTH))
            {
                pcDestination[ui8CharIndex] = 0;
                return 0;
            }
            else
            {
                pcDestination[ui8CharIndex++] = cTemp;
                break;
            }
        }
    }

    while (1)
    {
        cTemp = (char)fgetc(fp);
        // ����� ���������?
        if (cTemp == EMPTY_SPASE_SIGHN)
        {
            // ����� ���������.
            pcDestination[ui8CharIndex] = 0;
            return 1;
        }
        else
        {
            // ������ �����������?
            // ���� ����������?
            // ����� ����������� ������������?
            if ((cTemp == '\n') ||
                    (feof(fp)) ||
                    (ui8CharIndex >= MAX_COLUMN_TEXT_LENGTH))
            {
                pcDestination[ui8CharIndex] = 0;
                return 0;
            }
            else
            {
                pcDestination[ui8CharIndex++] = cTemp;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
// ���� ������� ����� ������.
int8_t CParse::EndSignCheck(FILE *fp)
{
    while (!(feof(fp)))
    {
        if (fgetc(fp) == '\n')
        {
            return 1;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------
// �������� ���������� � ������.
void CParse::GetDiskInfo(void)
{
    char acCommand[128];
    // �������� �������.
    sprintf(acCommand,
            "%s%s",
            "lsblk > ",
            pccParseFileName
           );
    // �������� �������.
    (system(acCommand));

    FILE *fp;
    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
    // ������� ����, � ������� ������� �������� ����� �� ������.
    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
    {
        printf("Cannot open file.\n");
        exit (1);
    }

    uint8_t i;
    ui8FsmState = 0;
    ui8DiskIndex = 0;

    // ������ �� ������, ���������������� ��������� � �������.
    while (!feof(fp) &&
            (ui8DiskIndex < MAX_PARSE_DISK_NUMBER) &&
            (ui8FsmState != 3))
    {
        switch(ui8FsmState)
        {
        case 0:
            // ������ ������� ������ ������ � ������ �����?
            if (StartSignCheck(fp))
            {
                i = 0;
                ui8FsmState = 1;
            }
            else
            {
                ui8FsmState = 2;
            }
            break;

        case 1:
            // ��������� ��� ����� �� ������?
            if (!(GetColumnSrtingData(fp, acTempBuff)))
            {
                // ��������� ����.
                ui8DiskIndex++;
                ui8FsmState = 0;
            }
            // ����� ������� � ������ �����?
            if (i == LSBLK_COMMAND_ANSWER_TABLE_NAME_COLUMN)
            {
                strcpy(axTDiskInfo[ui8DiskIndex].acName, acTempBuff);
            }
            // ����� ������� � �������� �����?
            else if (i == LSBLK_COMMAND_ANSWER_TABLE_SIZE_COLUMN)
            {
                strcpy(axTDiskInfo[ui8DiskIndex].acSize, acTempBuff);
            }
            // ����� ������� � ������ ������������ �����?
            else if (i == LSBLK_COMMAND_ANSWER_TABLE_MOUNTPOINT_COLUMN)
            {
                strcpy(axTDiskInfo[ui8DiskIndex].acMountPoint, acTempBuff);
            }
            // ��������� �����.
            i++;
            break;

        case 2:
            if (EndSignCheck(fp))
            {
                ui8FsmState = 0;
                break;
            }
            else
            {
                ui8FsmState = 0;
                break;
            }

        case 3:
            break;

        default:
            break;
        };
    }
    fclose(fp);

//    for (i = 0; i < MAX_PARSE_DISK_NUMBER; i++)
//    {
//        printf("axTDiskInfo[i].acName %s\n\r", (axTDiskInfo[i].acName));
//        printf("axTDiskInfo[i].acSize %s\n\r", (axTDiskInfo[i].acSize));
//        printf("axTDiskInfo[i].acMountPoint %s\n\r", (axTDiskInfo[i].acMountPoint));
//    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� - Linux �������� � ����� mmc0?
int8_t CParse::CheckMountedDiskMmc0(void)
{
    uint8_t ui8Result;
    char acCommand[128];
    // �������� �������.
    sprintf(acCommand,
            "%s%s",
            "lsblk -r -o NAME,MOUNTPOINT | grep '/$' > ",
            pccParseFileName
           );
    // �������� �������.
    (system(acCommand));

    FILE *fp;
    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
    // ������� ����, � ������� ������� �������� ����� �� ������.
    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
    {
        printf("Cannot open pccParseFileName.\n");
        exit (1);
    }

    ui8Result = 0;
    // ������� ���������� ������ ������(��� ����������).
    if (fgets(acTempBuff, LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH, fp))
    {
        // Linux �������� � ����� mmc0?
        if (memcmp((acTempBuff),
                   "mmcblk0",
                   7) == 0)

        {
            ui8Result = 1;
        }
    }
    else
    {
        printf("Cannot read pccParseFileName.\n");
    }
    fclose(fp);
    return ui8Result;
}

//-----------------------------------------------------------------------------------------------------
// ���������� ������� BASH � ���������� ��������� �� ������ � �������.
char* CParse::BashCommandHandler(char* pcCommand)
{
    char acCommand[128];

    // �������� �������.
    sprintf(acCommand,
            "%s > %s",
            pcCommand,
            pccParseFileName
           );
    // �������� �������.
    (system(acCommand));

    FILE *fp;
    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
    // ������� ����, � ������� ������� �������� ����� �� ������.
    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
    {
        printf("Cannot open pccParseFileName.\n");
        pcCommand = (char*)0;
        return pcCommand;
    }

    // ������� ���������� ������ ������(��� ����������).
    if (fgets(pcCommand, LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH, fp))
    {

    }
    else
    {
        fclose(fp);
        printf("Cannot read pccParseFileName.\n");
        pcCommand = (char*)0;
        return pcCommand;
    }

    fclose(fp);

    int sl = strlen(pcCommand) - 1;
    if(pcCommand[sl] == '\n')
    {
        pcCommand[sl] = 0;
    }

    return pcCommand;
}

//-----------------------------------------------------------------------------------------------------
// �������� ���������� � ������.
void CParse::GetDiskInfoNew(void)
{
    char acCommand[128];

//    // �������� �������.
//    sprintf(acCommand,
//            "%s%s",
//            "lsblk | awk '!/�/ {print $1}' > ",
//            pccParseFileName
//           );
//    // �������� �������.
//    (system(acCommand));
//
//    FILE *fp;
//    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
//    // ������� ����, � ������� ������� �������� ����� �� ������.
//    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
//    {
//        printf("Cannot open pccParseFileName.\n");
//        exit (1);
//    }
//
//    // ������� ���������� ������ ������(��� ����������).
//    if (fgets(acTempBuff, LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH, fp))
//    {
//        // Linux �������� � ����� mmc0?
//        if (memcmp((acTempBuff),
//                   "mmcblk0",
//                   7) == 0)
//
//        {
//
//        }
//    }
//    else
//    {
//        printf("Cannot read pccParseFileName.\n");
//    }
//    fclose(fp);




//    indata.open(pccParseFileName, std::ios::app); // opens the file
//    if (!indata)   // file couldn't be opened
//    {
//        cerr << "Error: file could not be opened" << endl;
//        exit(1);
//    }

    // ������ �� ������, ���������������� ��������� � �������.
    for (int i = 0;
            (i < MAX_PARSE_DISK_NUMBER);
            i++)
    {
        uint8_t ui8NrTemp;

        ui8NrTemp = ((i + 1) * 2);
        // �������� �������.
        sprintf(acCommand,
                "%s%d%s%s",
                "lsblk | awk '!/�/ && NR == ",
                ui8NrTemp,
                " {print $1}' > ",
                pccParseFileName
               );

        // �������� �������.
        (system(acCommand));
//        usleep(1000000);


        indata.open(pccParseFileName, std::ios::binary); // opens the file
        if (!indata)   // file couldn't be opened
        {
            cerr << "Error: file could not be opened" << endl;
            // ������� ����.
            indata.close();
            exit(1);
        }

        // ��������� �������� � �����.
        indata.seekg(0, ios_base::beg);
        // ��������� ����.
        indata.read(axTDiskInfo[i].acName,
                    LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH);
//        strcpy(axTDiskInfo[ui8DiskIndex].acName, acTempBuff);
//            strcpy(axTDiskInfo[ui8DiskIndex].acSize, acTempBuff);
//            strcpy(axTDiskInfo[ui8DiskIndex].acMountPoint, acTempBuff);


        printf("GetDiskInfoNew i %d\n\r", i);
        printf("GetDiskInfoNew ((i + 1) * 2) %d\n\r", ((i + 1) * 2));
        printf("GetDiskInfoNew ui8NrTemp %d\n\r", ui8NrTemp);
        printf("GetDiskInfoNew axTDiskInfo[i].acName %s\n\r", axTDiskInfo[i].acName);
        printf("GetDiskInfoNew axTDiskInfo[i].acSize %s\n\r", axTDiskInfo[i].acSize);
        printf("GetDiskInfoNew acCommand %s\n\r", acCommand);

        // ������� ����.
        indata.close();





//        FILE *fp;
//        char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
//        // ������� ����, � ������� ������� �������� ����� �� ������.
//        if ((fp = fopen(pccParseFileName, "r") ) == NULL)
//        {
//            printf("Cannot open pccParseFileName.\n");
//            exit (1);
//        }
//
//        // ������� ���������� ������ ������(��� ����������).
//        if (fgets(acTempBuff, LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH, fp))
//        {
//            // Linux �������� � ����� mmc0?
//            if (memcmp((acTempBuff),
//                       "mmcblk0",
//                       7) == 0)
//
//            {
//
//            }
//        }
//        else
//        {
//            printf("Cannot read pccParseFileName.\n");
//        }
//
//
//        printf("GetDiskInfoNew acTempBuff %s\n\r", acTempBuff);
//
//        fclose(fp);

    }

    // ������ �� ������, ���������������� ��������� � �������.
    for (int i = 0;
            (i < MAX_PARSE_DISK_NUMBER);
            i++)
    {
        uint8_t ui8NrTemp;

        ui8NrTemp = ((i + 1) * 2);
        // �������� �������.
        sprintf(acCommand,
                "%s%d%s%s",
                "lsblk | awk '!/�/ && NR == ",
                ui8NrTemp,
                " {print $4}' > ",
                pccParseFileName
               );
        // �������� �������.
        (system(acCommand));
//        usleep(100000);


        indata.open(pccParseFileName, std::ios::binary); // opens the file
        if (!indata)   // file couldn't be opened
        {
            cerr << "Error: file could not be opened" << endl;
            // ������� ����.
            indata.close();
            exit(1);
        }

        // ��������� �������� � �����.
        indata.seekg(0, ios_base::beg);
        // ��������� ����.
        indata.read(axTDiskInfo[i].acSize,
                    LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH);
//        strcpy(axTDiskInfo[ui8DiskIndex].acName, acTempBuff);
//            strcpy(axTDiskInfo[ui8DiskIndex].acSize, acTempBuff);
//            strcpy(axTDiskInfo[ui8DiskIndex].acMountPoint, acTempBuff);



        // ������� ����.
        indata.close();
    }




}




