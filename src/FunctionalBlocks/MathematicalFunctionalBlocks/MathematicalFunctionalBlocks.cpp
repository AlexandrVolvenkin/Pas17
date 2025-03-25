
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
#include <list>

#include "MathematicalFunctionalBlocks.h"
#include "SumMathematicalFunctionalBlock/SumMathematicalFunctionalBlock.h"
#include "DivisionMathematicalFunctionalBlock/DivisionMathematicalFunctionalBlock.h"
#include "MultiplicationMathematicalFunctionalBlock/MultiplicationMathematicalFunctionalBlock.h"
#include "SqrtMathematicalFunctionalBlock/SqrtMathematicalFunctionalBlock.h"
#include "SubstractionMathematicalFunctionalBlock/SubstractionMathematicalFunctionalBlock.h"
//#include "Manager.h"
//#include "Exercise.h"
#include "../PasNewConfig.h"

//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------
CGeneralMathematicalFunctionalBlockJob::
CGeneralMathematicalFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
CGeneralMathematicalFunctionalBlockJob::
~CGeneralMathematicalFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
void CGeneralMathematicalFunctionalBlockJob::
ExtractJobFromDataBase(
	TGeneralMathematicalFunctionalBlockDescriptor* pxGeneralMathematicalFunctionalBlockDescriptor)
{
	// извлечение задания из базы данных в соответствии с описателем
	// математического функционального блока.
	// документ - PAS_05_NEW_PO.doc стр.9

	// получим тип задания.
	SetJobType(static_cast<uint16_t>
			   (pxGeneralMathematicalFunctionalBlockDescriptor -> uiFunctionalBlockType));
	m_uiOperandAddressX1 =
		((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX1) & 0x7f);
	m_uiOperandAddressX1SourceType =
		(((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX1) & 0x80) >> 7);

	// извлечём значение.
	union
	{
		uint32_t uiUnionData = 0;
		float fUnionData;
	};

	uiUnionData |=
		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[0]));
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[1])) << 8);
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[2])) << 16);
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC1[3])) << 24);
	m_fConstantC1 = fUnionData;

	m_uiOperandAddressX2 =
		((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX2) & 0x7f);
	m_uiOperandAddressX2SourceType =
		(((pxGeneralMathematicalFunctionalBlockDescriptor -> uiOperandAddressX2) & 0x80) >> 7);

	// извлечём значение.
	uiUnionData = 0;

	uiUnionData |=
		(static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[0]));
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[1])) << 8);
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[2])) << 16);
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralMathematicalFunctionalBlockDescriptor -> auiConstantC2[3])) << 24);
	m_fConstantC2 =  fUnionData;

	m_uiResultLocationAddress =
		(pxGeneralMathematicalFunctionalBlockDescriptor -> uiResultLocationAddress);
}

////-----------------------------------------------------------------------------------------------------
//void CGeneralMathematicalFunctionalBlockJob::
//InsertJobToDataBase(
//CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob)
//{
//
//}
////-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
CMathematicalFunctionalBlockManager::
CMathematicalFunctionalBlockManager()
{
	SetJobsNumber(0);
	SetExecutorsNumber(0);
	// получим указатель на первый блок описателей математических функциональных блоков
	// в общей базе данных прибора.
	m_pxDataBaseJobsBlockAddress =
		(TPlcDataBaseBlock*)&(xPlcDataBase.
							  axPlcDataBaseBlocks[MATHEMATICAL_FUNCTIONAL_BLOCK_DATA_BASE_BLOCK_OFFSET]);
}

//-----------------------------------------------------------------------------------------------------
CMathematicalFunctionalBlockManager::
~CMathematicalFunctionalBlockManager()
{
	DestroyProductionFactors();
}

//-----------------------------------------------------------------------------------------------------
void CMathematicalFunctionalBlockManager::
DestroyProductionFactors(void)
{
	{
		std::list<CGeneralMathematicalFunctionalBlockExecutor*>::iterator xListIterator;

		for(xListIterator = ExecutorsListPointer() -> begin();
				xListIterator != ExecutorsListPointer() -> end();
				xListIterator++)
		{
			delete (*xListIterator);
		}
	}

	{
		std::list<CGeneralMathematicalFunctionalBlockJob*>::iterator xListIterator;

		for(xListIterator = JobsListPointer() -> begin();
				xListIterator != JobsListPointer() -> end();
				xListIterator++)
		{
			delete (*xListIterator);
		}
	}
}

