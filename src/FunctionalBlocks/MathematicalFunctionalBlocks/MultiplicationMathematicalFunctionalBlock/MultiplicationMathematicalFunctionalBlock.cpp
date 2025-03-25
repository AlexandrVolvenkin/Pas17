
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------

#include <iostream>
#include <list>
#include <iterator>

#include "MultiplicationMathematicalFunctionalBlock.h"
//#include "../FunctionalBlocks/FunctionalBlocks.h"
//#include "Manager.h"
//#include "Exercise.h"
//#include "PasNewConfig.h"

//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------
CMultiplicationMathematicalFunctionalBlockJob::
CMultiplicationMathematicalFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
CMultiplicationMathematicalFunctionalBlockJob::
~CMultiplicationMathematicalFunctionalBlockJob()
{

}

////-----------------------------------------------------------------------------------------------------
//void CMultiplicationMathematicalFunctionalBlockJob::
//ExtractJobFromDataBase(
//	TGeneralMathematicalFunctionalBlockDescriptor* pxGeneralMathematicalFunctionalBlockDescriptor)
//{
//	// ���������� ������� �� ���� ������ � ������������ � ����������
//	// ��������������� ��������������� ����� - ��������.
//	// �������� - PAS_05_NEW_PO.doc ���.9
//
//	// ������� ��� �������.
//	SetJobType(static_cast<uint16_t>
//			   (pxGeneralMathematicalFunctionalBlockDescriptor -> uiFunctionalBlockType));
//	m_uiOperandAddressX1 =
//		((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX1) & 0x7f);
//	m_uiOperandAddressX1SourceType =
//		(((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX1) & 0x80) >> 7);
//
//	// �������� ��������.
//	uint32_t uiConstantC1 = 0;
//	uiConstantC1 |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[0]));
//	uiConstantC1 |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[1])) << 8);
//	uiConstantC1 |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[2])) << 16);
//	uiConstantC1 |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[3])) << 32);
//	m_fConstantC1 = static_cast<float>(uiConstantC1);
//
//	m_uiOperandAddressX2 =
//		((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX2) & 0x7f);
//	m_uiOperandAddressX2SourceType =
//		(((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX2) & 0x80) >> 7);
//
//	// �������� ��������.
//	uint32_t uiConstantC2 = 0;
//	uiConstantC2 |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[0]));
//	uiConstantC2 |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[1])) << 8);
//	uiConstantC2 |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[2])) << 16);
//	uiConstantC2 |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[3])) << 32);
//	m_fConstantC2 = static_cast<float>(uiConstantC2);
//
//	m_uiResultLocationAddress =
//		(pxGeneralMathematicalFunctionalBlockDescriptor -> uiResultLocationAddress);
//}

////-----------------------------------------------------------------------------------------------------
//void CMultiplicationMathematicalFunctionalBlockJob::
//InsertJobToDataBase(CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob)
//{
//
//}
////-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
CMultiplicationMathematicalFunctionalBlockManager::
CMultiplicationMathematicalFunctionalBlockManager()
{

}

//-----------------------------------------------------------------------------------------------------
CMultiplicationMathematicalFunctionalBlockManager::
~CMultiplicationMathematicalFunctionalBlockManager()
{

}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
CMultiplicationMathematicalFunctionalBlockExecutor::
CMultiplicationMathematicalFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
CMultiplicationMathematicalFunctionalBlockExecutor::
~CMultiplicationMathematicalFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
void CMultiplicationMathematicalFunctionalBlockExecutor::
Fsm(void)
{
	float fOperandX1;
	float fOperandX2;

	if (*m_pfOperandX1)
	{
		fOperandX1 = (m_fConstantC1 * (*m_pfOperandX1));
	}
	else
	{
		fOperandX1 = (m_fConstantC1);
	}

	if (*m_pfOperandX2)
	{
		fOperandX2 = (*m_pfOperandX2);
	}
	else
	{
		fOperandX2 = 1;
	}

	*m_pfResultLocation =
		(fOperandX1 * fOperandX2);
}
//-----------------------------------------------------------------------------------------------------








