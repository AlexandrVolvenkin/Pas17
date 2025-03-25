#ifndef ANALOGUECOMPARATORFUNCTIONALBLOCK_H_INCLUDED
#define ANALOGUECOMPARATORFUNCTIONALBLOCK_H_INCLUDED
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <list>

//#include "Dfa.h"
//#include "Job.h"
//#include "Manager.h"
//#include "Executor.h"
#include "../LogicalFunctionalBlocks.h"

using namespace std;

//class CGeneralLogicalFunctionalBlockJob;
//class CGeneralLogicalFunctionalBlockManager;
//class CGeneralLogicalFunctionalBlockExecutor;


//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ���������� �� �������������� ������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TAnalogueComparatorFunctionalBlockDescriptor
{
	uint8_t uiFunctionalBlockType;
	// ������ ������� ����������� �������� � ��.
	// ��� ������� ���� ��������������� ����� ���� ������.
	uint8_t auiInputAddress[8];
	// D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
	// D5 - ��� =1 ������������, 0 � �� ������������.
	// D6 - ���1 � ��� ������������ ��������� ������ ���:
	// D7 - ���2 � ��� ������������ ��������� ������ ���:
	// 00 � ��� ������������
	// 01 � ����������������� ������������
	// 10 � ��������� ������������
	// 11 � ��������� (������ ���� ��� ��������� �������)
	uint8_t uiTalArhGrp;
	// ���� 1:
	// ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
	// ���� 2,3,4,5,6,7,8:
	// ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
	uint8_t auiRelayOut[8];
	// ����������� ����� ($ff+����� ������ 1-10)    *).
	uint8_t uiCrc;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
class CAnalogueComparatorFunctionalBlockJob :
	public CGeneralLogicalFunctionalBlockJob
{
public:

	CAnalogueComparatorFunctionalBlockJob();
	virtual ~CAnalogueComparatorFunctionalBlockJob();

//	virtual void ExtractJobFromDataBase(uint16_t uiJobDescriptorIndex);
//	virtual void InsertJobToDataBase(uint16_t uiJobDescriptorIndex);

private:

	virtual void ExtractJobFromDataBase(
		TGeneralLogicalFunctionalBlockDescriptor* pxGeneralLogicalFunctionalBlockDescriptor);
//	virtual void InsertJobToDataBase(
//		CGeneralLogicalFunctionalBlockJob* pxGeneralLogicalFunctionalBlockJob);
//    virtual void Fsm(void);

protected:
	// ����� - �������� �������� ����������� �������:
	uint8_t m_uiAnalogInputSourceAddress;
	// 0 � ������� ���������� ������ � ���������� �������� (1-96);
	// 1 � ��������� �������� (1-48).
	uint8_t m_uiAnalogInputSourceType;
	// �������  � ������������� �������� ������� � ������� IEEE754;
	float m_fSetpoint;
	// cond � ������� ������������ ������:
	// ��� cond = 0 ����� = 1, ���� ���������� ������ ������ �������
	// ��� cond = 1 ����� = 1, ���� ���������� ������ ������ �������
	uint8_t m_uiOutputGenerationCondition;
	// ��� � ���������� 0 � 31 % ��������� ��������� (�����) ����������� �������:
	// ����� �������� ������ ��� �����, ��� 7 = 0, ���� ��� 7 = 1 (��������� ��������) �� ��� = 0
	uint8_t m_uiHisteresis;
	// ��� � ������� ������� �������� ��������� ������ � �������� ��� (0-120)
	// 0,1�120=12 �, 1�120=120 �(2,0 ���), 10�120=1200 � (20,0 ���)
	uint8_t m_uiOnTimeDelaySetpoint;
	// ���1 � ���4 � ��� �����: 1 � ������, 0 � ���������;
	uint8_t m_uiDiscreteInputType;
	// ����1 � ����4 � �������� �������� �������:
	// 0 � ������� ������ ��� (������� ������ DI_value, ����� 1-384);
	// 1 � ����� ��� (����� 1-120) ��� ������� ���������� � �������� ������ ��� (������� ������ Log_Com, ����� 121-240), 241 � ������ (��� ��) ����, 242 � ������ (��� ��) �����;
	// 2 � ���� ��������������� �������� ����������� ������� (Bad_�I ����� 1-96);
	// 3 � ���������� ����� ��������� CoDeSys (Log_Sost, ����� 1-240)
	// 4 � ���� ��������������� �������� ����������� ������� (Bad_DI, ����� 1-384);
	uint8_t m_uiDiscreteInputSourceType;
	// �����1 � �����4 � ����� � ��������������� �������;
	uint8_t m_uiDiscreteInputSourceAddress;

	// ���2, ���1 � ��� ������������ ��������� ������ ���:
	// 00 � ��� ������������
	// 01 � ����������������� ������������
	// 10 � ��������� ������������
	// 11 � ��������� (������ ���� ��� ��������� �������)
	uint8_t m_uiAlarmType;
	// ��� =1 ������������, 0 � �� ������������
	bool m_bIsArchived;
	// � ��� � ����� ������ ��������� �����: (1-24), 0 � ��� ������ �� �������� ����� ;
	uint8_t m_uiLightPanelCellNumber;
	// ���� 1:
	// ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
	// ���� 2,3,4,5,6,7,8:
	// ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
	uint8_t m_auiRelayOut[8];
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
class CAnalogueComparatorFunctionalBlockManager :
	public CGeneralLogicalFunctionalBlockManager
{
public:

	CAnalogueComparatorFunctionalBlockManager();
	virtual ~CAnalogueComparatorFunctionalBlockManager();
//    virtual void Fsm(void);

protected:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
class CAnalogueComparatorFunctionalBlockExecutor :
	public CGeneralLogicalFunctionalBlockExecutor
{
public:

	CAnalogueComparatorFunctionalBlockExecutor();
	virtual ~CAnalogueComparatorFunctionalBlockExecutor();
	virtual void SetJob(CGeneralLogicalFunctionalBlockJob*
						pxGeneralMathematicalFunctionalBlockJob);
	virtual void Fsm(void);

protected:
private:
};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
#endif // ANALOGUECOMPARATORFUNCTIONALBLOCK_H_INCLUDED
