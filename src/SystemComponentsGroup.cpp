//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include "SystemComponentsGroup.h"
#include "PasNewConfig.h"

using namespace std;

////-----------------------------------------------------------------------------------------------------
//CSystemComponentsGroupInterface::CSystemComponentsGroupInterface()
//{
//    //ctor
//}
//
////-----------------------------------------------------------------------------------------------------
//CSystemComponentsGroupInterface::~CSystemComponentsGroupInterface()
//{
//    //dtor
//}

////-----------------------------------------------------------------------------------------------------
//CSystemComponentsGroup::CSystemComponentsGroupInterface(const CSystemComponentsGroupInterface& other)
//{
//    //copy ctor
//}
////-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CSystemComponentsGroupInternalModules::CSystemComponentsGroupInternalModules()
{
    cout << "CSystemComponentsGroupInternalModules construct" << endl;
    m_uiComponentsNumber = 0;
}

//-----------------------------------------------------------------------------------------------------
CSystemComponentsGroupInternalModules::~CSystemComponentsGroupInternalModules()
{
//    for(uint8_t i = 0; i < m_vpxSystemComponents.size(); i++)
    for(uint8_t i = 0; i < m_uiComponentsNumber; i++)
    {
//        delete m_vpxSystemComponents[i];
        delete m_apxSystemComponents[i];
    }
    cout << "CSystemComponentsGroupInternalModules destruct" << endl;
}

//-----------------------------------------------------------------------------------------------------
void CSystemComponentsGroupInternalModules::AddSystemComponent(CSystemComponentInterface* pxSystemComponent)
{
//    m_vpxSystemComponents.push_back(pxSystemComponent);
}

