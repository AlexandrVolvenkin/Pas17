
#include "PasNewConfig.h"
#include "Auxiliary.h"
#include "Task.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
float fFloatLsbToMsbByteOrderConverter(float fData)
{
    float fTempData;
    unsigned char *pucSource;
    unsigned char *pucDestination;

    pucSource = (unsigned char*)&fData;
    pucDestination = (unsigned char*)&fTempData;
    pucDestination[2] = pucSource[0];
    pucDestination[3] = pucSource[1];
    pucDestination[0] = pucSource[2];
    pucDestination[1] = pucSource[3];
//
//    pucDestination[0] = pucSource[3];
//    pucDestination[1] = pucSource[2];
//    pucDestination[2] = pucSource[1];
//    pucDestination[3] = pucSource[0];

    return fTempData;
}

//long swap(long i)
//{
//    char *c = (char *) &i;
//    return * (long *) (char[])
//    {
//        c[3], c[2], c[1], c[0]
//    };
//}

//float ReverseFloat( const float inFloat )
//{
//   float retVal;
//   char *floatToConvert = ( char* ) & inFloat;
//   char *returnFloat = ( char* ) & retVal;
//
//   // swap the bytes into a temporary buffer
//   returnFloat[0] = floatToConvert[3];
//   returnFloat[1] = floatToConvert[2];
//   returnFloat[2] = floatToConvert[1];
//   returnFloat[3] = floatToConvert[0];
//
//   return retVal;
//}

//-----------------------------------------------------------------------------------------------------
float fFloatMsbToLsbByteOrderConverter(float fData)
{
    float fTempData;
    unsigned char *pucSource;
    unsigned char *pucDestination;

    pucSource = (unsigned char*)&fData;
    pucDestination = (unsigned char*)&fTempData;
    pucDestination[0] = pucSource[2];
    pucDestination[1] = pucSource[3];
    pucDestination[2] = pucSource[0];
    pucDestination[3] = pucSource[1];
//
//    pucDestination[0] = pucSource[3];
//    pucDestination[1] = pucSource[2];
//    pucDestination[2] = pucSource[1];
//    pucDestination[3] = pucSource[0];

    return fTempData;
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ������ ��� �������� �� ����.
// � ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
void vLittleToBigEndianFloatConverter(unsigned char* pucDestination, unsigned char* pucSource, unsigned int nuiLength)
{
    int i;

    for (i = 0; i < nuiLength; i++)
    {
        pucDestination[2] = pucSource[0];
        pucDestination[3] = pucSource[1];
        pucDestination[0] = pucSource[2];
        pucDestination[1] = pucSource[3];
        pucSource += sizeof(float);
        pucDestination += sizeof(float);
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ������ �������� �� ����, �� ���������� �������.
// � ������� �������� ���������, ������������ LittleEndian. � ���� BigEndian.
void vBigToLittleEndianFloatConverter(unsigned char* pucDestination, unsigned char* pucSource, unsigned int nuiLength)
{
    int i;

    for (i = 0; i < nuiLength; i++)
    {
        pucDestination[0] = pucSource[2];
        pucDestination[1] = pucSource[3];
        pucDestination[2] = pucSource[0];
        pucDestination[3] = pucSource[1];
        pucSource += sizeof(float);
        pucDestination += sizeof(float);
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ������ � ����������� �����������, �� float IEEE754, � ������������� ��� 16 ���.
void vAnalogueInputToScalableCodeConverter(void)
{
    float fData;
    float fMinOfScale;
    float fMaxOfScale;
    float *fSource;
    unsigned short *pusDestination;
    TAnalogueInputDescriptionWork *pxAnalogueInputDescriptionWork;
    TAnalogueInputDescriptionWork xAnalogueInputDescriptionWork;

    pxAnalogueInputDescriptionWork =
        axAnalogueInputDescriptionWork;

    fSource = afAnalogueInputData;
    pusDestination = (unsigned short*)&aucInputRegistersArray[AI_VALUE_SHORT_CODE_BYTE_ARRAY_OFFSET];

    for (int i = 0; i < MAX_HANDLED_ANALOGUE_INPUT; i++)
    {
        // ���������� �������� ����������?
        if (aucCoilsArray[BAD_AI_BIT_ARRAY_OFFSET + i])
        {
            pusDestination[i] = ANALOGUE_INPUT_ERROR;
        }
        else
        {
            fMinOfScale = pxAnalogueInputDescriptionWork[i].fMinOfScale;
            fMaxOfScale = pxAnalogueInputDescriptionWork[i].fMaxOfScale;
            fData = fSource[i];
            pusDestination[i] = (unsigned short)(((float)MTVI5_MAX_CODE) *
                                                 ((fData - fMinOfScale) /
                                                  (fMaxOfScale - fMinOfScale)));
        }
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM, IEEE754.
// � ��������� ����������.
// �� ������ ��������� ���������� ����������� ����� ���������� ������ ���� �������,
// ������� ������ ����� � RAM ������ � ������ �������. �� ������, ������ � ����
// ������� ������ ���� �������� �� ��������� Modbus.
void vAnalogueInputDescriptionWorkExstract(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    float *pfDestination;
    TAnalogueInputDescriptionDataBasePackOne *pxSourse;
    TAnalogueInputDescriptionWork *pxDestination;

    pxDestination =
        axAnalogueInputDescriptionWork;
    pxSourse = (TAnalogueInputDescriptionDataBasePackOne*)(&xPlcDataBase.
               axPlcDataBaseBlocks[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET].
               aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_INPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxSourse = (TAnalogueInputDescriptionDataBasePackOne*)(&xPlcDataBase.
                       axPlcDataBaseBlocks[ANALOGUE_INPUT_MODULE_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                       aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        pxDestination[i].ucScaleType =
            (pxSourse[nucBlocksInBlockCounter].
             ucServiseL & ANALOGUE_INPUT_SCALE_TYPE_MASK);
        pxDestination[i].ucDimentionCode =
            ((pxSourse[nucBlocksInBlockCounter].
              ucServiseL & ANALOGUE_INPUT_DIMENTION_CODE_MASK) >> 4);
        pxDestination[i].ucRangeCode =
            (pxSourse[nucBlocksInBlockCounter].
             ucServiseH & ANALOGUE_INPUT_RANGE_CODE_MASK);

        pxDestination[i].fMinOfScale =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucMinOfScaleMantissLsb));
        pxDestination[i].fMaxOfScale =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucMaxOfScaleMantissLsb));
        pxDestination[i].fSetPointLL =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointLLMantissLsb));
        pxDestination[i].fSetPointL =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointLMantissLsb));
        pxDestination[i].fSetPointH =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointHMantissLsb));
        pxDestination[i].fSetPointHH =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucSetPointHHMantissLsb));

        memcpy((&pxDestination[i].acTextDescriptor),
               (&pxSourse[nucBlocksInBlockCounter].
                acTextDescriptor),
               ANALOGUE_INPUT_NAME_LENGTH);
        pxDestination[i].
        acTextDescriptor[ANALOGUE_INPUT_NAME_LENGTH] =
            END_OF_STRING;

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ���� ������ ���������� ����������� ����� �� ����������� �������� �������
// � ������ �������� ������(HMI ������, ������� SCADA, �������������).
// � ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
// � � ������� �������� ��������� ������������ ������������ �� 4 ����, � ���� ������������ �� 1 �����.
// ������������ ��� �������� ���� ������ ���������� ����������� ����� �� ������� �������(HMI ������, ������� SCADA, �������������).
void vAnalogueInputDescriptionWorkToModbus(void)
{
    TAnalogueInputDescriptionWork *pxSourse;
    TAnalogueInputDescriptionModbusPackOne *pxDestination;

    pxDestination =
        (TAnalogueInputDescriptionModbusPackOne*)(&aucHoldingRegistersArray[AI_VALUE_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET]);
    pxSourse =
        axAnalogueInputDescriptionWork;

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_INPUT;
            i++)
    {
        pxDestination[i].fMinOfScale =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fMinOfScale);
        pxDestination[i].fMaxOfScale =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fMaxOfScale);
        pxDestination[i].fSetPointLL =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fSetPointLL);
        pxDestination[i].fSetPointL =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fSetPointL);
        pxDestination[i].fSetPointH =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fSetPointH);
        pxDestination[i].fSetPointHH =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fSetPointHH);
    }
}

