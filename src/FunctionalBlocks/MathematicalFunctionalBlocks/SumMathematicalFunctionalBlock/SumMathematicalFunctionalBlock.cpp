
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

#include "SumMathematicalFunctionalBlock.h"
//#include "../FunctionalBlocks/FunctionalBlocks.h"
//#include "Manager.h"
//#include "Exercise.h"
//#include "PasNewConfig.h"
#include "../AvailableResources.h"

//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------
CSumMathematicalFunctionalBlockJob::
CSumMathematicalFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
CSumMathematicalFunctionalBlockJob::
~CSumMathematicalFunctionalBlockJob()
{

}

////-----------------------------------------------------------------------------------------------------
//void CSumMathematicalFunctionalBlockJob::
//ExtractJobFromDataBase(
//	TGeneralMathematicalFunctionalBlockDescriptor* pxGeneralMathematicalFunctionalBlockDescriptor)
//{
//	// извлечение задания из базы данных в соответствии с описателем
//	// математического функционального блока - сумматор.
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
////	cout << "ExtractJobFromDataBase auiConstantC1[0] " << (int)((pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[0])) << endl;
////	cout << "ExtractJobFromDataBase auiConstantC1[1] " << (int)((pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[1])) << endl;
////	cout << "ExtractJobFromDataBase auiConstantC1[2] " << (int)((pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[2])) << endl;
////	cout << "ExtractJobFromDataBase auiConstantC1[3] " << (int)((pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[3])) << endl;
////	cout << "ExtractJobFromDataBase uiConstantC1 " << std::hex << (uiConstantC1) << endl;
////	cout << "ExtractJobFromDataBase fConstantC1 " << std::hex << (int)(fConstantC1) << endl;
////	cout << "ExtractJobFromDataBase fConstantC1 " << (fConstantC1) << endl;
////	cout << "ExtractJobFromDataBase m_fConstantC1 " << (m_fConstantC1) << endl;
//
////printf("%.2X", a);
////	printf("printf ExtractJobFromDataBase uiConstantC1 %x\n\r", (uiConstantC1));
////	printf("printf ExtractJobFromDataBase m_fConstantC1 %x\n\r", (m_fConstantC1));
////	printf("printf ExtractJobFromDataBase m_fConstantC1 %x\n\r", (m_fConstantC1));
////	printf("printf ExtractJobFromDataBase m_fConstantC1 %d\n\r", (m_fConstantC1));
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
////	cout << "ExtractJobFromDataBase uiConstantC2 " << (int)(uiConstantC2) << endl;
////	cout << "ExtractJobFromDataBase m_fConstantC2 " << (float)(m_fConstantC2) << endl;
//
//	m_uiResultLocationAddress =
//		(pxGeneralMathematicalFunctionalBlockDescriptor -> uiResultLocationAddress);
//}

////-----------------------------------------------------------------------------------------------------
//void CSumMathematicalFunctionalBlockJob::
//InsertJobToDataBase(CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob)
//{
//
//}
////-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
CSumMathematicalFunctionalBlockManager::
CSumMathematicalFunctionalBlockManager()
{

}

//-----------------------------------------------------------------------------------------------------
CSumMathematicalFunctionalBlockManager::
~CSumMathematicalFunctionalBlockManager()
{

}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
CSumMathematicalFunctionalBlockExecutor::
CSumMathematicalFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
CSumMathematicalFunctionalBlockExecutor::
~CSumMathematicalFunctionalBlockExecutor()
{

}

////-----------------------------------------------------------------------------------------------------
//void CSumMathematicalFunctionalBlockExecutor::
//SetJob(CGeneralMathematicalFunctionalBlockJob*
//	   pxGeneralMathematicalFunctionalBlockJob)
//{
//
//}

////-----------------------------------------------------------------------------------------------------
//void CSumMathematicalFunctionalBlockExecutor::
//SetJob(CAvailableResources*
//	   pxAvailableResources,
//	   CGeneralMathematicalFunctionalBlockJob*
//	   pxGeneralMathematicalFunctionalBlockJob)
//{
//	// источник операнда - расчетная величина?
//	if ((pxGeneralMathematicalFunctionalBlockJob ->
//			GetOperandAddressX1SourceType()) ==
//			CSumMathematicalFunctionalBlockJob::ANALOG_INPUT_VALUES_OPERAND_SOURCE)
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
//			CSumMathematicalFunctionalBlockJob::ANALOG_INPUT_VALUES_OPERAND_SOURCE)
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

////-----------------------------------------------------------------------------------------------------
//void CSumMathematicalFunctionalBlockExecutor::
//AllocateResources(CAvailableResources*
//				  pxAvailableResources)
//{
//}

//-----------------------------------------------------------------------------------------------------
void CSumMathematicalFunctionalBlockExecutor::
Fsm(void)
{
	float fOperandX1;
	float fOperandX2;



//	cout << "CSumMathematicalFunctionalBlockExecutor fsm *m_pfOperandX1 " << (float)(*m_pfOperandX1) << endl;
//	cout << "CSumMathematicalFunctionalBlockExecutor fsm m_fConstantC1 " << (float)(m_fConstantC1) << endl;
//	cout << "CSumMathematicalFunctionalBlockExecutor fsm *m_pfOperandX2 " << (float)(*m_pfOperandX2) << endl;
//	cout << "CSumMathematicalFunctionalBlockExecutor fsm m_fConstantC2 " << (float)(m_fConstantC2) << endl;

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
		(fOperandX1 + fOperandX2);

//	cout << "CSumMathematicalFunctionalBlockExecutor fsm (fOperandX1 + fOperandX2) " << (float)(fOperandX1 + fOperandX2) << endl;
//	cout << "CSumMathematicalFunctionalBlockExecutor fsm m_pfResultLocation " << (float)(*m_pfResultLocation) << endl;

//	switch (GetFsmState())
//	{
//	case SUMM_MODE_NORMAL:
//		*m_pfResultLocation =
//			(m_fConstantC1 * (*m_pfOperandX1)) + (m_fConstantC2 * (*m_pfOperandX2));
//			break;
//
//	case SUMM_MODE_SUM_WITH_CONSTANT_C1:
//		*m_pfResultLocation =
//			(m_fConstantC1) + (m_fConstantC2 * (*m_pfOperandX2));
//			break;
//
//	case SUMM_MODE_SUM_WITH_CONSTANT_C2:
//		*m_pfResultLocation =
//			(m_fConstantC1 * (*m_pfOperandX1)) + (m_fConstantC2);
//			break;
//
//	case SUMM_MODE_SUM_OF_CONSTANT:
//		*m_pfResultLocation =
//			(m_fConstantC1) + (m_fConstantC2);
//			break;
//
//	default:
//		break;
//	}
}
//-----------------------------------------------------------------------------------------------------








//	fSetpoint = 0;
//	(static_cast<uint32_t>(fSetpoint)) |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiInputAddress[1]));
//	((static_cast<uint32_t>(fSetpoint)) >> 8) |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiInputAddress[2]));
//	((static_cast<uint32_t>(fSetpoint)) >> 16) |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiInputAddress[3]));
//	((static_cast<uint32_t>(fSetpoint)) >> 24) |=
//		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiInputAddress[4]));