//-----------------------------------------------------------------------------------------------------
void CSystemComponentsGroupInternalModules::AddSystemComponent(uint8_t uiType, uint8_t uiAddress)
{
    switch (uiType)
    {
    case MODULE_TYPE_MVDI:
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMvdi5(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MVDS9:
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMvds9(uiAddress);
        m_uiComponentsNumber++;
        break;

//-----------------------------------------------------------------------------------------------------
    case MODULE_TYPE_MVPS3:
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMvps3(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MVAI5:
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMvai5(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MVST3:
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMvst3(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MVAO3:
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMvao3(uiAddress);
        m_uiComponentsNumber++;
        break;

//-----------------------------------------------------------------------------------------------------
    case MODULE_TYPE_MR51:
//        m_vpxSystemComponents.push_back(new CSystemComponentModuleMr51(uiAddress));
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMr51(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MR53:
//        m_vpxSystemComponents.push_back(new CSystemComponentModuleMr53(uiAddress));
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMr53(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MR54:
//        m_vpxSystemComponents.push_back(new CSystemComponentModuleMr54(uiAddress));
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMr54(uiAddress);
        m_uiComponentsNumber++;
        break;

    case MODULE_TYPE_MR55:
//        m_vpxSystemComponents.push_back(new CSystemComponentModuleMr55(uiAddress));
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMr55(uiAddress);
        m_uiComponentsNumber++;
        break;

//-----------------------------------------------------------------------------------------------------
    case MODULE_TYPE_MTVI5:
//        m_vpxSystemComponents.push_back(new CSystemComponentModuleMtvi5(uiAddress));
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponentModuleMtvi5(uiAddress);
        m_uiComponentsNumber++;
        break;

//-----------------------------------------------------------------------------------------------------
    default:
//        m_vpxSystemComponents.push_back(new CSystemComponent(uiAddress));
        m_apxSystemComponents[m_uiComponentsNumber] = new CSystemComponent(uiAddress);
        m_uiComponentsNumber++;
        break;
    }

}

//-----------------------------------------------------------------------------------------------------
void CSystemComponentsGroupInternalModules::Gathering(void)
{
    std::cout << "CSystemComponentsGroupInternalModules::Gathering 1"  << std::endl;
    unsigned char ucSlaveAddress;
    int i;
    unsigned char auiSpiTxBuffer[TX_RX_BUFF_SIZE];
    unsigned char auiSpiRxBuffer[TX_RX_BUFF_SIZE];

    memset(&xPlcConfigSearch,
           0,
           sizeof(struct TPlcConfigSearch));
    memset(&xPlcConfigSearchTemp,
           0,
           sizeof(struct TPlcConfigSearch));
    memset(&xPlcConfigDataTemp,
           0,
           sizeof(struct TPlcConfigData));

    ucSlaveAddress = 0;

//    for (ucSlaveAddress = 0;
//            ucSlaveAddress < SPI_CHIP_SELECT_MAX_ADDRESS;
//            ucSlaveAddress++)
//    {
//        AddSystemComponent(0,
//                           ucSlaveAddress);
//    }
//    m_uiComponentsNumber = 0;

    // ������� ��������� SPI. ����� ������ ������������?
    for (ucSlaveAddress = 0;
            ucSlaveAddress < SPI_CHIP_SELECT_MAX_ADDRESS;
            ucSlaveAddress++)
    {
        // ���� ������ �� ������� MODULE_REQUEST_QUANTITY ���,
        // �� ��� ��� ��� �� ����������.
        for (i = 0; i < MODULE_REQUEST_QUANTITY; i++)
        {
            usleep(10000);
            auiSpiTxBuffer[0] = GET_MODULE_TYPE_COMMAND;
            m_pxCommunicationDevice -> Exchange(ucSlaveAddress,
                                                auiSpiTxBuffer,
                                                auiSpiRxBuffer,
                                                SPI_PREAMBLE_LENGTH +
                                                GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH +
                                                ONE_BYTE_CRC_LENGTH,
                                                LOW_SPEED_IN_HZ);

//            {
//                cout << "GET_MODULE_TYPE_COMMAND auiSpiRxBuffer" << endl;
//                unsigned char *pucSourceTemp;
//                pucSourceTemp = (unsigned char*)auiSpiRxBuffer;
//                for(int i=0; i<16; )
//                {
//                    for(int j=0; j<8; j++)
//                    {
//                        cout << hex << uppercase << setw(2) << setfill('0') << (unsigned int)pucSourceTemp[i + j] << " ";
//                    }
//                    cout << endl;
//                    i += 8;
//                }
//            }

            // ������ ��������?
            if (auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET] ==
                    GET_MODULE_TYPE_COMMAND) // if echo answer is ok.
            {
//                std::cout << "CSystemComponentsGroupInternalModules::Gathering ucSlaveAddress 1 "  << (int)ucSlaveAddress << std::endl;
                // ����� ������� ������ ��� ������?
                if ((iCrcSummOneByteCompare(&auiSpiRxBuffer[GET_MODULE_TYPE_COMMAND_OFFSET],
                                            GET_MODULE_TYPE_COMMAND_LENGTH +
                                            GET_MODULE_TYPE_COMMAND_ANSWER_LENGTH) > 0))
                {
                    // ������ ������������. �������� �������� ���������� -
                    // ����������� ���������� ������� � �������.
                    (xPlcConfigSearchTemp.ui8ModulesQuantity)++;
                    // �������� ��� ������ � ������� ��� �������������� ���������� ������� ��� ������ �� SPI.
                    xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type =
                        auiSpiRxBuffer[MODULE_TYPE_OFFSET];
                    // �������� ����� ������ � ������� ��� �������������� ���������� ������� ��� ������ �� SPI.
                    xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
                        ucSlaveAddress;

                    AddSystemComponent(auiSpiRxBuffer[MODULE_TYPE_OFFSET],
                                       ucSlaveAddress);
                    std::cout << "CSystemComponentsGroupInternalModules::Gathering ucSlaveAddress "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address << std::endl;
                    std::cout << "CSystemComponentsGroupInternalModules::Gathering ui8Type "  << (int)xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Type << std::endl;
                }
                else
                {
                    // �������� ����� ������ � ������� ��� �������������� ���������� ������� ��� ������ �� SPI.
                    xPlcConfigSearchTemp.axConfigSearch[ucSlaveAddress].ui8Address =
                        ucSlaveAddress;

                    AddSystemComponent(0,
                                       ucSlaveAddress);
                }
                // ������� � ������ ���������� ������.
                break;
            }
        }
    }



    (xPlcConfigSearch.ui8ModulesQuantity) =
        (xPlcConfigSearchTemp.ui8ModulesQuantity);

    (xPlcConfigDataTemp.xConfigData.ucModulesQuantity) =
        (xPlcConfigSearch.ui8ModulesQuantity);
    for (int i = 0;
            i < (SPI_CHIP_SELECT_MAX_ADDRESS);
            i++)
    {
        (xPlcConfigSearch.axConfigSearch[i].ui8Address) =
            (xPlcConfigSearchTemp.axConfigSearch[i].ui8Address);
        (xPlcConfigSearch.axConfigSearch[i].ui8Type) =
            (xPlcConfigSearchTemp.axConfigSearch[i].ui8Type);
    }

    for (int i = 0;
            i < (SPI_CHIP_SELECT_MAX_ADDRESS);
            i++)
    {
        xPlcConfigDataTemp.xConfigData.aucModuleType[i] =
            xPlcConfigSearch.axConfigSearch[i].ui8Type;
    }
}

//-----------------------------------------------------------------------------------------------------
void CSystemComponentsGroupInternalModules::Muster(void)
{

}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponentsGroupInternalModules::IsSequenceSlotOk(void)
{
//    enum
//    {
//// ��� ����������� ������������ ������������������ ������� �� SPI � ���������� ucInsideTypeIndex -
//// ������� ����� ���������� ������� -
//// ����������� �����, ��������� �����,
//// ����������� ������, ��������� ������ � �.�.
//// ucInsideTypeIndex ����� ��������� ��� ���������� ������� ������, ��� ������� � ����.
//        DISCRETE_INPUT_MODULE_GROUP = 1,
//        ANOLOGUE_INPUT_MODULE_GROUP = 2,
//        DISCRETE_OUTPUT_MODULE_GROUP = 3,
//        CURRENT_OUTPUT_MODULE_GROUP = 4,
//    };

    unsigned char ucSequenceCheck;

    ucSequenceCheck = 0;
    // �������� ������� ���������� �������.
    // 1 ������ - ������ ����������� �����, 2 ������ - ������ ����������� �����,
    // 3 ������ - ������ ����������� ������, 4 ������ - ������ ����������� ������.
    for (uint8_t i = 0;
            i < INTERNAL_MODULE_QUANTITY;
            i++)
    {
        switch(xPlcConfigDataTemp.
                xConfigData.aucModuleType[i])

        {
        case MODULE_TYPE_MVDI:
        case MODULE_TYPE_MVDS9:
            if (ucSequenceCheck > DISCRETE_INPUT_MODULE_GROUP)
            {
                printf("Sequence error.\n");
                return 1;
            }
            // ��������� ������ � ������� ������ ������.
            ucSequenceCheck = DISCRETE_INPUT_MODULE_GROUP;
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            if (ucSequenceCheck > ANOLOGUE_INPUT_MODULE_GROUP)
            {
                printf("Sequence error.\n");
                return 1;
            }
            // ��������� ������ � ������� ������ ������.
            ucSequenceCheck = ANOLOGUE_INPUT_MODULE_GROUP;
            break;
        case MODULE_TYPE_MR53:
        case MODULE_TYPE_MR54:
        case MODULE_TYPE_MR55:
        case MODULE_TYPE_MR51:
            if (ucSequenceCheck > DISCRETE_OUTPUT_MODULE_GROUP)
            {
                printf("Sequence error.\n");
                return 1;
            }
            // ��������� ������ � ������� ������ ������.
            ucSequenceCheck = DISCRETE_OUTPUT_MODULE_GROUP;
            break;

        case MODULE_TYPE_MTVI5:
            if (ucSequenceCheck > CURRENT_OUTPUT_MODULE_GROUP)
            {
                printf("Sequence error.\n");
                return 1;
            }
            // ��������� ������ � ������� ������ ������.
            ucSequenceCheck = CURRENT_OUTPUT_MODULE_GROUP;
            break;

        default:
            break;
        };
    }

    printf("Sequence ok.\n");
    return 0;
}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponentsGroupInternalModules::IsConfigurationMatch(void)
{

// ���������� ������� ���������?
    if ((xPlcConfigDataTemp.xConfigData.ucModulesQuantity) ==
            xPlcConfigData.xConfigData.ucModulesQuantity)
    {
        printf("Module quantity does match.\n");
// ������������ ���������?
        if ((memcmp((const void*)&(xPlcConfigDataTemp.xConfigData.aucModuleType),
                    (const void*)&xPlcConfigData.xConfigData.aucModuleType,
                    (INTERNAL_MODULE_QUANTITY))) == 0)
        {
            printf("Configuration does match.\n");
            return true;
        }
        else
        {
            printf("Configuration does not match.\n");
            fbPlcConfigurationError = 1;
            return false;
        }
    }
    else
    {
        printf("Module quantity does not match.\n");
        fbPlcConfigurationError = 1;
        return false;
    }
}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponentsGroupInternalModules::IsDiscreteAndAnalogueModuleQuantityMatch(void)
{
    unsigned char nucIndexNumber;
    unsigned char ucSlaveAddress;
    unsigned char ucDiscreteInputModuleQuantity;
    unsigned char ucAnalogueInputModuleQuantity;

    ucDiscreteInputModuleQuantity = 0;
    ucAnalogueInputModuleQuantity = 0;

    // ���������� ��������� ���������� ��� ���������� �������.
    for (ucSlaveAddress = 0;
            ucSlaveAddress < INTERNAL_MODULE_QUANTITY;
            ucSlaveAddress++)
    {
        switch(xPlcConfigData.
                xConfigData.aucModuleType[ucSlaveAddress])
        {
        case MODULE_TYPE_MVDI:
        case MODULE_TYPE_MVDS9:
            ucDiscreteInputModuleQuantity++;
            break;

        case MODULE_TYPE_MVPS3:
        case MODULE_TYPE_MVAI5:
        case MODULE_TYPE_MVST3:
        case MODULE_TYPE_MVAO3:
            ucAnalogueInputModuleQuantity++;
            break;

        default:
            break;
        };
    }

// ���������� ������� ���������?
    if ((ucDiscreteInputModuleQuantity ==
            (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity)) &&
            (ucAnalogueInputModuleQuantity ==
             (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity)))
    {
        printf("DiscreteInput AnalogueInput Module quantity does match.\n");
        return true;
    }
    else
    {
        printf("DiscreteInput AnalogueInput Module quantity does not match.\n");
        return false;
    }
}

//-----------------------------------------------------------------------------------------------------
void CSystemComponentsGroupInternalModules::NewConfigurationAndDataBaseCreate(void)
{
    // ������ ������� ������������ � ����� ���� ������.
    memset(&xPlcDataBase,
           0,
           sizeof(struct TPlcDataBase));
    iPlcGetModulesDataBase();
    iModuleMrXXStartDataBaseCreate();
    iDiscreteInputStartDataBaseCreate();
    iDiscreteSygnalTextDescriptorStartDataBaseCreate();
    iDimentionsParametersDataBaseCreate();
    printf("The new DataBase of discrete signals have created.\n");
    printf("DataBase don't match.\n");
    printf("DataBase restored from new modules.\n");

    vDataBaseBlockCrcCreate();
}

//-----------------------------------------------------------------------------------------------------
void CSystemComponentsGroupInternalModules::MrXXDataBaseCreate(void)
{
    iPlcGetModulesDataBase();
    iModuleMrXXStartDataBaseCreate();
    printf("The new DataBase of MrXX have created.\n");
    printf("DataBase don't match.\n");

    vDataBaseBlockCrcCreate();
}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponentsGroupInternalModules::IsReadyToStartWork(void)
{

    // ������������ ���������?
    if (IsConfigurationMatch())
    {
        return true;
    }
    else
    {
        return false;
    }

//    for (int i = 0;
//            i < xPlcConfigData.xConfigData.ucModulesQuantity;
//            i++)
//    {
//        if (!(m_apxSystemComponents[i] -> IsAbleToReplace(xPlcConfigData.xConfigData.aucModuleType[i])))
//        {
//            return false;
//        }
//    }
//
//    return true;
}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponentsGroupInternalModules::IsSpecificationCorrect(void)
{
    enum
    {
        SEQUENCE_CHECK = 0,
        SEQUENCE_CHECK_WRONG,
        EMPTY_SLOT_CHECK,
        EMPTY_SLOT_CHECK_WRONG,
        CONFIGURATION_CHECK,
        CONFIGURATION_CHECK_OK,
        CONFIGURATION_CHECK_WRONG,
        CONFIGURATION_AND_DATA_BASE_FILE_CHECK,
        CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK,
        CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG,
        CONFIGURATION_CREATE,
        CONFIGURATION_CREATE_WRONG,
        UNCONDITIONAL_STOP_SIGNALING,
        STOP_SIGNALING_CONFIRMATION_WAITING,
        CONFIGURATION_OK_NORMAL_START
    };


    int i;
    uint8_t ui8FsmState;

    ui8FsmState = SEQUENCE_CHECK;

    while (1)
    {
        switch(ui8FsmState)

        {
        case SEQUENCE_CHECK:
            printf("iPlcConfigurationCompare SEQUENCE_CHECK: ui8FsmState %d\n\r", (ui8FsmState));
            // �������� ������������������ ����� �������?
            if (IsSequenceSlotOk())
            {
                // �������� ������������������ ����� �������.
                // ���������, ��� ������ "������ ������������������" �������������, ������������.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_SEQUENCE_ERROR;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_SEQUENCE_ERROR: ui8FsmState %d\n\r", (ui8FsmState));
                fbPlcConfigurationError = 1;
                ui8FsmState = UNCONDITIONAL_STOP_SIGNALING;
            }
            else
            {
                // �������� - ������ ������ ��� ���� ����������� ������ ������������ ��� ���� ������?.
                ui8FsmState = CONFIGURATION_AND_DATA_BASE_FILE_CHECK;
            }
            break;

        case CONFIGURATION_AND_DATA_BASE_FILE_CHECK:
            printf("iPlcConfigurationCompare CONFIGURATION_AND_DATA_BASE_FILE_CHECK: ui8FsmState %d\n\r", (ui8FsmState));
            // ������ ������ ��� ���� ����������� ������ ������������ ��� ���� ������?
            if (fbPlcConfigurationFileError || fbPlcDataBaseFileError)
            {
                printf("Configuration, or data base file error.\n");
                ui8FsmState = CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG;
            }
            else
            {
                printf("Configuration, or data base file ok.\n");
                ui8FsmState = CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK;
            }
            break;

        case CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK:
            printf("iPlcConfigurationCompare CONFIGURATION_AND_DATA_BASE_FILE_CHECK_OK: ui8FsmState %d\n\r", (ui8FsmState));
            printf("Configuration, or data base file ok.\n");
            ui8FsmState = CONFIGURATION_CHECK;
            break;


        case CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG:
            printf("iPlcConfigurationCompare CONFIGURATION_AND_DATA_BASE_FILE_CHECK_WRONG: ui8FsmState %d\n\r", (ui8FsmState));
            // ������ ������� ������������ � ����� ���� ������.
            NewConfigurationAndDataBaseCreate();

//            {
//                // ������ ������� ������������ � ����� ���� ������.
//                memset(&xPlcDataBase,
//                       0,
//                       sizeof(struct TPlcDataBase));
//                iPlcGetModulesDataBase();
//                iModuleMrXXStartDataBaseCreate();
//                iDiscreteInputStartDataBaseCreate();
//                iDiscreteSygnalTextDescriptorStartDataBaseCreate();
//                iDimentionsParametersDataBaseCreate();
//                printf("DataBase don't match.\n");
//                printf("The new DataBase of discrete signals have created.\n");
//                printf("DataBase restored from new modules.\n");
//                vDataBaseBlockCrcCreate();
//            }

            // ���������, ��� ������ "����� ������������ � ����� ���� ������" ������������� � ������������, �������� �������������.
            xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE;
            printf("iPlcConfigurationCompare CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE: ui8FsmState %d\n\r", (ui8FsmState));
            fbPlcConfigurationError = 1;
            ui8FsmState = STOP_SIGNALING_CONFIRMATION_WAITING;
            break;

        case CONFIGURATION_CHECK:
            printf("iPlcConfigurationCompare CONFIGURATION_CHECK: ui8FsmState %d\n\r", (ui8FsmState));
            // �� ������ ������ � ��� ����������� ������ ������������ ��� ���� ������.
            // ������������ ���������?
            if (IsConfigurationMatch())
            {
                printf("Configuration does match.\n");
                ui8FsmState = CONFIGURATION_CHECK_OK;
            }
            else
            {
                printf("Configuration does not match.\n");
                ui8FsmState = CONFIGURATION_CHECK_WRONG;
            }
            break;

        case CONFIGURATION_CHECK_OK:
            printf("iPlcConfigurationCompare CONFIGURATION_CHECK_OK: ui8FsmState %d\n\r", (ui8FsmState));
//            // ������� ���� ������ ������ � ���� ������ �������.
//            vPlcGetModulesDataBaseCompare();

            // ���� ������ ������ �� ��������� � ����� ������ �������?
            if (iPlcGetModulesDataBase() != 0)
            {
                // ���� ������ ������ �� ��������� � ����� ������ �������.
                // ������� ���� ������ ������.
                printf("DataBase restored from new modules.\n");
                vDataBaseBlockCrcCreate();
                iPlcDataBaseSave();
                vWorkArraysUpdate();
            }

            xPlcConfigDataTransmit.xConfigData.ucErrorCode = 0;
            ui8FsmState = CONFIGURATION_OK_NORMAL_START;
            break;


        case CONFIGURATION_CHECK_WRONG:
            printf("iPlcConfigurationCompare CONFIGURATION_CHECK_WRONG: ui8FsmState %d\n\r", (ui8FsmState));
            // ���������� ������� ����������� � ����������� ����� ���������?
            if (IsDiscreteAndAnalogueModuleQuantityMatch())
            {
                // ������ ������� ������������.
                // ������ ������� ������������ � ����� ���� ������ ��.
                MrXXDataBaseCreate();
//                {
//                    iPlcGetModulesDataBase();
//                    iModuleMrXXStartDataBaseCreate();
//                    printf("DataBase don't match.\n");
//                    printf("The new DataBase of MrXX have created.\n");
//
//                    vDataBaseBlockCrcCreate();
//                }

                // ���������, ��� ������ "����� ������������ � ������� ���� ������" ������������� � ������������, �������� �������������.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_NEW_CONFIGURATION;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_NEW_CONFIGURATION: ui8FsmState %d\n\r", (ui8FsmState));
                fbPlcConfigurationError = 1;
                ui8FsmState = STOP_SIGNALING_CONFIRMATION_WAITING;
            }
            else
            {
                // ������ ������� ������������ � ����� ���� ������.
                NewConfigurationAndDataBaseCreate();
//                {
//                    // ������ ������� ������������ � ����� ���� ������.
//                    memset(&xPlcDataBase,
//                           0,
//                           sizeof(struct TPlcDataBase));
//                    iPlcGetModulesDataBase();
//                    iModuleMrXXStartDataBaseCreate();
//                    iDiscreteInputStartDataBaseCreate();
//                    iDiscreteSygnalTextDescriptorStartDataBaseCreate();
//                    iDimentionsParametersDataBaseCreate();
//                    printf("DataBase don't match.\n");
//                    printf("The new DataBase of discrete signals have created.\n");
//                    printf("DataBase restored from new modules.\n");
//                    vDataBaseBlockCrcCreate();
//                }

                // ���������, ��� ������ "����� ������������ � ����� ���� ������" ������������� � ������������, �������� �������������.
                xPlcConfigDataTransmit.xConfigData.ucErrorCode = CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE;
                printf("iPlcConfigurationCompare CONFIGURATION_ERROR_NEW_CONFIGURATION_AND_DATA_BASE: ui8FsmState %d\n\r", (ui8FsmState));
                fbPlcConfigurationError = 1;
                ui8FsmState = STOP_SIGNALING_CONFIRMATION_WAITING;
            }
            break;

        case CONFIGURATION_OK_NORMAL_START:
            printf("iPlcConfigurationCompare CONFIGURATION_OK_NORMAL_START: ui8FsmState %d\n\r", (ui8FsmState));
            // ��������� � ���������� ����� ������.
            return true;
            break;

        case UNCONDITIONAL_STOP_SIGNALING:
            printf("iPlcConfigurationCompare UNCONDITIONAL_STOP_SIGNALING: ui8FsmState %d\n\r", (ui8FsmState));
//            // ������ ������ ������������. ������������ � ��������� �������������.
            // ���� ������ ������������, �� ��������� ���������� ������� �������� � �����.
            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity =
                xPlcConfigData.xConfigData.ucModulesQuantity;
//            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity |= CONFIGURATION_ERROR;
            return false;
            break;

        case STOP_SIGNALING_CONFIRMATION_WAITING:
            printf("iPlcConfigurationCompare STOP_SIGNALING_CONFIRMATION_WAITING: ui8FsmState %d\n\r", (ui8FsmState));
            // ���� ������ ������������, �� ��������� ���������� ������� �������� � �����.
            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity =
                xPlcConfigData.xConfigData.ucModulesQuantity;
//            xPlcConfigDataTransmit.xConfigData.ucModulesQuantity |= CONFIGURATION_ERROR;
            return false;
            break;

        default:
            return false;
            break;
        };
    }
}