//-----------------------------------------------------------------------------------------------------
// ������ ��������� ���� ������ ������������.
int iDimentionsParametersDataBaseCreate(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    TDimentionParameterPackOne *pxDimentionParameterPackOne;
    TDimentionParameterPackOne xDimentionParameterPackOne;
    TDimentionParameter *pxDimentionParameter;

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
    // ������� ��������� �� ���� ������ ������� � ����� �������.
    pxDimentionParameterPackOne = (TDimentionParameterPackOne*)&(xPlcDataBase.
                                  axPlcDataBaseBlocks[DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET].
                                  aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // ����������� ��������� ���� ������ � ����� ������.
    for (int i = 0;
            i < DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            // ������� ��������� �� ��������� ���� � ����� ���� ������ �������.
            pxDimentionParameterPackOne = (TDimentionParameterPackOne*)&(xPlcDataBase.
                                          axPlcDataBaseBlocks[DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                          aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }
        // ��������� ���� ���������, � ����� ����� ���� ������ �������.
        memcpy(xDimentionParameterPackOne.acDimentionParameterText,
               pxDimentionParameter[i].acDimentionParameterText,
               DIMENSIONS_PARAMETERS_NAME_LENGTH);

        memcpy((uint8_t*)&pxDimentionParameterPackOne[nucBlocksInBlockCounter],
               (uint8_t*)&xDimentionParameterPackOne,
               sizeof(struct TDimentionParameterPackOne));
        // ��������� ���������.
        nucBlocksInBlockCounter++;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ���� ������ ������������ �� ������ ������� ���� ������, � ������ �������� � RAM.
void vDimentionsParametersDataBaseExstract(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    float *pfDestination;
    TDimentionParameterPackOne *pxDimentionParameterPackOne;
    TDimentionParameterPackOne xDimentionParameterPackOne;
    TDimentionParameter *pxDimentionParameter;

    pxDimentionParameter =
        axDimentionsParametersDescriptionWork;
    pxDimentionParameterPackOne = (TDimentionParameterPackOne*)(&xPlcDataBase.
                                  axPlcDataBaseBlocks[DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET].
                                  aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    for (int i = 0;
            i < DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == DIMENSIONS_PARAMETERS_DATA_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxDimentionParameterPackOne = (TDimentionParameterPackOne*)(&xPlcDataBase.
                                          axPlcDataBaseBlocks[DIMENSIONS_PARAMETERS_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                          aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        memcpy((uint8_t*)&xDimentionParameterPackOne,
               (uint8_t*)&pxDimentionParameterPackOne[nucBlocksInBlockCounter],
               sizeof(struct TDimentionParameterPackOne));

        memcpy((&pxDimentionParameter[i].acDimentionParameterText),
               (&xDimentionParameterPackOne.
                acDimentionParameterText),
               DIMENSIONS_PARAMETERS_NAME_LENGTH);
        pxDimentionParameter[i].
        acDimentionParameterText[DIMENSIONS_PARAMETERS_NAME_LENGTH] =
            END_OF_STRING;
        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM, IEEE754.
// ��������� ���������� ����������� ������ ������ ����� � ������ ������� � RAM. ������ � ����
// ������� ������ ���� �������� �� ��������� Modbus.

void vAnalogueOutputDescriptionWorkExstract(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    float *pfDestination;
    TAnalogueOutputRegulatorDescriptionDataBasePackOne *pxSourse;
    TAnalogueOutputRegulatorDescriptionWork *pxDestination;

    pxDestination =
        axAnalogueOutputRegulatorDescriptionWork;
    pxSourse = (TAnalogueOutputRegulatorDescriptionDataBasePackOne*)(&xPlcDataBase.
               axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET].
               aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_OUTPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxSourse = (TAnalogueOutputRegulatorDescriptionDataBasePackOne*)(&xPlcDataBase.
                       axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                       aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        pxDestination[i].ucPvAddress =
            pxSourse[nucBlocksInBlockCounter].
            ucPvAddress;
        pxDestination[i].ucRegulatorType =
            pxSourse[nucBlocksInBlockCounter].
            ucRegulatorType;
        pxDestination[i].ucOutAddress =
            pxSourse[nucBlocksInBlockCounter].
            ucOutAddress;
        pxDestination[i].ucOutInverse =
            pxSourse[nucBlocksInBlockCounter].
            ucOutInverse;
        pxDestination[i].ucControl =
            pxSourse[nucBlocksInBlockCounter].
            ucControl;
        pxDestination[i].ucProgrammSPAddress =
            pxSourse[nucBlocksInBlockCounter].
            ucProgrammSPAddress;
        pxDestination[i].fKp =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucKpMantissLsb));

        pxDestination[i].fTi =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucTiMantissLsb));
        pxDestination[i].fTd =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucTdMantissLsb));
        pxDestination[i].fOutRestrictionLow =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucOutRestrictionLowMantissLsb));
        pxDestination[i].fOutRestrictionHigh =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucOutRestrictionHighMantissLsb));
        pxDestination[i].fPreferredOutputValue =
            (fStep5ToFloat(&pxSourse[nucBlocksInBlockCounter].
                           ucPreferredOutputValueMantissLsb));

        pxDestination[i].ucForceControlFcOnAddress =
            pxSourse[nucBlocksInBlockCounter].
            ucForceControlFcOnAddress;
        pxDestination[i].ucForceControlHaOnAddress =
            pxSourse[nucBlocksInBlockCounter].
            ucForceControlHaOnAddress;
        pxDestination[i].ucForceControlSpOutAddress =
            pxSourse[nucBlocksInBlockCounter].
            ucForceControlSpOutAddress;

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ���� ������ �������� �� ���� �� ������� �������� � RAM IEEE754, � ����� ������ ���� ������.
// (� ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
// � � ������� �������� ��������� ������������ ������������ �� 4 ����, � ���� ������������ �� 1 �����).
// ������������ ��� ��������� ���� ������ � �������� ������(HMI ������, ������� SCADA, �������������).
void vAnalogueOutputDataBasePack(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    TAnalogueOutputRegulatorDescriptionModbusPackOne *pxSourse;
    TAnalogueOutputRegulatorDescriptionDataBasePackOne *pxDestination;

    pxSourse =
        (TAnalogueOutputRegulatorDescriptionModbusPackOne*)
        (&aucHoldingRegistersArray[CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET]);
    pxDestination =
        (TAnalogueOutputRegulatorDescriptionDataBasePackOne*)(&xPlcDataBase.
                axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET].
                aucPlcDataBaseBlockData[0]);

    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;
    // ����� ��������� � ���� ���� ������ �� ������� �������� � RAM IEEE754, � ����� ������ ���� ������.
    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_OUTPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxDestination = (TAnalogueOutputRegulatorDescriptionDataBasePackOne*)(&xPlcDataBase.
                            axPlcDataBaseBlocks[CURRENT_OUTPUT_MODULE_REGULATOR_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                            aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        vFloatToStep5(&pxDestination[nucBlocksInBlockCounter].
                      ucKpMantissLsb,
                      fFloatMsbToLsbByteOrderConverter((float)pxSourse[i].fKp));
        vFloatToStep5(&pxDestination[nucBlocksInBlockCounter].
                      ucTiMantissLsb,
                      fFloatMsbToLsbByteOrderConverter((float)pxSourse[i].fTi));
        vFloatToStep5(&pxDestination[nucBlocksInBlockCounter].
                      ucTdMantissLsb,
                      fFloatMsbToLsbByteOrderConverter((float)pxSourse[i].fTd));
        vFloatToStep5(&pxDestination[nucBlocksInBlockCounter].
                      ucOutRestrictionLowMantissLsb,
                      fFloatMsbToLsbByteOrderConverter((float)pxSourse[i].fOutRestrictionLow));
        vFloatToStep5(&pxDestination[nucBlocksInBlockCounter].
                      ucOutRestrictionHighMantissLsb,
                      fFloatMsbToLsbByteOrderConverter((float)pxSourse[i].fOutRestrictionHigh));
        vFloatToStep5(&pxDestination[nucBlocksInBlockCounter].
                      ucPreferredOutputValueMantissLsb,
                      fFloatMsbToLsbByteOrderConverter((float)pxSourse[i].fPreferredOutputValue));

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ��������� ���������� ����������� �������� � �������� ������(HMI ������, ������� SCADA, �������������)
// �� ���������� ������� ������(� ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
// � � ������� �������� ��������� ������������ ������������ �� 4 ����, � ���� ������������ �� 1 �����).
// ������������ ��� ��������� ���������� ���������� ����������� � �������� ������(HMI ������, ������� SCADA, �������������).
void vAnalogueOutputDescriptionModbusToWork(void)
{
    TAnalogueOutputRegulatorDescriptionModbusPackOne *pxSourse;
    TAnalogueOutputRegulatorDescriptionWork *pxDestination;

    pxDestination =
        axAnalogueOutputRegulatorDescriptionWork;
    pxSourse =
        (TAnalogueOutputRegulatorDescriptionModbusPackOne*)
        (&aucHoldingRegistersArray[CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET]);

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_OUTPUT;
            i++)
    {
        pxDestination[i].fKp =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fKp);
        pxDestination[i].fTi =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fTi);
        pxDestination[i].fTd =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fTd);
        pxDestination[i].fOutRestrictionLow =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fOutRestrictionLow);
        pxDestination[i].fOutRestrictionHigh =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fOutRestrictionHigh);
        pxDestination[i].fPreferredOutputValue =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fPreferredOutputValue);
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ��������� ���������� ����������� �� ����������� �������� �������
// � ������ �������� ������(HMI ������, ������� SCADA, �������������).
// � ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
// � � ������� �������� ��������� ������������ ������������ �� 4 ����, � ���� ������������ �� 1 �����.
// ������������ ��� �������� ���������� ���������� ����������� �� ������� �������(HMI ������, ������� SCADA, �������������).
void vAnalogueOutputDescriptionWorkToModbus(void)
{
    TAnalogueOutputRegulatorDescriptionWork *pxSourse;
    TAnalogueOutputRegulatorDescriptionModbusPackOne *pxDestination;

    pxDestination =
        (TAnalogueOutputRegulatorDescriptionModbusPackOne*)
        (&aucHoldingRegistersArray[CURRENT_OUTPUT_DATA_BASE_BIG_ENDIAN_BYTE_ARRAY_OFFSET]);
    pxSourse =
        axAnalogueOutputRegulatorDescriptionWork;

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_OUTPUT;
            i++)
    {
        pxDestination[i].fKp =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fKp);
        pxDestination[i].fTi =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fTi);
        pxDestination[i].fTd =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fTd);
        pxDestination[i].fOutRestrictionLow =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fOutRestrictionLow);
        pxDestination[i].fOutRestrictionHigh =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fOutRestrictionHigh);
        pxDestination[i].fPreferredOutputValue =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fPreferredOutputValue);
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ���������� ������� ����������� �������� � �������� ������(HMI ������, ������� SCADA, �������������)
// �� ���������� ������� ������(� ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
// � � ������� �������� ��������� ������������ ������������ �� 4 ����, � ���� ������������ �� 1 �����).
// ������������ ��� ��������� ���������� �������� ����������� � �������� ������(HMI ������, ������� SCADA, �������������).
void vAnalogueOutputContAvModbusToWork(void)
{
    TMtvi5ContAvDataPackOne *pxSourse;
    TMtvi5ContAvData *pxDestination;

    pxDestination =
        axAnalogueOutputRegulatorContAv;
    pxSourse =
        (TMtvi5ContAvDataPackOne*)(&aucHoldingRegistersArray[CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET]);

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_OUTPUT;
            i++)
    {
        pxDestination[i].fSp =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fSp);
        pxDestination[i].fOut =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fOut);
        pxDestination[i].fPv =
            fFloatMsbToLsbByteOrderConverter(pxSourse[i].fPv);
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� ���������� ������� ����������� �� ����������� �������� �������
// � �������� � �������� ������(HMI ������, ������� SCADA, �������������).
// � ������� �������� ��������� ������������ LittleEndian, � ���� BigEndian.
// � � ������� �������� ��������� ������������ ������������ �� 4 ����, � ���� ������������ �� 1 �����.
// ������������ ��� �������� ���������� �������� ����������� �� ������� �������(HMI ������, ������� SCADA, �������������).
void vAnalogueOutputContAvWorkToModbus(void)
{
    TMtvi5ContAvData *pxSourse;
    TMtvi5ContAvDataPackOne *pxDestination;

    pxDestination =
        (TMtvi5ContAvDataPackOne*)(&aucHoldingRegistersArray[CONT_AV_BIG_ENDIAN_BYTE_ARRAY_OFFSET]);
    pxSourse =
        axAnalogueOutputRegulatorContAv;

    for (int i = 0;
            i < MAX_HANDLED_ANALOGUE_OUTPUT;
            i++)
    {
        pxDestination[i].fSp =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fSp);
        pxDestination[i].fOut =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fOut);
        pxDestination[i].fPv =
            fFloatLsbToMsbByteOrderConverter(pxSourse[i].fPv);
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM.
// � ���� ��������������� ������ ���������� ������� Modbus-Master.
void vModbusMasterModuleDataBaseExstract(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    TMbmFunction1234PackOne *pxSourse;
    TMbmFunction1234PackOne xMbmFunction1234PackOne;
    TMbmFunction1234 *pxDestination;

    pxDestination =
        axModbusMasterFunction1234DescriptionWork;
    pxSourse = (TMbmFunction1234PackOne*)(&xPlcDataBase.
                                          axPlcDataBaseBlocks[MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET].
                                          aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    for (int i = 0;
            i < EXTERNAL_MODULE_FUNCTION_1234_QUANTITY;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxSourse = (TMbmFunction1234PackOne*)(&xPlcDataBase.
                                                  axPlcDataBaseBlocks[MBM_FUNCTION_1234_BLOCK_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                                                  aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        memcpy((uint8_t*)&xMbmFunction1234PackOne,
               (uint8_t*)&pxSourse[nucBlocksInBlockCounter],
               sizeof(struct TMbmFunction1234PackOne));

        pxDestination[i].ucModbusFunction =
            xMbmFunction1234PackOne.ucModbusFunction;
        pxDestination[i].ucModbusSlaveAddress =
            xMbmFunction1234PackOne.ucModbusSlaveAddress;
        pxDestination[i].ucAddressH =
            xMbmFunction1234PackOne.ucAddressH;
        pxDestination[i].ucAddressL =
            xMbmFunction1234PackOne.ucAddressL;
        pxDestination[i].ucQuantityH =
            xMbmFunction1234PackOne.ucQuantityH;
        pxDestination[i].ucQuantityL =
            xMbmFunction1234PackOne.ucQuantityL;
        pxDestination[i].ucTust =
            xMbmFunction1234PackOne.ucTust;
        pxDestination[i].ucIOQuantity =
            xMbmFunction1234PackOne.ucIOQuantity;
        pxDestination[i].ui16DiValueArrayBegin =
            xMbmFunction1234PackOne.ui16DiValueArrayBegin;
        pxDestination[i].ucAiValueArrayBegin =
            xMbmFunction1234PackOne.ucAiValueArrayBegin;
        pxDestination[i].ucMaxCodeL =
            xMbmFunction1234PackOne.ucMaxCodeL;
        pxDestination[i].ucMaxCodeH =
            xMbmFunction1234PackOne.ucMaxCodeH;

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������������� �� ������ ������� ���� ������, � ������ �������� � RAM.
// � ���� ��������������� ������ ���������� ��������� ���������� ���������� ��������.
void vDiscreteSygnalTextDescriptorDataBaseExstract(void)
{
    unsigned char nucBlockCounter;
    unsigned char nucBlocksInBlockCounter; // ���� ��������� - ���� ����, � ����� �����.
    TDiscreteSygnalTextDescriptorPackOne *pxSourse;
    TDiscreteSygnalTextDescriptorPackOne xDiscreteSygnalTextDescriptorPackOne;
    TDiscreteSygnalTextDescriptor *pxDestination;

    pxDestination =
        axDiscreteSygnalTextDescriptor;
    pxSourse = (TDiscreteSygnalTextDescriptorPackOne*)(&xPlcDataBase.
               axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET].
               aucPlcDataBaseBlockData[0]);
    nucBlockCounter = 0;
    nucBlocksInBlockCounter = 0;

    for (int i = 0;
            i < MAX_HANDLED_DISCRETE_INPUT;
            i++)
    {
        // ��������� ���� ���� ���� ������?
        if (nucBlocksInBlockCounter == TEXT_TITLES_DATA_BASE_BLOCKS_IN_BLOCK_QUANTITY)
        {
            // ��������� ���� ���� ������.
            nucBlockCounter++;
            pxSourse = (TDiscreteSygnalTextDescriptorPackOne*)(&xPlcDataBase.
                       axPlcDataBaseBlocks[TEXT_TITLES_DATA_BASE_BLOCK_OFFSET + nucBlockCounter].
                       aucPlcDataBaseBlockData[0]);
            nucBlocksInBlockCounter = 0;
        }

        memcpy((uint8_t*)&xDiscreteSygnalTextDescriptorPackOne,
               (uint8_t*)&pxSourse[nucBlocksInBlockCounter],
               sizeof(struct TDiscreteSygnalTextDescriptorPackOne));

        memcpy((&pxDestination[i].acTextDescriptor),
               (&xDiscreteSygnalTextDescriptorPackOne.
                acTextDescriptor),
               DISCRETE_SYGNAL_NAME_LENGTH);
        pxDestination[i].
        acTextDescriptor[DISCRETE_SYGNAL_NAME_LENGTH] =
            END_OF_STRING;

        nucBlocksInBlockCounter++;
    }
}

//-----------------------------------------------------------------------------------------------------
// �����������, ��������������� ������ ������������ �� ����������� ������� � ������ �������� �� ����������� �����.
void vConfigurationDataPack(TConfigDataPackOne *pxDestination, TConfigData *pxSourse)
{
    TConfigDataPackOne xConfigDataPackOne;
    TConfigData xConfigData;

    memcpy((uint8_t*)&xConfigData,
           (uint8_t*)pxSourse,
           sizeof(struct TConfigData));

    xConfigDataPackOne.ucModulesQuantity =
        xConfigData.ucModulesQuantity;

    memcpy((uint8_t*)&xConfigDataPackOne.aucModuleType,
           (uint8_t*)&xConfigData.aucModuleType,
           INTERNAL_MODULE_QUANTITY);

    xConfigDataPackOne.ucLastDiscreteInputModuleAddressPlusOne =
        xConfigData.ucLastDiscreteInputModuleAddressPlusOne;
    xConfigDataPackOne.ucLastAnalogueInputModuleAddressPlusOne =
        xConfigData.ucLastAnalogueInputModuleAddressPlusOne;
    xConfigDataPackOne.ucDiscreteOutputQuantity =
        xConfigData.ucDiscreteOutputQuantity;
    xConfigDataPackOne.ui16DiscreteInputQuantity =
        xConfigData.ui16DiscreteInputQuantity;
    xConfigDataPackOne.ucAnalogueInputQuantity =
        xConfigData.ucAnalogueInputQuantity;
    xConfigDataPackOne.ucDisplayType =
        xConfigData.ucDisplayType;
    xConfigDataPackOne.ucErrorCode =
        xConfigData.ucErrorCode;

    memcpy((uint8_t*)pxDestination,
           (uint8_t*)&xConfigDataPackOne,
           sizeof(struct TConfigDataPackOne));
}

//-----------------------------------------------------------------------------------------------------
// �����������, ��������������� ������ ������������ �� ����������� ������� � ������ �������� �� ����������� �����.
void vConfigurationModbusRepresentDataPack(TConfigDataModbusRepresentPackOne *pxDestination, TConfigData *pxSourse)
{
//    TConfigDataPackOne xConfigDataPackOne;
//    TConfigData xConfigData;
//
//    memcpy((uint8_t*)&xConfigData,
//           (uint8_t*)pxSourse,
//           sizeof(struct TConfigData));

    pxDestination -> ui16ModulesQuantity =
        pxSourse -> ucModulesQuantity;

    for (int i = 0;
            (i < INTERNAL_MODULE_QUANTITY);
            i++)
    {
        pxDestination -> aui16ModuleType[i] =
            pxSourse -> aucModuleType[i];
    }

    pxDestination -> ui16LastDiscreteInputModuleAddressPlusOne =
        pxSourse -> ucLastDiscreteInputModuleAddressPlusOne;
    pxDestination -> ui16LastAnalogueInputModuleAddressPlusOne =
        pxSourse -> ucLastAnalogueInputModuleAddressPlusOne;
    pxDestination -> ui16DiscreteOutputQuantity =
        pxSourse -> ucDiscreteOutputQuantity;
    pxDestination -> ui16DiscreteInputQuantity =
        pxSourse -> ui16DiscreteInputQuantity;
    pxDestination -> ui16AnalogueInputQuantity =
        pxSourse -> ucAnalogueInputQuantity;
    pxDestination -> ui16DisplayType =
        pxSourse -> ucDisplayType;
    pxDestination -> ui16ErrorCode =
        pxSourse -> ucErrorCode;

//    memcpy((uint8_t*)pxDestination,
//           (uint8_t*)&xConfigDataPackOne,
//           sizeof(struct TConfigDataPackOne));
}

//-----------------------------------------------------------------------------------------------------
void vByteToBitArray(unsigned char *pucDestination, unsigned char *pucSource, int iLength)
{
    int i;
    int j;

    for (i = 0; i < iLength; i++)
    {
        for (j = 0; j < BITS_IN_BYTE_QUANTITY; j++)
        {
            if (((*pucSource) >> j) & 0x01)
            {
                pucDestination[j] = 1;
            }
            else
            {
                pucDestination[j] = 0;
            }
        }
        pucSource++;
        pucDestination += BITS_IN_BYTE_QUANTITY;
    }
}

//-----------------------------------------------------------------------------------------------------
void vBitToByteArray(unsigned char *pucDestination, unsigned char *pucSource, int iLength)
{
    int i;
    int j;

    for (i = 0; i < iLength; i++)
    {
        for (j = 0; j < BITS_IN_BYTE_QUANTITY; j++)
        {
            if (*pucSource)
            {
                *pucDestination |= (0x01 << j);
            }
            else
            {
                *pucDestination &= ~(0x01 << j);
            }
            pucSource++;
        }
        pucDestination++;
    }
}

//-----------------------------------------------------------------------------------------------------
void vByteToShortArray(unsigned short *pusDestination, unsigned char *pucSource, int iLength)
{
    int i;

    iLength /= sizeof(short);
    for (i = 0; i < (iLength); i++)
    {
        pusDestination[i] = (unsigned short)pucSource[i];
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����� �� ����. ���� ����� �������� ������, �� ���������� TRUE.
int iBufferIsZero(unsigned char *pucSource, int iLength)
{
    int i;
    bool bWasNonZero;

    bWasNonZero = 0;
    for (i = 0; i < (iLength); i++)
    {
        if (pucSource[i])
        {
            bWasNonZero = 1;
        }
    }

    if (bWasNonZero)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����������� ����� ������ ���� ������ �
// �������� � � ����� ������(PLC_DATA_BASE_BLOCK_CRC_OFFSET).
void vDataBaseBlockCrcCreate(void)
{
    unsigned short usCrc;
    int i;
    TPlcDataBaseBlock *pxPlcDataBaseBlockData;
    // ������� ��������� �� ������ ���� � ���� ������.
    pxPlcDataBaseBlockData = (TPlcDataBaseBlock*)(&xPlcDataBase.
                             axPlcDataBaseBlocks[0]);

    // �������� ����������� ����� ���� ������ ���� ������.
    for (i = 0;
            i < PLC_DATA_BASE_BLOCK_QUANTITY;
            i++)
    {
        // �������� ����������� ����� ����� ���� ������.
        usCrc = usCrc16(&pxPlcDataBaseBlockData[i].
                        aucPlcDataBaseBlockData[0],
                        (PLC_DATA_BASE_BLOCK_LENGTH -
                         TWO_BYTE_CRC_LENGTH));
        // �������� � � ����� �����(PLC_DATA_BASE_BLOCK_CRC_OFFSET).
        pxPlcDataBaseBlockData[i].
        aucPlcDataBaseBlockData[PLC_DATA_BASE_BLOCK_CRC_OFFSET] =
            (unsigned char)(usCrc >> 8);
        pxPlcDataBaseBlockData[i].
        aucPlcDataBaseBlockData[PLC_DATA_BASE_BLOCK_CRC_OFFSET + 1] =
            (unsigned char)(usCrc);
    }
    // �������� ����������� ����� ���� ���� ������.
    xPlcDataBase.usCrc =
        usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                (sizeof(xPlcDataBase.axPlcDataBaseBlocks)));
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����������� ����� ����� ���� ������ �
// �������� � � ����� �����(PLC_DATA_BASE_BLOCK_CRC_OFFSET).
void vDataBaseOneBlockCrcCreate(TPlcDataBaseBlock *pxSource)
{
    unsigned short usCrc;
    // �������� ����������� ����� ����� ���� ������.
    usCrc = usCrc16(&(pxSource ->
                      aucPlcDataBaseBlockData[0]),
                    (PLC_DATA_BASE_BLOCK_LENGTH -
                     TWO_BYTE_CRC_LENGTH));
    // �������� � � ����� �����(PLC_DATA_BASE_BLOCK_CRC_OFFSET).
    pxSource ->
    aucPlcDataBaseBlockData[PLC_DATA_BASE_BLOCK_CRC_OFFSET] =
        (unsigned char)(usCrc >> 8);
    pxSource ->
    aucPlcDataBaseBlockData[PLC_DATA_BASE_BLOCK_CRC_OFFSET + 1] =
        (unsigned char)(usCrc);
    // �������� ����������� ����� ���� ���� ������.
    xPlcDataBase.usCrc =
        usCrc16((unsigned char*)xPlcDataBase.axPlcDataBaseBlocks,
                (sizeof(xPlcDataBase.axPlcDataBaseBlocks)));
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����������� ����� ������������� ��������� �������.
void vModulesContextStaticCrcCreate(TAllModulesContext *pxSource)
{
    uint8_t aui8ModuleContextStatic[(sizeof(struct TModuleContextStatic) * MAX_MODULE_QUANTITY)];
    uint8_t *pui8ModuleContextStatic;

    pui8ModuleContextStatic = aui8ModuleContextStatic;
    for (int i = 0;
            (i < MAX_MODULE_QUANTITY);
            i++)
    {
        // ��������� ������������ �������� ������ ������.
        memcpy(pui8ModuleContextStatic,
               (unsigned char*)&(pxSource -> axAllModulesContext[i].xModuleContextStatic),
               (sizeof(struct TModuleContextStatic)));
        pui8ModuleContextStatic += sizeof(struct TModuleContextStatic);
    }

    // �������� ����������� ����� ������������� ��������� �������.
//    pxSource -> usCrc =
//        usCrc16(aui8ModuleContextStatic,
//                sizeof(aui8ModuleContextStatic));
    pxSource -> usCrc =
        usCrcSummTwoByteCalculation(aui8ModuleContextStatic,
                                    sizeof(aui8ModuleContextStatic));
}

//-----------------------------------------------------------------------------------------------------
// ��������� ����������� ����� ������������� ��������� �������.
int iModulesContextStaticCrcCheck(TAllModulesContext *pxSource)
{
    uint8_t aui8ModuleContextStatic[(sizeof(struct TModuleContextStatic) * MAX_MODULE_QUANTITY)];
    uint8_t *pui8ModuleContextStatic;

    pui8ModuleContextStatic = aui8ModuleContextStatic;
    for (int i = 0;
            (i < MAX_MODULE_QUANTITY);
            i++)
    {
        // ��������� ������������ �������� ������ ������.
        memcpy(pui8ModuleContextStatic,
               (unsigned char*)&(pxSource -> axAllModulesContext[i].xModuleContextStatic),
               (sizeof(struct TModuleContextStatic)));
        pui8ModuleContextStatic += sizeof(struct TModuleContextStatic);
    }

    // �������� ����������� ����� ������������� ��������� �������.
//    if ((pxSource -> usCrc) ==
//            usCrc16(aui8ModuleContextStatic,
//                    sizeof(aui8ModuleContextStatic)))
//    {
//        return 0;
//    }
//    else
//    {
//        return -1;
//    }

    if ((pxSource -> usCrc) ==
            usCrcSummTwoByteCalculation(aui8ModuleContextStatic,
                                        sizeof(aui8ModuleContextStatic)))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void vWorkArraysUpdate(void)
{
//    // ����� �������� ��������� � ���� ������, ��� ������� ������� � ������ ������ ����� ��������.
//    CSettingsRW::PlcSettingsLoad();
    vAnalogueInputDescriptionWorkExstract();
    vAnalogueInputDescriptionWorkToModbus();
    vDimentionsParametersDataBaseExstract();
    vModbusMasterModuleDataBaseExstract();
    vAnalogueOutputDescriptionWorkExstract();
    vAnalogueOutputDescriptionWorkToModbus();
    vDiscreteInputDataBaseExstract();
    vDiscreteSygnalTextDescriptorDataBaseExstract();
    vFunctionBlockDataBaseExstract();

    // �������� ����������� �����. ����� ����� ��������� ����������� ����� ������ ������� ������.
    xCCrc.CreateAll();
}

//-----------------------------------------------------------------------------------------------------
// �������� ��������� ����� �� Linux � �������� � aucInputRegistersArray[CURRENT_TIME_BYTE_ARRAY_OFFSET].
void vCurrentTimeUpdate(void)
{
    time_t rawtime;
    rawtime = time(NULL);
    struct tm *timeinfo;
    unsigned short *pusDestination;

    // ���� �������� �����?
    if ((ui8CurrentTimeUpdateDelayCounter) == 0)
    {
        // ���������� ����� N ������� ������� ������ �� 100mS.
        ui8CurrentTimeUpdateDelayCounter = 3;

        // ������� ��������� �� ����� � ������� �������� � ������� ������� �������.
        pusDestination =
            (unsigned short*)&aucInputRegistersArray[CURRENT_TIME_BYTE_ARRAY_OFFSET];

        rawtime = time(NULL);
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        xCurrentTime.tm_sec = timeinfo -> tm_sec;
        xCurrentTime.tm_min = timeinfo -> tm_min;
        xCurrentTime.tm_hour = timeinfo -> tm_hour;
        xCurrentTime.tm_mday = timeinfo -> tm_mday;
//    xCurrentTime.tm_mon = (timeinfo -> tm_mon) + 1;
//    //  - 2000, ������ ��� ������� ������ ������� 0 - 99.
//    xCurrentTime.tm_year = (timeinfo -> tm_year) + 1900 - 2000;


        xCurrentTime.tm_mon = (timeinfo -> tm_mon);
        xCurrentTime.tm_year = (timeinfo -> tm_year);

        // ������� �����������?
        if ((timeinfo -> tm_wday) == LINUX_WEEK_DAY_SUNDAY)
        {
            xCurrentTime.tm_wday = WEEK_DAY_SUNDAY;
        }
        else
        {
            xCurrentTime.tm_wday = timeinfo -> tm_wday;
        }

        // �������� ������� ����� � ������� ������ Modbus.
//    memcpy(pusDestination,
//           aui8CurrentTime,
//           CURRENT_TIME_BYTE_QUANTITY);

        pusDestination[CURRENT_TIME_SECOND_OFFSET] = timeinfo -> tm_sec;
        pusDestination[CURRENT_TIME_MINUTE_OFFSET] = timeinfo -> tm_min;
        pusDestination[CURRENT_TIME_HOUR_OFFSET] = timeinfo -> tm_hour;
        pusDestination[CURRENT_TIME_MONTH_DAY_OFFSET] = timeinfo -> tm_mday;
        pusDestination[CURRENT_TIME_MONTH_OFFSET] = (timeinfo -> tm_mon) + 1;
        pusDestination[CURRENT_TIME_YEAR_OFFSET] = (timeinfo -> tm_year) - 100;

        // ������� �����������?
        if ((timeinfo -> tm_wday) == LINUX_WEEK_DAY_SUNDAY)
        {
            pusDestination[CURRENT_TIME_WEEK_DAY_OFFSET] = WEEK_DAY_SUNDAY;
        }
        else
        {
            pusDestination[CURRENT_TIME_WEEK_DAY_OFFSET] = timeinfo -> tm_wday;
        }

        memcpy(&aucHoldingRegistersArray[CURRENT_TIME_HOLDING_BYTE_ARRAY_OFFSET],
               &aucInputRegistersArray[CURRENT_TIME_BYTE_ARRAY_OFFSET],
               (CURRENT_TIME_BYTE_QUANTITY * sizeof(short)));

        // ������ ������?
        if (ui8CurrentTimeSaveDelayCounter != timeinfo -> tm_min)
        {
            ui8CurrentTimeSaveDelayCounter = timeinfo -> tm_min;
            // �������� ������� ����� � FRAM.
            iFramWrite(FRAM_LAST_SAVED_TIME_OFFSET,
                       (uint8_t*)&xCurrentTime,
                       sizeof(xCurrentTime));
        }
    }
    else
    {
        ui8CurrentTimeUpdateDelayCounter--;
    }
}

//-----------------------------------------------------------------------------------------------------
// ������������� ��������� ����� Linux.
// ���� 1 - ������� 0 � 59;
// ���� 2 - ������ 0 �59;
// ���� 3 - ��� 0 � 23;
// ���� 4 - ���� 1 � 31;
// ���� 5 - ����� 1 � 12.
// ���� 6 - ��� 0 � 99.
// ���� 7 � ���� ������ 1 � 7.
void vLinuxCurrentTimeSet(unsigned char *pucSource)
{
    time_t rawtime;
    struct tm *timeinfo;
    struct timeval systime;

    rawtime = time(NULL);
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timeinfo -> tm_sec = pucSource[CURRENT_TIME_SECOND_OFFSET];
    timeinfo -> tm_min = pucSource[CURRENT_TIME_MINUTE_OFFSET];
    timeinfo -> tm_hour = pucSource[CURRENT_TIME_HOUR_OFFSET];
    timeinfo -> tm_mday = pucSource[CURRENT_TIME_MONTH_DAY_OFFSET];
    (timeinfo -> tm_mon) = pucSource[CURRENT_TIME_MONTH_OFFSET] - 1;
    (timeinfo -> tm_year) = pucSource[CURRENT_TIME_YEAR_OFFSET] + 100;
    timeinfo -> tm_wday = pucSource[CURRENT_TIME_WEEK_DAY_OFFSET];

    systime =
    {
        mktime(timeinfo),
        0
    };
    settimeofday(&systime, NULL);
    system("sudo hwclock --systohc");
}











