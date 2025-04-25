//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include <typeinfo>

#include "Task.h"
#include "Resources.h"
#include "Platform.h"
#include "DataStore.h"
#include "DeviceControl.h"
#include "DataContainer.h"
#include "InternalModule.h"
#include "ConfigurationCreate.h"
#include "DataBaseCreate.h"

using namespace std;

//-------------------------------------------------------------------------------
CDataBaseCreate::CDataBaseCreate()
{
    std::cout << "CDataBaseCreate constructor"  << std::endl;
    m_puiIntermediateBuff = new uint8_t[256];
    SetFsmState(START);
}

//-------------------------------------------------------------------------------
CDataBaseCreate::~CDataBaseCreate()
{
    delete[] m_puiIntermediateBuff;
    delete[] GetResources() ->
    m_pxDiscreteSygnalTextDescriptorsWork;
}

//-------------------------------------------------------------------------------
void CDataBaseCreate::SetInternalModuleName(std::string sName)
{
    m_sInternalModuleName = sName;
}

//-------------------------------------------------------------------------------
void CDataBaseCreate::SetDeviceControlName(std::string sName)
{
    m_sDeviceControlName = sName;
}

//-------------------------------------------------------------------------------
uint8_t CDataBaseCreate::Init(void)
{
    std::cout << "CDataBaseCreate Init"  << std::endl;
    SetExecutorDataContainer(static_cast<CDataContainerDataBase*>(GetResources() ->
                             AddDataContainer(std::make_shared<CDataContainerDataBase>())));
    SetCustomerDataContainer(GetExecutorDataContainerPointer());

    GetResources() ->
    m_pxDiscreteSygnalTextDescriptorsWork = new TDiscreteSygnalTextDescriptor[MAX_HANDLED_DISCRETE_INPUT];
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ������ ������������.
void DimentionsParametersDataBaseCreate(uint8_t* puiBlockDataPointer)
{
    TDimentionParameterPackOne *pxDimentionParameterPackOne;
    TDimentionParameter *pxDimentionParameter;
    TDimentionParameter axDimentionsParametersDescriptionWork[DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY];

    memset(axDimentionsParametersDescriptionWork,
           0,
           sizeof(axDimentionsParametersDescriptionWork));

    pxDimentionParameter = axDimentionsParametersDescriptionWork;

    // �������� ��������� ���� ������ ������������.
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "����.�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "%");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "���");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "���");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "��/��2");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "���");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "��/���");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "1/�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "��/�2");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "�3/�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "��3/�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "��/�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "�/�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "�");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "������");
    pxDimentionParameter++;
    strcpy(pxDimentionParameter -> acDimentionParameterText,
           "������");

    // ������� ��������� �� ����� � ����� ��������� ��������� ����� ������ �������.
    pxDimentionParameter = axDimentionsParametersDescriptionWork;
    // ������� ��������� �� ��������� ����� � ������� ���������� ���� ���� ������
    // � ����� ������� ��������
    pxDimentionParameterPackOne = (TDimentionParameterPackOne*)puiBlockDataPointer;
    // ����������� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        // ��������� ���� ���������, � ����� ����� ���� ������ �������.
        memcpy(pxDimentionParameterPackOne[i].acDimentionParameterText,
               pxDimentionParameter[i].acDimentionParameterText,
               DIMENSIONS_PARAMETERS_NAME_LENGTH);
    }
}

