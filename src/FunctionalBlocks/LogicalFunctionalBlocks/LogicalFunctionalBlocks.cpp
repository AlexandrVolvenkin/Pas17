
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

#include "LogicalFunctionalBlocks.h"
#include "AnalogueComparatorFunctionalBlock/AnalogueComparatorFunctionalBlock.h"
//#include "Manager.h"
//#include "Exercise.h"
#include "../PasNewConfig.h"

////-----------------------------------------------------------------------------------------------------
//// Job section.
//
////-----------------------------------------------------------------------------------------------------
//CGeneralLogicalFunctionalBlockJob::
//CGeneralLogicalFunctionalBlockJob()
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//CGeneralLogicalFunctionalBlockJob::
//~CGeneralLogicalFunctionalBlockJob()
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//void CGeneralLogicalFunctionalBlockJob::
//ExtractJobFromDataBase(
//TGeneralLogicalFunctionalBlockDescriptor* pxGeneralLogicalFunctionalBlockDescriptor)
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//void CGeneralLogicalFunctionalBlockJob::
//InsertJobToDataBase(
//CGeneralLogicalFunctionalBlockJob* pxGeneralLogicalFunctionalBlockJob)
//{
//
//}
////-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
CLogicalFunctionalBlockManager::
CLogicalFunctionalBlockManager()
{
	SetJobsNumber(0);
	// ������� ��������� �� ������ ���� ���������� ���������� �������������� ������
	// � ����� ���� ������ �������.
	m_pxDataBaseJobsBlockAddress =
		(TPlcDataBaseBlock*)&(xPlcDataBase.
				axPlcDataBaseBlocks[LOGICAL_FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET]);
//	m_pxDataBaseJobAddress =
//		(TGeneralLogicalFunctionalBlockDescriptor*)&(xPlcDataBase.
//				axPlcDataBaseBlocks[LOGICAL_FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET].
//				aucPlcDataBaseBlockData[0]);
}

//-----------------------------------------------------------------------------------------------------
CLogicalFunctionalBlockManager::
~CLogicalFunctionalBlockManager()
{

}

//-----------------------------------------------------------------------------------------------------
void CLogicalFunctionalBlockManager::
ExtractJobFromDataBase(void)
{
	TPlcDataBaseBlock* pxDataBaseJobsBlockAddress;
	CGeneralLogicalFunctionalBlockJob* pxGeneralLogicalFunctionalBlockJob;

	pxDataBaseJobsBlockAddress = m_pxDataBaseJobsBlockAddress;

	// �������� �� ���� ������ ������� ��� ���������� �������������� ������.
	for (int i = 0;
			i < NUMBER_OF_DATA_BASE_BLOCKS_OF_LOGICAL_FUNCTIONAL_BLOCKS;
			i++)
	{
		// ������� ��������� �� ���� ���� ������ � ����������� ���������� �������������� ������.
		TGeneralLogicalFunctionalBlockDescriptor*
		pxDataBaseJobAddressIntermediate =
			(reinterpret_cast<TGeneralLogicalFunctionalBlockDescriptor*>
			 (&pxDataBaseJobsBlockAddress[i].aucPlcDataBaseBlockData[0]));
		for (int j = 0;
				j < NUMBER_OF_LOGICAL_FUNCTIONAL_BLOCKS_IN_ONE_DATA_BASE_BLOCK;
				j++)
		{
			// ������ ���� ���������?
			switch (pxDataBaseJobAddressIntermediate[j].uiFunctionalBlockType)
			{
			case ABSENT_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case AND_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case AND_NOT_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case OR_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case OR_NOT_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case HISTERESIS_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case TIMER_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case TRIGGER_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				break;

			case ANALOGUE_COMPARATOR_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
				// �������� ������ - ������� ��� �����������.
				pxGeneralLogicalFunctionalBlockJob =
					new CAnalogueComparatorFunctionalBlockJob;
				// ��������� ��������� �� ���������, �
				// �������� �� ���� ������ ������� ��� �����������.
				pxGeneralLogicalFunctionalBlockJob ->
				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
				// �������� � ������ ������� ��� ������������ ��������� ������.
				JobsListPointer() -> push_back(pxGeneralLogicalFunctionalBlockJob);
				SetJobsNumber(GetJobsNumber() + 1);
				break;

			default:
//			ABSENT_LOGICAL_FUNCTIONAL_BLOCK_TYPE
				break;
			}
		}
	}
}

////-----------------------------------------------------------------------------------------------------
//void CLogicalFunctionalBlockManager::
//ExtractJobFromDataBase(TPlcDataBaseBlock*
//					   pxDataBaseJobsBlockAddress)
//{
//	CGeneralLogicalFunctionalBlockJob* pxGeneralLogicalFunctionalBlockJob;
//
//	// �������� �� ���� ������ ������� ��� ���������� �������������� ������.
//	for (int i = 0;
//			i < NUMBER_OF_DATA_BASE_BLOCKS_OF_LOGICAL_FUNCTIONAL_BLOCKS;
//			i++)
//	{
//		// ������� ��������� �� ���� ���� ������ � ����������� ���������� �������������� ������.
//		TGeneralLogicalFunctionalBlockDescriptor*
//		pxDataBaseJobAddressIntermediate =
//			(reinterpret_cast<TGeneralLogicalFunctionalBlockDescriptor*>
//			 (&pxDataBaseJobsBlockAddress[i].aucPlcDataBaseBlockData[0]));
//		for (int j = 0;
//				j < NUMBER_OF_LOGICAL_FUNCTIONAL_BLOCKS_IN_ONE_DATA_BASE_BLOCK;
//				j++)
//		{
//			// ������ ���� ���������?
//			switch (pxDataBaseJobAddressIntermediate[j].uiFunctionalBlockType)
//			{
//			case ABSENT_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case AND_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case AND_NOT_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case OR_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case OR_NOT_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case HISTERESIS_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case TIMER_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case TRIGGER_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				break;
//
//			case ANALOGUE_COMPARATOR_LOGICAL_FUNCTIONAL_BLOCK_TYPE:
//				// �������� ������ - ������� ��� �����������.
//				pxGeneralLogicalFunctionalBlockJob =
//					new CAnalogueComparatorFunctionalBlockJob;
//				// ��������� ��������� �� ���������, �
//				// �������� �� ���� ������ ������� ��� �����������.
//				pxGeneralLogicalFunctionalBlockJob[j].
//				ExtractJobFromDataBase(&pxDataBaseJobAddressIntermediate[j]);
//				// �������� � ������ ������� ��� ������������ ��������� ������.
//				JobsListPointer() -> push_back(pxGeneralLogicalFunctionalBlockJob);
//				SetJobsNumber(GetJobsNumber() + 1);
//				break;
//
//			default:
////			ABSENT_LOGICAL_FUNCTIONAL_BLOCK_TYPE
//				break;
//			}
//		}
//	}
//}

//-----------------------------------------------------------------------------------------------------
void CLogicalFunctionalBlockManager::
ExecutorsJobDistribution(void)
{
}
//-----------------------------------------------------------------------------------------------------







////-----------------------------------------------------------------------------------------------------
//// Executor section.
//
////-----------------------------------------------------------------------------------------------------
//CGeneralLogicalFunctionalBlockExecutor::CGeneralLogicalFunctionalBlockExecutor()
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//CGeneralLogicalFunctionalBlockExecutor::~CGeneralLogicalFunctionalBlockExecutor()
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//void CGeneralLogicalFunctionalBlockExecutor::Fsm(void)
//{
//
//}







