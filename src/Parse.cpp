

#include "Parse.h"

using namespace std;

//CParse xCArchiveSaveParse;
// признак начала строки с именем диска.
uint8_t CParse::auiPartitionNameBeginSign[] =
{
    0xe2, 0x94, 0x94, 0xe2, 0x94, 0x80
};

//-----------------------------------------------------------------------------------------------------
// ищет признак начала строки с именем диска.
int8_t CParse::StartSignCheck(FILE *fp)
{
    char cTemp;
    for (uint8_t i = 0;
            ((i < sizeof(auiPartitionNameBeginSign)) &&
             (!(feof(fp))));
            i++)
    {
        cTemp = (char)fgetc(fp);
        if (cTemp != auiPartitionNameBeginSign[i])
        {
            return 0;
        }
    }
    return 1;
}

//-----------------------------------------------------------------------------------------------------
// ищет и извлекает слово в строке.
int8_t CParse::GetColumnSrtingData(FILE *fp, char *pcDestination)
{
    char cTemp;
    uint8_t uiCharIndex;

    uiCharIndex = 0;
    while (1)
    {
        cTemp = (char)fgetc(fp);
        // слово не началось?
        if (cTemp == EMPTY_SPASE_SIGHN)
        {
            // строка закончилась?
            // файл закончился?
            // длина превосходит максимальную?
            if ((feof(fp)) ||
                    (uiCharIndex >= MAX_COLUMN_TEXT_LENGTH))
            {
                pcDestination[uiCharIndex] = 0;
                return 0;
            }
        }
        else
        {
            // слово началось.
            // строка закончилась?
            // файл закончился?
            // длина превосходит максимальную?
            if ((cTemp == '\n') ||
                    (feof(fp)) ||
                    (uiCharIndex >= MAX_COLUMN_TEXT_LENGTH))
            {
                pcDestination[uiCharIndex] = 0;
                return 0;
            }
            else
            {
                pcDestination[uiCharIndex++] = cTemp;
                break;
            }
        }
    }

    while (1)
    {
        cTemp = (char)fgetc(fp);
        // слово кончилось?
        if (cTemp == EMPTY_SPASE_SIGHN)
        {
            // слово кончилось.
            pcDestination[uiCharIndex] = 0;
            return 1;
        }
        else
        {
            // строка закончилась?
            // файл закончился?
            // длина превосходит максимальную?
            if ((cTemp == '\n') ||
                    (feof(fp)) ||
                    (uiCharIndex >= MAX_COLUMN_TEXT_LENGTH))
            {
                pcDestination[uiCharIndex] = 0;
                return 0;
            }
            else
            {
                pcDestination[uiCharIndex++] = cTemp;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
// ищет признак конца строки.
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
// получает информацию о дисках.
void CParse::GetDiskInfo(void)
{
    char acCommand[128];
    // создадим команду.
    sprintf(acCommand,
            "%s%s",
            "lsblk > ",
            pccParseFileName
           );
    // отправим команду.
    (system(acCommand));

    FILE *fp;
    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
    // откроем файл, в который система помещает ответ на запрос.
    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
    {
        printf("Cannot open file.\n");
        exit (1);
    }

    uint8_t i;
    uiFsmState = 0;
    uiDiskIndex = 0;

    // пройдём по дискам, предположительно имеющимся в системе.
    while (!feof(fp) &&
            (uiDiskIndex < MAX_PARSE_DISK_NUMBER) &&
            (uiFsmState != 3))
    {
        switch(uiFsmState)
        {
        case 0:
            // найден признак начала строки с именем диска?
            if (StartSignCheck(fp))
            {
                i = 0;
                uiFsmState = 1;
            }
            else
            {
                uiFsmState = 2;
            }
            break;

        case 1:
            // извлечены все слова из строки?
            if (!(GetColumnSrtingData(fp, acTempBuff)))
            {
                // следующий диск.
                uiDiskIndex++;
                uiFsmState = 0;
            }
            // номер колонки с именем диска?
            if (i == LSBLK_COMMAND_ANSWER_TABLE_NAME_COLUMN)
            {
                strcpy(axTDiskInfo[uiDiskIndex].acName, acTempBuff);
            }
            // номер колонки с размером диска?
            else if (i == LSBLK_COMMAND_ANSWER_TABLE_SIZE_COLUMN)
            {
                strcpy(axTDiskInfo[uiDiskIndex].acSize, acTempBuff);
            }
            // номер колонки с точкой монтирования диска?
            else if (i == LSBLK_COMMAND_ANSWER_TABLE_MOUNTPOINT_COLUMN)
            {
                strcpy(axTDiskInfo[uiDiskIndex].acMountPoint, acTempBuff);
            }
            // следующее слово.
            i++;
            break;

        case 2:
            if (EndSignCheck(fp))
            {
                uiFsmState = 0;
                break;
            }
            else
            {
                uiFsmState = 0;
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
// проверяет - Linux загружен с диска mmc0?
int8_t CParse::CheckMountedDiskMmc0(void)
{
    uint8_t uiResult;
    char acCommand[128];
    // создадим команду.
    sprintf(acCommand,
            "%s%s",
            "lsblk -r -o NAME,MOUNTPOINT | grep '/$' > ",
            pccParseFileName
           );
    // отправим команду.
    (system(acCommand));

    FILE *fp;
    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
    // откроем файл, в который система помещает ответ на запрос.
    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
    {
        printf("Cannot open pccParseFileName.\n");
        exit (1);
    }

    uiResult = 0;
    // получим содержимое первой строки(имя устройства).
    if (fgets(acTempBuff, LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH, fp))
    {
        // Linux загружен с диска mmc0?
        if (memcmp((acTempBuff),
                   "mmcblk0",
                   7) == 0)

        {
            uiResult = 1;
        }
    }
    else
    {
        printf("Cannot read pccParseFileName.\n");
    }
    fclose(fp);
    return uiResult;
}

//-----------------------------------------------------------------------------------------------------
// отправляет соманду BASH и возвращает указатель на строку с ответом.
char* CParse::BashCommandHandler(char* pcCommand)
{
    char acCommand[128];

    // создадим команду.
    sprintf(acCommand,
            "%s > %s",
            pcCommand,
            pccParseFileName
           );
    // отправим команду.
    (system(acCommand));

    FILE *fp;
    char acTempBuff[LSBLK_COMMAND_ANSWER_MAX_ROW_LENGTH];
    // откроем файл, в который система помещает ответ на запрос.
    if ((fp = fopen(pccParseFileName, "r") ) == NULL)
    {
        printf("Cannot open pccParseFileName.\n");
        pcCommand = (char*)0;
        return pcCommand;
    }

    // получим содержимое первой строки(имя устройства).
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
// получает информацию о дисках.
void CParse::GetDiskInfoNew(void)
{
    char acCommand[128];

    // пройдём по дискам, предположительно имеющимся в системе.
    for (int i = 0;
            (i < MAX_PARSE_DISK_NUMBER);
            i++)
    {
        uint8_t uiNrTemp;

        uiNrTemp = ((i + 2));
//        uiNrTemp = ((i + 1) * 2);
        // создадим команду.
        sprintf(acCommand,
                "%s%d%s%s",
                "lsblk | awk '!/в/ && NR == ",
                uiNrTemp,
                " {print $1}' > ",
                pccParseFileName
               );

        // отправим команду.
        (system(acCommand));
//        usleep(1000000);


        indata.open(pccParseFileName, std::ios::binary); // opens the file
        if (!indata)   // file couldn't be opened
        {
            cerr << "Error: file could not be opened" << endl;
            // закроем файл.
            indata.close();
            exit(1);
        }

        memset(axTDiskInfo[i].acName, 0, 16);
        // установим смещение в файле.
        indata.seekg(0, ios_base::beg);
        // прочитаем файл.
        indata.read(axTDiskInfo[i].acName,
                    LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH);


//        printf("GetDiskInfoNew i %d\n\r", i);
////        printf("GetDiskInfoNew ((i + 1) * 2) %d\n\r", ((i + 1) * 2));
////        printf("GetDiskInfoNew uiNrTemp %d\n\r", uiNrTemp);
//        printf("GetDiskInfoNew axTDiskInfo[i].acName %s\n\r", axTDiskInfo[i].acName);
////        printf("GetDiskInfoNew axTDiskInfo[i].acSize %s\n\r", axTDiskInfo[i].acSize);
////        printf("GetDiskInfoNew acCommand %s\n\r", acCommand);

        // закроем файл.
        indata.close();
    }


    // пройдём по дискам, предположительно имеющимся в системе.
    for (int i = 0;
            (i < MAX_PARSE_DISK_NUMBER);
            i++)
    {
        uint8_t uiNrTemp;

        uiNrTemp = ((i + 2));
//        uiNrTemp = ((i + 1) * 2);
        // создадим команду.
        sprintf(acCommand,
                "%s%d%s%s",
                "lsblk | awk '!/в/ && NR == ",
                uiNrTemp,
                " {print $4}' > ",
                pccParseFileName
               );
        // отправим команду.
        (system(acCommand));
//        usleep(100000);


        indata.open(pccParseFileName, std::ios::binary); // opens the file
        if (!indata)   // file couldn't be opened
        {
            cerr << "Error: file could not be opened" << endl;
            // закроем файл.
            indata.close();
            exit(1);
        }

        memset(axTDiskInfo[i].acSize, 0, 16);
        // установим смещение в файле.
        indata.seekg(0, ios_base::beg);
        // прочитаем файл.
        indata.read(axTDiskInfo[i].acSize,
                    LSBLK_COMMAND_ANSWER_MAX_NAME_LENGTH);

//        printf("GetDiskInfoNew i %d\n\r", i);
////        printf("GetDiskInfoNew ((i + 1) * 2) %d\n\r", ((i + 1) * 2));
////        printf("GetDiskInfoNew uiNrTemp %d\n\r", uiNrTemp);
////        printf("GetDiskInfoNew axTDiskInfo[i].acName %s\n\r", axTDiskInfo[i].acName);
//        printf("GetDiskInfoNew axTDiskInfo[i].acSize %s\n\r", axTDiskInfo[i].acSize);
////        printf("GetDiskInfoNew acCommand %s\n\r", acCommand);


        // закроем файл.
        indata.close();
    }




}




