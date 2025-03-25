
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

#include "SubstractionMathematicalFunctionalBlock.h"
//#include "../FunctionalBlocks/FunctionalBlocks.h"
//#include "Manager.h"
//#include "Exercise.h"
//#include "PasNewConfig.h"

//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------
CSubstractionMathematicalFunctionalBlockJob::
CSubstractionMathematicalFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
CSubstractionMathematicalFunctionalBlockJob::
~CSubstractionMathematicalFunctionalBlockJob()
{

}

////-----------------------------------------------------------------------------------------------------
//void CSubstractionMathematicalFunctionalBlockJob::
//ExtractJobFromDataBase(
//	TGeneralMathematicalFunctionalBlockDescriptor* pxGeneralMathematicalFunctionalBlockDescriptor)
//{
//	// извлечение задания из базы данных в соответствии с описателем
//	// математического функционального блока.
//	// документ - PAS_05_NEW_PO.doc стр.9
//
//	// получим тип задания.
//	SetJobType(static_cast<uint16_t>
//			   (pxGeneralMathematicalFunctionalBlockDescriptor -> uiFunctionalBlockType));
//	m_uiOperandAddressX1 =
//		((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX1) & 0x7f);
//	m_uiOperandAddressX1SourceType =
//		(((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX1) & 0x80) >> 7);
//
//	// извлечём значение.
//	union
//	{
//		uint32_t uiUnionData = 0;
//		float fUnionData;
//	};
//
//	uiUnionData |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[0]));
//	uiUnionData |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[1])) << 8);
//	uiUnionData |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[2])) << 16);
//	uiUnionData |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[3])) << 24);
//	m_fConstantC1 = fUnionData;
//
//	m_uiOperandAddressX2 =
//		((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX2) & 0x7f);
//	m_uiOperandAddressX2SourceType =
//		(((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX2) & 0x80) >> 7);
//
//	// извлечём значение.
//	uiUnionData = 0;
//
//	uiUnionData |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[0]));
//	uiUnionData |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[1])) << 8);
//	uiUnionData |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[2])) << 16);
//	uiUnionData |=
//		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[3])) << 24);
//	m_fConstantC2 =  fUnionData;
//
//	m_uiResultLocationAddress =
//		(pxGeneralMathematicalFunctionalBlockDescriptor -> uiResultLocationAddress);
//}

////-----------------------------------------------------------------------------------------------------
//void CSubstractionMathematicalFunctionalBlockJob::
//InsertJobToDataBase(CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob)
//{
//
//}
////-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
CSubstractionMathematicalFunctionalBlockManager::
CSubstractionMathematicalFunctionalBlockManager()
{

}

//-----------------------------------------------------------------------------------------------------
CSubstractionMathematicalFunctionalBlockManager::
~CSubstractionMathematicalFunctionalBlockManager()
{

}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
CSubstractionMathematicalFunctionalBlockExecutor::
CSubstractionMathematicalFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
CSubstractionMathematicalFunctionalBlockExecutor::
~CSubstractionMathematicalFunctionalBlockExecutor()
{

}

////-----------------------------------------------------------------------------------------------------
//void CSubstractionMathematicalFunctionalBlockExecutor::
//SetJob(CAvailableResources*
//	   pxAvailableResources,
//	   CGeneralMathematicalFunctionalBlockJob*
//	   pxGeneralMathematicalFunctionalBlockJob)
//{
//	// источник операнда - расчетная величина?
//	if ((pxGeneralMathematicalFunctionalBlockJob ->
//			GetOperandAddressX1SourceType()) ==
//			CSubstractionMathematicalFunctionalBlockJob::ANALOG_INPUT_VALUES_OPERAND_SOURCE)
//	{
//		m_pfOperandX1 =
//			pxAvailableResources ->
//			GetAnalogInputValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
//										GetOperandAddressX1());
//	}
//	// источник операнда - входная аналоговая величина?
//	else
//	{
//		m_pfOperandX1 =
//			pxAvailableResources ->
//			GetCalculatedValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
//									   GetOperandAddressX1());
//	}
//
//	m_fConstantC1 =
//		pxGeneralMathematicalFunctionalBlockJob ->
//		GetConstantC1();
//
//	// источник операнда - расчетная величина?
//	if ((pxGeneralMathematicalFunctionalBlockJob ->
//			GetOperandAddressX2SourceType()) ==
//			CSubstractionMathematicalFunctionalBlockJob::ANALOG_INPUT_VALUES_OPERAND_SOURCE)
//	{
//		m_pfOperandX2 =
//			pxAvailableResources ->
//			GetAnalogInputValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
//										GetOperandAddressX2());
//	}
//	// источник операнда - входная аналоговая величина?
//	else
//	{
//		m_pfOperandX2 =
//			pxAvailableResources ->
//			GetCalculatedValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
//									   GetOperandAddressX2());
//	}
//
//	m_fConstantC2 =
//		pxGeneralMathematicalFunctionalBlockJob ->
//		GetConstantC2();
//
//	m_pfResultLocation =
//		pxAvailableResources ->
//		GetCalculatedValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
//								   GetResultLocationAddress());
//}

//-----------------------------------------------------------------------------------------------------
void CSubstractionMathematicalFunctionalBlockExecutor::
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
		fOperandX2 = (m_fConstantC2 * (*m_pfOperandX2));
	}
	else
	{
		fOperandX2 = (m_fConstantC2);
	}

	*m_pfResultLocation =
		(fOperandX1 - fOperandX2);
}
//-----------------------------------------------------------------------------------------------------