//-----------------------------------------------------------------------------------------------------
void CDataBaseCreate::DiscreteSignalsTextTitlesCreateStart(void)
{
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor =
        (GetResources() -> GetDiscreteSygnalTextDescriptorsWorkPointer());

    memset((unsigned char*)pxDiscreteSygnalTextDescriptor,
           0,
           (sizeof(struct TDiscreteSygnalTextDescriptor) *
            MAX_HANDLED_DISCRETE_INPUT));
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ��������� ���������� ������� ����������� �����.
void CDataBaseCreate::DiscreteInputModuleDiscreteSignalsTextTitlesCreate(void)
{
    unsigned char nucDiscreteInputCounter;
    unsigned char nucModuleCounter;
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;
    // ����� � ������� �����.
    nucDiscreteInputCounter = 0;
    // ����� � ������� ������.
    nucModuleCounter = 1;

    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    uint16_t uiDiscreteSignalsNumber =
        ((pxDeviceConfigSearch ->
          uiServiceDiscreteInputModuleQuantity) *
         DISCRETE_MODULE_INPUT_QUANTITY);

    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
    pxDiscreteSygnalTextDescriptor =
        (GetResources() -> GetDiscreteSygnalTextDescriptorsWorkPointer());

    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    for (uint16_t i = 0;
            i < uiDiscreteSignalsNumber;
            i++)
    {
        // �������� ������ ���������� ��������� ����������� �������.
        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                "%s_%02d_%02d  ",
                "��. DI",
                nucModuleCounter,
                nucDiscreteInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
        // ��������� ���� ������ ����������� �����.
        nucDiscreteInputCounter++;
        // ��������� ���������.
        i++;
        // ���������� ��� ����� ������ ����������� �����?
        if (nucDiscreteInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
        {
            // ����� � ������� �����.
            nucDiscreteInputCounter = 0;
            // ��������� ������.
            nucModuleCounter++;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ��������� ���������� ������� ����������� �����.
void CDataBaseCreate::AnalogoueInputModuleDiscreteSignalsTextTitlesCreate(void)
{
    unsigned char nucModuleCounter;
    unsigned char nucModuleInputCounter;
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;
    unsigned char ucFlowControl;
    // ����� � ������� ������.
    nucModuleCounter = 1;
    // ����� � ������� �����.
    nucModuleInputCounter = 0;

    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    // ������� ���������� ���������� �������� ����������� �������� ����������� �����.
    // ����� �������� �������� � ������� ������� �� ����� ��������� ����������
    // ���������� ��������.
    uint16_t uiDiscreteSignalsNumber =
        ((pxDeviceConfigSearch ->
          uiServiceDiscreteInputModuleQuantity) *
         DISCRETE_MODULE_INPUT_QUANTITY);

    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
    // � ����� ��������� ���������� ��� ������� ����������� �����.
    pxDiscreteSygnalTextDescriptor =
        ((GetResources() -> GetDiscreteSygnalTextDescriptorsWorkPointer()) +
         (sizeof(struct TDiscreteSygnalTextDescriptor) *
          uiDiscreteSignalsNumber));

    // ������� ���������� ���������� �������� ����������� ����������� ���������.
    // ���� ���������� ������ ��������� ANALOGUE_INPUT_DI_VALUE_QUANTITY ����������.
    uiDiscreteSignalsNumber =
        ((pxDeviceConfigSearch ->
          uiServiceAnalogueInputQuantity) *
         ANALOGUE_INPUT_DI_VALUE_QUANTITY);

    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
    // ��� �������� ����������� �������� ����������� �����.
    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
    // ����� � �������� ��������� ��� ������� LL+L.
    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
    // ���� �� ���� ���������� �������� ���������� �������� ����������� �����.
    for (uint16_t i = 0;
            i < uiDiscreteSignalsNumber;
        )
    {
        // ���� �� ������ ������ ����������� ����� ����.
        for (uint8_t j = 0;
                j < ANALOG_MODULE_INPUT_QUANTITY;
                j++)
        {
            // ���� �� ������ ����������� ������� ������������ 4 ����������.
            // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
            for (uint8_t k = 0;
                    k < ANALOGUE_INPUT_DI_VALUE_QUANTITY;
                    k++)
            {
                switch(ucFlowControl)
                {
                case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
                    // �������� ������ ���������� ��������� ����������� �������.
                    sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                            "%s_%02d_%01d_LL",
                            "��. AI",
                            nucModuleCounter,
                            nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
                    // ������� � ���������� ��������� ��������� �������.
                    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
                    break;

                case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
                    // �������� ������ ���������� ��������� ����������� �������.
                    sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                            "%s_%02d_%01d_L ",
                            "��. AI",
                            nucModuleCounter,
                            nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

                    // ������� � ���������� ��������� ��������� �������.
                    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
                    break;

                case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
                    // �������� ������ ���������� ��������� ����������� �������.
                    sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                            "%s_%02d_%01d_H ",
                            "��. AI",
                            nucModuleCounter,
                            nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

                    // ������� � ���������� ��������� ��������� �������.
                    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
                    break;

                case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
                    // �������� ������ ���������� ��������� ����������� �������.
                    sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
                            "%s_%02d_%01d_HH",
                            "��. AI",
                            nucModuleCounter,
                            nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);

                    // ������� � ���������� ��������� ��������� �������.
                    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
                    break;

                default:
                    break;
                };

                // ��������� ���������.
                i++;
            }

            // ��������� ����.
            nucModuleInputCounter++;
        }

        // ����� � ������� �����.
        nucModuleInputCounter = 0;
        // ��������� ������.
        nucModuleCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ����������� ���� ���� ������ ��������� ���������� ���������� �������� �� �������� ������� � �����,
// � ������� �� �������� �� ������������� � ��������.
void CDataBaseCreate::DiscreteSignalsTextTitlesWorkToCommonFormat(uint8_t* puiBlockDataPointer)
{
    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;

    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
    pxDiscreteSygnalTextDescriptor =
        (GetResources() -> GetDiscreteSygnalTextDescriptorsWorkPointer());
    // ������� ��������� �� ��������� ����� � ������� ���������� ���� ���� ������
    // � ����� ������� ��������
    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)puiBlockDataPointer;

    CConfigurationCreate::TConfigDataPackOne* pxDeviceConfigSearch =
        (GetResources() -> GetDeviceConfigSearchPointer());

    // ����������� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < (pxDeviceConfigSearch ->
                 uiDiscreteInputSignalsQuantity);
            i++)
    {
        // ��������� ���� ���������, � ����� ����� ���� ������ �������.
        memcpy(pxDiscreteSygnalTextDescriptorPackOne[i].acTextDescriptor,
               pxDiscreteSygnalTextDescriptor[i].acTextDescriptor,
               DISCRETE_SYGNAL_NAME_LENGTH);
    }
}

////-----------------------------------------------------------------------------------------------------
//// ������ ��������� ���� ������ ��������� ���������� ���������� ��������.
//int iDiscreteSygnalTextDescriptorStartDataBaseCreate(void)
//{
//    int i;
//    unsigned char nucBlockCounter;
//    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
//    unsigned char nucDiscreteInputCounter;
//    unsigned char nucModuleInputCounter;
//    // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
//    unsigned char nucInputSetPointCounter;
//    unsigned char nucModuleCounter;
//    unsigned char ucFlowControl;
//    TDiscreteSygnalTextDescriptorPackOne *pxDiscreteSygnalTextDescriptorPackOne;
//    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
//    TDiscreteSygnalTextDescriptor *pxDiscreteSygnalTextDescriptor;
//    TDiscreteSygnalTextDescriptor axDiscreteSygnalTextDescriptor[MAX_HANDLED_DISCRETE_INPUT];
//
//
//    // ������� ��������� �� ����� ��� ��������������� ��������� ���� ������ �������.
//    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
//    // ����� � ������� �����.
//    nucDiscreteInputCounter = 0;
//    nucModuleInputCounter = 0;
//    // ����� � ������� ������.
//    nucModuleCounter = 1;
//    i = 0;
//
//    memset((unsigned char*)pxDiscreteSygnalTextDescriptor,
//           0,
//           (sizeof(struct TDiscreteSygnalTextDescriptor)));
//
////    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
////    // ��� �������� ����������� �������� ����������� �����.
////    while (i < (xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
////                DISCRETE_MODULE_INPUT_QUANTITY))
////    {
////        // �������� ������ ���������� ��������� ����������� �������.
////        sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
////                "%s_%02d_%02d  ",
////                "��. DI",
////                nucModuleCounter,
////                nucDiscreteInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////        // ��������� ���� ������ ����������� �����.
////        nucDiscreteInputCounter++;
////        // ��������� ���������.
////        i++;
////        // ���������� ��� ����� ������ ����������� �����?
////        if (nucDiscreteInputCounter == DISCRETE_MODULE_INPUT_QUANTITY)
////        {
////            // ����� � ������� �����.
////            nucDiscreteInputCounter = 0;
////            // ��������� ������.
////            nucModuleCounter++;
////        }
////    }
//
//    // ����� � ������� �����.
//    nucDiscreteInputCounter = 0;
//    nucModuleInputCounter = 0;
//    // ����� � ������ ������� - LL+L.
//    nucInputSetPointCounter = 0;
//    // ����� � ������� ������.
//    nucModuleCounter = 1;
//
////    // �������� ������ ����� ��������� ���� ������ ��������� ���������� ���������� ��������.
////    // ��� �������� ����������� �������� ����������� �����.
////    // ���� ���������� ���� ��������� ������ ���������� ������� - �������: LL, L, H, HH.
////    // ����� � �������� ��������� ��� ������� LL+L.
////    ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
////    while (i < ((xPlcConfigService.xPlcConfigServiceData.ucServiceDiscreteInputModuleQuantity *
////                 DISCRETE_MODULE_INPUT_QUANTITY) +
////                (xPlcConfigService.xPlcConfigServiceData.ucServiceAnalogueInputModuleQuantity *
////                 ANALOG_MODULE_INPUT_QUANTITY *
////                 ANALOGUE_INPUT_DI_VALUE_QUANTITY)))
////    {
////        switch(ucFlowControl)
////        {
////        case ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L:
////            // �������� ������ ���������� ��������� ����������� �������.
////            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
////                    "%s_%02d_%01d_LL",
////                    "��. AI",
////                    nucModuleCounter,
////                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////            // ������� � ���������� ��������� ��������� �������.
////            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_L;
////            break;
////
////        case ANALOGUE_INPUT_SET_POINT_VIOLATION_L:
////            // �������� ������ ���������� ��������� ����������� �������.
////            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
////                    "%s_%02d_%01d_L ",
////                    "��. AI",
////                    nucModuleCounter,
////                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////
////            // ������� � ���������� ��������� ��������� �������.
////            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_H;
////            break;
////
////        case ANALOGUE_INPUT_SET_POINT_VIOLATION_H:
////            // �������� ������ ���������� ��������� ����������� �������.
////            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
////                    "%s_%02d_%01d_H ",
////                    "��. AI",
////                    nucModuleCounter,
////                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////
////            // ������� � ���������� ��������� ��������� �������.
////            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H;
////            break;
////
////        case ANALOGUE_INPUT_SET_POINT_VIOLATION_HH_H:
////            // �������� ������ ���������� ��������� ����������� �������.
////            sprintf((char*)(pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
////                    "%s_%02d_%01d_HH",
////                    "��. AI",
////                    nucModuleCounter,
////                    nucModuleInputCounter + CONVERT_INTEGER_TO_NATURAL_NUMBER);
////
////            // ������� � ���������� ��������� ��������� �������.
////            ucFlowControl = ANALOGUE_INPUT_SET_POINT_VIOLATION_LL_L;
////            break;
////
////        default:
////            break;
////        };
////
////        nucDiscreteInputCounter++;
////        nucInputSetPointCounter++;
////        // ���������� ������� �� ������ �����(4 - LL, L, H, HH).
////        // ������ �� ���� �������� ������ �����?
////        if (nucInputSetPointCounter >= 4)
////        {
////            nucInputSetPointCounter = 0;
////            // ��������� ����.
////            nucModuleInputCounter++;
////        }
////        // ��������� ���������.
////        i++;
////
////        // ���������� ��� ���������� ������� ���������� ������� ������ ����������� �����?
////        if (nucDiscreteInputCounter == (ANALOG_MODULE_INPUT_QUANTITY *
////                                        ANALOGUE_INPUT_DI_VALUE_QUANTITY))
////        {
////            // ����� � ������� �����.
////            nucDiscreteInputCounter = 0;
////            nucModuleInputCounter = 0;
////            // ����� � ������ ������� - LL+L.
////            nucInputSetPointCounter = 0;
////            // ��������� ������.
////            nucModuleCounter++;
////        }
////    }
//
//    // ������� ��������� �� ����� � ����� ��������� ��������������� ��������� ����� ������ �������.
//    pxDiscreteSygnalTextDescriptor = axDiscreteSygnalTextDescriptor;
////    // ������� ��������� �� ���� ������ ������� � ����� �������.
////    pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
////                                            axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
////                                            aucPlcDataBaseBlockData[0]);
//    nucBlockCounter = 0;
//    nucBlocksInBlockCounter = 0;
//
//    // ����������� ��������� ���� ������ � ����� ������.
//    for (int i = 0;
//            i < MAX_HANDLED_DISCRETE_INPUT;
//            i++)
//    {
//        // ��������� ���� ���� ���� ������?
//        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
//        {
//            // ��������� ���� ���� ������.
//            nucBlockCounter++;
////            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
////            pxDiscreteSygnalTextDescriptorPackOne = (TDiscreteSygnalTextDescriptorPackOne*)&(xPlcDataBase.
////                                                    axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
////                                                    aucPlcDataBaseBlockData[0]);
//            nucBlocksInBlockCounter = 0;
//        }
//
//        // ��������� ���� ��������� ��������� ���������� ����������� �������, � ����� ������ �������.
//        memcpy((&xDiscreteSygnalTextDescriptorPackOne.
//                acTextDescriptor),
//               (&pxDiscreteSygnalTextDescriptor[i].acTextDescriptor),
//               DISCRETE_SYGNAL_NAME_LENGTH);
//
//        memcpy((uint8_t*)&pxDiscreteSygnalTextDescriptorPackOne[nucBlocksInBlockCounter],
//               (uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
//               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));
//
//        // ��������� ���������.
//        nucBlocksInBlockCounter++;
//    }
//}

//-------------------------------------------------------------------------------
uint8_t CDataBaseCreate::Fsm(void)
{
//    std::cout << "CDataBaseCreate::Fsm 1" << endl;
    switch (GetFsmState())
    {
    case IDDLE:
//        std::cout << "CDataBaseCreate::Fsm IDDLE"  << std::endl;
        break;

    case STOP:
//        //std::cout << "CDataBaseCreate::Fsm STOP"  << std::endl;
        break;

    case START:
        std::cout << "CDataBaseCreate::Fsm START"  << std::endl;
        Init();
        SetFsmState(INIT);
        break;

    case INIT:
        std::cout << "CDataBaseCreate::Fsm INIT 1"  << std::endl;
        m_uiInternalModuleId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sInternalModuleName);

        m_uiDeviceControlId =
            GetResources() ->
            GetTaskIdByNameFromMap(m_sDeviceControlName);

        SetFsmState(READY);

        break;

    case READY:
//        std::cout << "CDataBaseCreate::Fsm READY"  << std::endl;
        break;

    case DONE_OK:
//        std::cout << "CDataBaseCreate::Fsm DONE_OK"  << std::endl;
//        SetFsmOperationStatus(DONE_OK);
//        SetFsmState(READY);
        break;

    case DONE_ERROR:
//        std::cout << "CDataBaseCreate::Fsm DONE_ERROR"  << std::endl;
//        SetFsmOperationStatus(DONE_ERROR);
//        SetFsmState(READY);
        break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START:
        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING:
//        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 2"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 3"  << std::endl;
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case SUBTASK_EXECUTOR_READY_CHECK_START:
        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_START"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_WAITING);
        }
        break;

    case SUBTASK_EXECUTOR_READY_CHECK_WAITING:
//        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 1"  << std::endl;
    {
        if (SetTaskData(GetExecutorDataContainerPointer()))
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 2"  << std::endl;
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_START);
        }
        else
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 3"  << std::endl;
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_READY_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateReadyWaitingError());
            }
        }
    }
    break;

    case SUBTASK_EXECUTOR_DONE_CHECK_START:
        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_START 1"  << std::endl;
        {
            GetTimerPointer() -> Set(TASK_READY_WAITING_TIME);
            SetFsmState(SUBTASK_EXECUTOR_DONE_CHECK_WAITING);
        }

        break;

    case SUBTASK_EXECUTOR_DONE_CHECK_WAITING:
//        std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 1"  << std::endl;
    {
        CDataContainerDataBase* pxDataContainer =
            (CDataContainerDataBase*)GetExecutorDataContainerPointer();

        uint8_t uiFsmState = pxDataContainer -> m_uiFsmCommandState;

        if (uiFsmState == DONE_OK)
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 2"  << std::endl;
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
            SetFsmState(GetFsmNextStateDoneOk());
        }
        else if (uiFsmState == DONE_ERROR)
        {
            std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 3"  << std::endl;
            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
            SetFsmState(GetFsmNextStateDoneWaitingDoneError());
        }
        else
        {
            // ����� �������� ���������� ������� �����������?
            if (GetTimerPointer() -> IsOverflow())
            {
                std::cout << "CDataBaseCreate::Fsm SUBTASK_EXECUTOR_DONE_CHECK_WAITING 4"  << std::endl;
                ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_ERROR;
                SetFsmState(GetFsmNextStateDoneWaitingError());
            }
        }
    }
    break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_START"  << std::endl;
        {
            *(GetResources() -> GetDeviceConfigSearchPointer()) = {0};
        }
        SetFsmState(DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START);
        break;

//    case DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING:
//        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
//        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
//            uint16_t  uiLength = sizeof(auiTempData);
//            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
//                   auiTempData,
//                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_EXECUTOR_DONE_CHECK_WAITING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
//
//            SetFsmState(DONE_OK);
//        }
//        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SEARCH_MODULES_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
//            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
//            uint16_t  uiLength = sizeof(auiTempData);
////            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
////                   auiTempData,
////                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SEARCH_MODULES_EXECUTOR_ANSWER_PROCESSING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;

            SetFsmState(DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START);
        }
        break;