//-----------------------------------------------------------------------------------------------------
void CMathematicalFunctionalBlockManager::
ExtractJobFromDataBase(void)
{
	TPlcDataBaseBlock* pxDataBaseJobsBlockAddress;
	CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob;

	pxDataBaseJobsBlockAddress = m_pxDataBaseJobsBlockAddress;

	// извлечём из базы данных задание для математических функциональных блоков.
	for (int i = 0;
			i < NUMBER_OF_DATA_BASE_BLOCKS_OF_MATHEMATICAL_FUNCTIONAL_BLOCKS;
			i++)
	{
		cout << "ExtractJobFromDataBase i " << (int)i << endl;

		// получим указатель на блок базы данных с описателями математических функциональных блоков.
		TGeneralMathematicalFunctionalBlockDescriptor*
		pxDataBaseJobAddressIntermediate =
			(reinterpret_cast<TGeneralMathematicalFunctionalBlockDescriptor*>
			 (&pxDataBaseJobsBlockAddress[i].aucPlcDataBaseBlockData[0]));
		for (int j = 0;
				j < NUMBER_OF_MATHEMATICAL_FUNCTIONAL_BLOCKS_IN_ONE_DATA_BASE_BLOCK;
				j++)
		{
			cout << "ExtractJobFromDataBase j " << (int)j << endl;


			cout << "ExtractJobFromDataBase uiFunctionalBlockType " << (int)(pxDataBaseJobAddressIntermediate[j].uiFunctionalBlockType) << endl;

			// какого типа описатель?
			switch (pxDataBaseJobAddressIntermediate[j].uiFunctionalBlockType)
			{
			case ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
				cout << "ExtractJobFromDataBase ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE " << endl;
				break;

			case SUM_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
				cout << "ExtractJobFromDataBase SUM_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE " << endl;
				// создадим объект - задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob =
					new CSumMathematicalFunctionalBlockJob;
				// передадим указатель на описатель, и
				// извлечём из базы данных задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob ->
				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
				// поместим в список заданий для исполнителей созданный объект.
				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
				SetJobsNumber(GetJobsNumber() + 1);
				break;

			case SUBTRACTION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
				cout << "ExtractJobFromDataBase SUBTRACTION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE " << endl;
				// создадим объект - задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob =
					new CSubstractionMathematicalFunctionalBlockJob;
				// передадим указатель на описатель, и
				// извлечём из базы данных задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob ->
				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
				// поместим в список заданий для исполнителей созданный объект.
				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
				SetJobsNumber(GetJobsNumber() + 1);
				break;

			case MULTIPLICATION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
				cout << "ExtractJobFromDataBase MULTIPLICATION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE " << endl;
				// создадим объект - задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob =
					new CMultiplicationMathematicalFunctionalBlockJob;
				// передадим указатель на описатель, и
				// извлечём из базы данных задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob ->
				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
				// поместим в список заданий для исполнителей созданный объект.
				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
				SetJobsNumber(GetJobsNumber() + 1);
				break;

			case DIVISION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
				cout << "ExtractJobFromDataBase DIVISION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE " << endl;
				// создадим объект - задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob =
					new CDivisionMathematicalFunctionalBlockJob;
				// передадим указатель на описатель, и
				// извлечём из базы данных задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob ->
				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
				// поместим в список заданий для исполнителей созданный объект.
				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
				SetJobsNumber(GetJobsNumber() + 1);
				break;

			case SQRT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
				cout << "ExtractJobFromDataBase SQRT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE " << endl;
				// создадим объект - задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob =
					new CSqrtMathematicalFunctionalBlockJob;
				// передадим указатель на описатель, и
				// извлечём из базы данных задание для исполнителя.
				pxGeneralMathematicalFunctionalBlockJob ->
				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
				// поместим в список заданий для исполнителей созданный объект.
				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
				SetJobsNumber(GetJobsNumber() + 1);
				break;

			default:
				cout << "ExtractJobFromDataBase default " << endl;
//			ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE
				break;
			}
		}
	}
}

