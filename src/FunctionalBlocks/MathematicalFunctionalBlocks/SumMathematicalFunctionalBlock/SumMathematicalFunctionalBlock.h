#ifndef CSUMMATHEMATICALFUNCTIONALBLOCK_H
#define CSUMMATHEMATICALFUNCTIONALBLOCK_H
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
#include "../MathematicalFunctionalBlocks.h"

using namespace std;


//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
//// ��������� ���������� �� �������������� �������������� ������.
//// ��������: PAS_05_NEW_PO.doc, ��� - 9,
//// 3. ���� ������ ��������� ���������� ��������
//struct TSumMathematicalFunctionalBlockDescriptor
//{
//	uint8_t uiFunctionalBlockType;
//	// ������� �1 � ����� ������ ����������,
//	// ���� ��� 7 ����� ��������� = 0,
//	// �� ���� 0-6 � ��� ����� ������� ���������� �������� ANA_ADDR (1-96),
//	// ���� ��� 7 ����� ��������� = 1,
//	// �� ���� 0-6 � ��� ����� ��������� �������� CALC_VAL_ADDR (1-48).
//	// ���� ����� ���������� �1 (�2) = 0, �� ������������ (���������) ������������ � ���������� �1 (�2)
//	uint8_t uiOperandAddressX1;
//	// ��������� �1, �2 � � ������� IEEE754.
//	// float - ������������� � ���������.
//	uint8_t auiConstantC1[4];
//	// ������� �2 � ���������� �������� �1.
//	uint8_t uiOperandAddressX2;
//	// ��������� �1, �2 � � ������� IEEE754.
//	// float - ������������� � ���������.
//	uint8_t auiConstantC2[4];
//	// Y_ADDR � ����� � ������� ��������� ������� CALC_VAL, ���� ���������� ���������.
//	uint8_t uiResultLocationAddress;
//	// ����������� ����� ($ff+����� ������ 1-10)    *).
//	uint8_t uiCrc;
//};
//#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
class CSumMathematicalFunctionalBlockJob :
	public CGeneralMathematicalFunctionalBlockJob
{
public:

	CSumMathematicalFunctionalBlockJob();
	virtual ~CSumMathematicalFunctionalBlockJob();

//	virtual void ExtractJobFromDataBase(
//		TGeneralMathematicalFunctionalBlockDescriptor* pxGeneralMathematicalFunctionalBlockDescriptor);
//	virtual void InsertJobToDataBase(
//		CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob);
//    virtual void Fsm(void);

//protected:
//	// ������� �1 � ����� ������ ����������,
//	// ���� ��� 7 ����� ��������� = 0,
//	// �� ���� 0-6 � ��� ����� ������� ���������� �������� ANA_ADDR (1-96),
//	// ���� ��� 7 ����� ��������� = 1,
//	// �� ���� 0-6 � ��� ����� ��������� �������� CALC_VAL_ADDR (1-48).
//	// ���� ����� ���������� �1 (�2) = 0, �� ������������ (���������) ������������ � ���������� �1 (�2)
//	uint8_t m_uiOperandAddressX1;
//	uint8_t m_uiOperandAddressX1SourceType;
//	// ��������� �1, �2 � � ������� IEEE754.
//	float m_fConstantC1;
//	// ������� �2 � ���������� �������� �1.
//	uint8_t m_uiOperandAddressX2;
//	uint8_t m_uiOperandAddressX2SourceType;
//	// ��������� �1, �2 � � ������� IEEE754.
//	float m_fConstantC2;
//	// Y_ADDR � ����� � ������� ��������� ������� CALC_VAL, ���� ���������� ���������.
//	uint8_t m_uiResultLocationAddress;
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
class CSumMathematicalFunctionalBlockManager :
	public CGeneralMathematicalFunctionalBlockManager
{
public:

	CSumMathematicalFunctionalBlockManager();
	virtual ~CSumMathematicalFunctionalBlockManager();
//    virtual void Fsm(void);

protected:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
class CSumMathematicalFunctionalBlockExecutor :
	public CGeneralMathematicalFunctionalBlockExecutor
{
public:
	enum
	{
		SUMM_MODE_NORMAL = 0,
		SUMM_MODE_SUM_WITH_CONSTANT_C1 = 1,
		SUMM_MODE_SUM_WITH_CONSTANT_C2 = 2,
		SUMM_MODE_SUM_OF_CONSTANT = 3,
	};

	CSumMathematicalFunctionalBlockExecutor();
	virtual ~CSumMathematicalFunctionalBlockExecutor();
//	virtual void SetJob(CGeneralMathematicalFunctionalBlockJob*
//						pxGeneralMathematicalFunctionalBlockJob);
//	virtual void SetJob(CAvailableResources*
//						pxAvailableResources,
//						CGeneralMathematicalFunctionalBlockJob*
//						pxGeneralMathematicalFunctionalBlockJob);
//	virtual void AllocateResources(CAvailableResources*
//								   pxAvailableResources);
	virtual void Fsm(void);

//protected:
//	float *m_pfOperandX1;
//	// ��������� �1, �2 � � ������� IEEE754.
//	float m_fConstantC1;
//	float *m_pfOperandX2;
//	// ��������� �1, �2 � � ������� IEEE754.
//	float m_fConstantC2;
//	float *m_pfResultLocation;
};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------

#endif // CSUMMATHEMATICALFUNCTIONALBLOCK_H