//-------------------------------------------------------------------------------
    case DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_START"  << std::endl;
        {
            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiInternalModuleId;
            pxDataContainer -> m_uiFsmCommandState =
                CInternalModule::SERVICE_DATA_CREATE_START;
            pxDataContainer -> m_puiDataPointer =
                (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer());

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
////            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////            uint16_t  uiLength = sizeof(auiTempData);
////            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
////                   auiTempData,
////                   uiLength);
//            uint16_t  uiLength = sizeof(struct TConfigDataPackOne);
//            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
//                   (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer()),
//                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm DATA_BASE_CREATE_INTERNAL_MODULES_SERVICE_DATA_CREATE_EXECUTOR_ANSWER_PROCESSING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case CONFIGURATION_REQUEST_START:
        std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_START"  << std::endl;
        {
            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_START);
            SetFsmNextStateDoneOk(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }

        SetFsmState(CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING);
        break;

    case CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
//            CDataContainerDataBase* pxExecutorDataContainer =
//                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
//            CDataContainerDataBase* pxCustomerDataContainer =
//                (CDataContainerDataBase*)GetCustomerDataContainerPointer();
//
////            uint8_t auiTempData[] = {1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 22, 4, 0,};
////            uint16_t  uiLength = sizeof(auiTempData);
////            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
////                   auiTempData,
////                   uiLength);
//            uint16_t  uiLength = sizeof(struct TConfigDataPackOne);
//            memcpy(pxExecutorDataContainer -> m_puiDataPointer,
//                   (uint8_t*)(GetResources() -> GetDeviceConfigSearchPointer()),
//                   uiLength);
//
//            pxExecutorDataContainer -> m_uiDataLength = uiLength;
//
//            memcpy(pxCustomerDataContainer -> m_puiDataPointer,
//                   (pxExecutorDataContainer -> m_puiDataPointer),
//                   pxExecutorDataContainer -> m_uiDataLength);
//            pxCustomerDataContainer -> m_uiDataLength =
//                pxExecutorDataContainer -> m_uiDataLength;
//
//            std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING uiLength "  << (int)uiLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxCustomerDataContainer -> m_uiDataLength "  << (int)pxCustomerDataContainer -> m_uiDataLength << std::endl;
//            std::cout << "CDataBaseCreate::Fsm CONFIGURATION_REQUEST_EXECUTOR_DONE_CHECK_WAITING pxExecutorDataContainer -> m_uiDataLength "  << (int)pxExecutorDataContainer -> m_uiDataLength << std::endl;
//
//            ((CDataContainerDataBase*)GetCustomerDataContainerPointer()) -> m_uiFsmCommandState = DONE_OK;
//            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case DIMENTIONS_PARAMETERS_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm DIMENTIONS_PARAMETERS_CREATE_START"  << std::endl;
        {
            DimentionsParametersDataBaseCreate(m_puiIntermediateBuff);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
            pxDataContainer -> m_uiFsmCommandState =
                CDeviceControl::DATA_BASE_BLOCK_START_WRITE;
            pxDataContainer -> m_uiDataIndex = DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START);
            SetFsmNextStateDoneOk(DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm DIMENTIONS_PARAMETERS_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    case TEXT_TITLES_CREATE_START:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_START"  << std::endl;
        {
            DiscreteSignalsTextTitlesCreateStart();
            SetFsmState(TEXT_TITLES_CREATE_DISCRETE_INPUT_MODULE_SIGNALS);
        }
        break;

    case TEXT_TITLES_CREATE_DISCRETE_INPUT_MODULE_SIGNALS:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_DISCRETE_INPUT_MODULE_SIGNALS"  << std::endl;
        {
            DiscreteInputModuleDiscreteSignalsTextTitlesCreate();
            SetFsmState(TEXT_TITLES_CREATE_ANALOGUE_INPUT_MODULE_SIGNALS);
        }
        break;

    case TEXT_TITLES_CREATE_ANALOGUE_INPUT_MODULE_SIGNALS:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_ANALOGUE_INPUT_MODULE_SIGNALS"  << std::endl;
        {
            AnalogoueInputModuleDiscreteSignalsTextTitlesCreate();
            SetFsmState(TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {
            DiscreteSignalsTextTitlesWorkToCommonFormat(m_puiIntermediateBuff);
//            memcpy(m_puiIntermediateBuff,
//                   (GetResources() -> GetDiscreteSygnalTextDescriptorsWorkPointer()),
//                   TEXT_TITLES_DATA_BASE_BLOCK_LENGTH);

            CDataContainerDataBase* pxDataContainer =
                (CDataContainerDataBase*)GetExecutorDataContainerPointer();
            pxDataContainer -> m_uiTaskId = m_uiDeviceControlId;
            pxDataContainer -> m_uiFsmCommandState =
                CDeviceControl::DATA_BASE_BLOCK_START_WRITE;
            pxDataContainer -> m_uiDataIndex = TEXT_TITLES_DATA_BASE_BLOCK_OFFSET;
            pxDataContainer -> m_puiDataPointer = m_puiIntermediateBuff;

            SetFsmState(SUBTASK_EXECUTOR_READY_CHECK_NO_DONE_CHECK_START);
            SetFsmNextStateDoneOk(TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING);
            SetFsmNextStateReadyWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingError(DONE_ERROR);
            SetFsmNextStateDoneWaitingDoneError(DONE_ERROR);
        }
        break;

    case TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_DISCRETE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START);
        }
        break;

    case TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_START"  << std::endl;
        {

        }
        break;

    case TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_ANALOGUE_SIGNALS_DATA_BASE_BLOCKS_WRITE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DONE_OK);
        }
        break;

    case TEXT_TITLES_CREATE_EXECUTOR_ANSWER_PROCESSING:
        std::cout << "CDataBaseCreate::Fsm TEXT_TITLES_CREATE_EXECUTOR_ANSWER_PROCESSING"  << std::endl;
        {
            SetFsmState(DONE_OK);
        }
        break;

//-------------------------------------------------------------------------------
    default:
        break;
    }
}

//-------------------------------------------------------------------------------