////-----------------------------------------------------------------------------------------------------
//void CMathematicalFunctionalBlockManager::
//ExtractJobFromDataBase(TPlcDataBaseBlock*
//					   pxDataBaseJobsBlockAddress)
//{
//	CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob;
//
//	// извлечём из базы данных задание для математических функциональных блоков.
//	for (int i = 0;
//			i < NUMBER_OF_DATA_BASE_BLOCKS_OF_MATHEMATICAL_FUNCTIONAL_BLOCKS;
//			i++)
//	{
//		// получим указатель на блок базы данных с описателями математических функциональных блоков.
//		TGeneralMathematicalFunctionalBlockDescriptor*
//		pxDataBaseJobAddressIntermediate =
//			(reinterpret_cast<TGeneralMathematicalFunctionalBlockDescriptor*>
//			 (&pxDataBaseJobsBlockAddress[i].aucPlcDataBaseBlockData[0]));
//		for (int j = 0;
//				j < NUMBER_OF_MATHEMATICAL_FUNCTIONAL_BLOCKS_IN_ONE_DATA_BASE_BLOCK;
//				j++)
//		{
//			// какого типа описатель?
//			switch (pxDataBaseJobAddressIntermediate[j].uiFunctionalBlockType)
//			{
//			case ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case SUM_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
//				// создадим объект - задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob =
//					new CSumMathematicalFunctionalBlockJob;
//				// передадим указатель на описатель, и
//				// извлечём из базы данных задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob[j].
//				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
//				// поместим в список заданий для исполнителей созданный объект.
//				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
//				SetJobsNumber(GetJobsNumber() + 1);
//				break;
//
//			case SUBTRACTION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
//				// создадим объект - задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob =
//					new CSubstractionMathematicalFunctionalBlockJob;
//				// передадим указатель на описатель, и
//				// извлечём из базы данных задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob[j].
//				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
//				// поместим в список заданий для исполнителей созданный объект.
//				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
//				SetJobsNumber(GetJobsNumber() + 1);
//				break;
//
//			case MULTIPLICATION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
//				// создадим объект - задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob =
//					new CMultiplicationMathematicalFunctionalBlockJob;
//				// передадим указатель на описатель, и
//				// извлечём из базы данных задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob[j].
//				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
//				// поместим в список заданий для исполнителей созданный объект.
//				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
//				SetJobsNumber(GetJobsNumber() + 1);
//				break;
//
//			case DIVISION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
//				// создадим объект - задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob =
//					new CDivisionMathematicalFunctionalBlockJob;
//				// передадим указатель на описатель, и
//				// извлечём из базы данных задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob[j].
//				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
//				// поместим в список заданий для исполнителей созданный объект.
//				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
//				SetJobsNumber(GetJobsNumber() + 1);
//				break;
//
//			case SQRT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
//				// создадим объект - задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob =
//					new CSqrtMathematicalFunctionalBlockJob;
//				// передадим указатель на описатель, и
//				// извлечём из базы данных задание для исполнителя.
//				pxGeneralMathematicalFunctionalBlockJob[j].
//				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
//				// поместим в список заданий для исполнителей созданный объект.
//				JobsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockJob);
//				SetJobsNumber(GetJobsNumber() + 1);
//				break;
//
//			default:
////			ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE
//				break;
//			}
//		}
//	}
//}

//-----------------------------------------------------------------------------------------------------
void CMathematicalFunctionalBlockManager::
ExecutorsJobDistribution(void)
{
	std::list<CGeneralMathematicalFunctionalBlockJob*>::iterator xListIterator;

	for(xListIterator = JobsListPointer() -> begin();
			xListIterator != JobsListPointer() -> end();
			xListIterator++)
	{
		CGeneralMathematicalFunctionalBlockExecutor* pxGeneralMathematicalFunctionalBlockExecutor;
		// какого типа задание?
		switch ((*xListIterator) -> GetJobType())
		{
		case ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
			break;

		case SUM_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
			// создадим объект - исполнитель.
			pxGeneralMathematicalFunctionalBlockExecutor =
				new CSumMathematicalFunctionalBlockExecutor;
			// передадим задание исполнителю.
			pxGeneralMathematicalFunctionalBlockExecutor ->
			SetJob(m_pxAvailableResources,
				   (*xListIterator));
			// поместим в список исполнителей созданный объект.
			ExecutorsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockExecutor);
			SetExecutorsNumber(GetExecutorsNumber() + 1);
			break;

		case SUBTRACTION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
			// создадим объект - исполнитель.
			pxGeneralMathematicalFunctionalBlockExecutor =
				new CSubstractionMathematicalFunctionalBlockExecutor;
			// передадим задание исполнителю.
			pxGeneralMathematicalFunctionalBlockExecutor ->
			SetJob(m_pxAvailableResources,
				   (*xListIterator));
			// поместим в список исполнителей созданный объект.
			ExecutorsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockExecutor);
			SetExecutorsNumber(GetExecutorsNumber() + 1);
			break;

		case MULTIPLICATION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
			// создадим объект - исполнитель.
			pxGeneralMathematicalFunctionalBlockExecutor =
				new CMultiplicationMathematicalFunctionalBlockExecutor;
			// передадим задание исполнителю.
			pxGeneralMathematicalFunctionalBlockExecutor ->
			SetJob(m_pxAvailableResources,
				   (*xListIterator));
			// поместим в список исполнителей созданный объект.
			ExecutorsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockExecutor);
			SetExecutorsNumber(GetExecutorsNumber() + 1);
			break;

		case DIVISION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
			// создадим объект - исполнитель.
			pxGeneralMathematicalFunctionalBlockExecutor =
				new CDivisionMathematicalFunctionalBlockExecutor;
			// передадим задание исполнителю.
			pxGeneralMathematicalFunctionalBlockExecutor ->
			SetJob(m_pxAvailableResources,
				   (*xListIterator));
			// поместим в список исполнителей созданный объект.
			ExecutorsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockExecutor);
			SetExecutorsNumber(GetExecutorsNumber() + 1);
			break;

		case SQRT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE:
			// создадим объект - исполнитель.
			pxGeneralMathematicalFunctionalBlockExecutor =
				new CSqrtMathematicalFunctionalBlockExecutor;
			// передадим задание исполнителю.
			pxGeneralMathematicalFunctionalBlockExecutor ->
			SetJob(m_pxAvailableResources,
				   (*xListIterator));
			// поместим в список исполнителей созданный объект.
			ExecutorsListPointer() -> push_back(pxGeneralMathematicalFunctionalBlockExecutor);
			SetExecutorsNumber(GetExecutorsNumber() + 1);
			break;

		default:
//			ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------------
void CMathematicalFunctionalBlockManager::Fsm(void)
{
//		cout << "CMathematicalFunctionalBlockManager::Fs begin " << endl;

	std::list<CGeneralMathematicalFunctionalBlockExecutor*>::iterator xListIterator;
//	int i = 0;
	for(xListIterator = ExecutorsListPointer() -> begin();
			xListIterator != ExecutorsListPointer() -> end();
			xListIterator++)
	{
//		cout << "CMathematicalFunctionalBlockManager::Fs i " << (int)i << endl;
		(*xListIterator) -> Fsm();
//		i++;
	}
}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
CGeneralMathematicalFunctionalBlockExecutor::CGeneralMathematicalFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
CGeneralMathematicalFunctionalBlockExecutor::~CGeneralMathematicalFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
void CGeneralMathematicalFunctionalBlockExecutor::
SetJob(CAvailableResources*
	   pxAvailableResources,
	   CGeneralMathematicalFunctionalBlockJob*
	   pxGeneralMathematicalFunctionalBlockJob)
{
	// адрес переменной запрограммирован?
	if (pxGeneralMathematicalFunctionalBlockJob ->
			GetOperandAddressX1())
	{
		// источник операнда - входная аналоговая величина?
		if ((pxGeneralMathematicalFunctionalBlockJob ->
				GetOperandAddressX1SourceType()) ==
				CSumMathematicalFunctionalBlockJob::ANALOG_INPUT_VALUES_OPERAND_SOURCE)
		{
			m_pfOperandX1 =
				pxAvailableResources ->
				GetAnalogInputValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
											GetOperandAddressX1() -
											CONVERT_NATURAL_NUMBER_TO_INTEGER);
		}
		// источник операнда - расчетная величина?
		else
		{
			m_pfOperandX1 =
				pxAvailableResources ->
				GetCalculatedValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
										   GetOperandAddressX1() -
										   CONVERT_NATURAL_NUMBER_TO_INTEGER);
		}
	}
	else
	{
		// адрес переменной не запрограммирован.
		m_pfOperandX1 = 0;
	}

	m_fConstantC1 =
		pxGeneralMathematicalFunctionalBlockJob ->
		GetConstantC1();

	// адрес переменной запрограммирован?
	if (pxGeneralMathematicalFunctionalBlockJob ->
			GetOperandAddressX2())
	{
		// источник операнда - входная аналоговая величина?
		if ((pxGeneralMathematicalFunctionalBlockJob ->
				GetOperandAddressX2SourceType()) ==
				CSumMathematicalFunctionalBlockJob::ANALOG_INPUT_VALUES_OPERAND_SOURCE)
		{
			m_pfOperandX2 =
				pxAvailableResources ->
				GetAnalogInputValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
											GetOperandAddressX2() -
											CONVERT_NATURAL_NUMBER_TO_INTEGER);
		}
		// источник операнда - расчетная величина?
		else
		{
			m_pfOperandX2 =
				pxAvailableResources ->
				GetCalculatedValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
										   GetOperandAddressX2() -
										   CONVERT_NATURAL_NUMBER_TO_INTEGER);
		}
	}
	else
	{
		// адрес переменной не запрограммирован.
		m_pfOperandX2 = 0;
	}

	m_fConstantC2 =
		pxGeneralMathematicalFunctionalBlockJob ->
		GetConstantC2();

	m_pfResultLocation =
		pxAvailableResources ->
		GetCalculatedValuesPointer(pxGeneralMathematicalFunctionalBlockJob ->
								   GetResultLocationAddress() -
								   CONVERT_NATURAL_NUMBER_TO_INTEGER);
}

////-----------------------------------------------------------------------------------------------------
//void CGeneralMathematicalFunctionalBlockExecutor::Fsm(void)
//{
//
//}
////-----------------------------------------------------------------------------------------------------






