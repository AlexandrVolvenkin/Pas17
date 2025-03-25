
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

#include "AnalogueComparatorFunctionalBlock.h"
//#include "../FunctionalBlocks/FunctionalBlocks.h"
//#include "Manager.h"
//#include "Exercise.h"
//#include "PasNewConfig.h"

//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------
CAnalogueComparatorFunctionalBlockJob::
CAnalogueComparatorFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
CAnalogueComparatorFunctionalBlockJob::
~CAnalogueComparatorFunctionalBlockJob()
{

}

//-----------------------------------------------------------------------------------------------------
void CAnalogueComparatorFunctionalBlockJob::
ExtractJobFromDataBase(
	TGeneralLogicalFunctionalBlockDescriptor* pxGeneralLogicalFunctionalBlockDescriptor)
{
	// извлечение задания из базы данных в соответствии с описателем
	// логического функционального блока - аналоговый компаратор.
	// документ - PAS_05_NEW_PO.doc стр.7

	// получим тип задания.
	SetJobType(static_cast<uint16_t>
			   (pxGeneralLogicalFunctionalBlockDescriptor -> uiFunctionalBlockType));
	m_uiAnalogInputSourceAddress =
		((pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[0]) & 0x7f);
	m_uiAnalogInputSourceType =
		(((pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[0]) & 0x80) >> 7);

	// извлечём уставку.
	union
	{
		uint32_t uiUnionData = 0;
		float fUnionData;
	};

	uiUnionData |=
		(static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[1]));
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[2])) << 8);
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[3])) << 16);
	uiUnionData |=
		((static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[4])) << 24);
	m_fSetpoint = fUnionData;

	m_uiOutputGenerationCondition =
		(((pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[5]) & 0x80) >> 7);
	m_uiHisteresis =
		(((pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[5]) & 0x7c) >> 2);
//	uiOnTimeDelaySetpoint =
//		((pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[5]) & 0x80);

	m_uiAlarmType =
		(((pxGeneralLogicalFunctionalBlockDescriptor -> uiTalArhGrp) & 0xfc) >> 6);
	m_bIsArchived =
		(((pxGeneralLogicalFunctionalBlockDescriptor -> uiTalArhGrp) & 0x20) >> 5);
	m_uiLightPanelCellNumber =
		((pxGeneralLogicalFunctionalBlockDescriptor -> uiTalArhGrp) & 0x1f);
	for (int i = 0;
			i < 8;
			i++)
	{
		m_auiRelayOut[i] =
			pxGeneralLogicalFunctionalBlockDescriptor -> auiRelayOut[i];
	}
}

////-----------------------------------------------------------------------------------------------------
//void CAnalogueComparatorFunctionalBlockJob::
//InsertJobToDataBase(CGeneralLogicalFunctionalBlockJob* pxGeneralLogicalFunctionalBlockJob)
//{
//
//}

////-----------------------------------------------------------------------------------------------------
//void CAnalogueComparatorFunctionalBlockJob::
//ExtractJobFromDataBase(uint16_t uiJobDescriptorIndex)
//{
//
//}

//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
CAnalogueComparatorFunctionalBlockManager::
CAnalogueComparatorFunctionalBlockManager()
{

}

//-----------------------------------------------------------------------------------------------------
CAnalogueComparatorFunctionalBlockManager::
~CAnalogueComparatorFunctionalBlockManager()
{

}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
CAnalogueComparatorFunctionalBlockExecutor::
CAnalogueComparatorFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
CAnalogueComparatorFunctionalBlockExecutor::
~CAnalogueComparatorFunctionalBlockExecutor()
{

}

//-----------------------------------------------------------------------------------------------------
void CAnalogueComparatorFunctionalBlockExecutor::
SetJob(CGeneralLogicalFunctionalBlockJob*
	   pxGeneralMathematicalFunctionalBlockJob)
{

}

//-----------------------------------------------------------------------------------------------------
void CAnalogueComparatorFunctionalBlockExecutor::
Fsm(void)
{

}









//	fSetpoint = 0;
//	(static_cast<uint32_t>(fSetpoint)) |=
//		(static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[1]));
//	((static_cast<uint32_t>(fSetpoint)) >> 8) |=
//		(static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[2]));
//	((static_cast<uint32_t>(fSetpoint)) >> 16) |=
//		(static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[3]));
//	((static_cast<uint32_t>(fSetpoint)) >> 24) |=
//		(static_cast<uint32_t>(pxGeneralLogicalFunctionalBlockDescriptor -> auiInputAddress[4]));
