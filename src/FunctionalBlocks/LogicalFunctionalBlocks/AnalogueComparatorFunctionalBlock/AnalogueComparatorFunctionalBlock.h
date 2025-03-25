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

// дл€ сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// —труктура описателей Ѕƒ функциональных блоков.
// источник: PAS_05_PO.DOC, стр - 3,
// 3. Ѕј«ј ƒјЌЌџ’ ќЅ–јЅќ“ » ƒ»— –≈“Ќџ’ —»√ЌјЋќ¬
struct TAnalogueComparatorFunctionalBlockDescriptor
{
	uint8_t uiFunctionalBlockType;
	// адреса входных управл€ющих сигналов и др.
	// дл€ каждого типа функционального блока свои данные.
	uint8_t auiInputAddress[8];
	// D0-D4 - “ЅЋ (группа): 0,1-24(0-нет вых.на табло).
	// D5 - ј–’ =1 архивировать, 0 Ц не архивировать.
	// D6 - “—√1 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
	// D7 - “—√2 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
	// 00 Ц нет сигнализации
	// 01 Ц предупредительна€ сигнализаци€
	// 10 Ц аварийна€ сигнализаци€
	// 11 Ц индикаци€ (ровный свет без звукового сигнала)
	uint8_t uiTalArhGrp;
	// Ѕайт 1:
	// ¬ыходы на ћ–1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
	// Ѕайт 2,3,4,5,6,7,8:
	// ¬ыходы на ћ–2,ћ–3,ћ–4,ћ–5,MP6,MP7,MP8:аналог.ћ–1.
	uint8_t auiRelayOut[8];
	// контрольна€ сумма ($ff+сумма байтов 1-10)    *).
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
	// »¬х—ј - источник входного аналогового сигнала:
	uint8_t m_uiAnalogInputSourceAddress;
	// 0 Ц входной аналоговый сигнал Ц измер€ема€ величина (1-96);
	// 1 Ц расчетна€ величина (1-48).
	uint8_t m_uiAnalogInputSourceType;
	// ”ставка  Ц ‘иксированное значение уставки в формате IEEE754;
	float m_fSetpoint;
	// cond Ц условие формировани€ выхода:
	// ѕри cond = 0 выход = 1, если аналоговый сигнал ћ≈Ќ№Ў≈ ”—“ј¬ »
	// ѕри cond = 1 выход = 1, если аналоговый сигнал ЅќЋ№Ў≈ ”—“ј¬ »
	uint8_t m_uiOutputGenerationCondition;
	// √—“ Ц гистерезис 0 Ц 31 % диапазона измерени€ (шкалы) аналогового сигнала:
	// имеет значение только дл€ »¬х—ј, бит 7 = 0, если бит 7 = 1 (расчетна€ величина) то √—“ = 0
	uint8_t m_uiHisteresis;
	// ”¬« Ц уставка времени задержки включени€ выхода в единицах ƒ¬« (0-120)
	// 0,1х120=12 —, 1х120=120 —(2,0 мин), 10х120=1200 — (20,0 мин)
	uint8_t m_uiOnTimeDelaySetpoint;
	// “¬х1 Ц “¬х4 Ц тип входа: 1 Ц пр€мой, 0 Ц инверсный;
	uint8_t m_uiDiscreteInputType;
	// »¬х—1 Ц »¬х—4 Ц источник входного сигнала:
	// 0 Ц входной сигнал ”—ќ (битовый массив DI_value, адрес 1-384);
	// 1 Ц выход ‘ЅЋ (адрес 1-120) или команда управлени€ с верхнего уровн€  ¬” (битовый массив Log_Com, адрес 121-240), 241 Ц кнопка (или ¬у)  ¬»“, 242 Ц кнопка (или ¬у) —Ѕ–ќ—;
	// 2 Ц флаг недостоверности входного аналогового сигнала (Bad_јI адрес 1-96);
	// 3 Ц дискретный выход алгоритма CoDeSys (Log_Sost, адрес 1-240)
	// 4 Ц флаг недостоверности входного дискретного сигнала (Bad_DI, адрес 1-384);
	uint8_t m_uiDiscreteInputSourceType;
	// јƒ–≈—1 Ц јƒ–≈—4 Ц адрес в соответствующем массиве;
	uint8_t m_uiDiscreteInputSourceAddress;

	// “—√2, “—√1 Ц тип сигнализации состо€ни€ выхода ‘ЅЋ:
	// 00 Ц нет сигнализации
	// 01 Ц предупредительна€ сигнализаци€
	// 10 Ц аварийна€ сигнализаци€
	// 11 Ц индикаци€ (ровный свет без звукового сигнала)
	uint8_t m_uiAlarmType;
	// ј–’ =1 архивировать, 0 Ц не архивировать
	bool m_bIsArchived;
	// є “ЅЋ Ц номер €чейки светового табло: (1-24), 0 Ц нет выхода на световое табло ;
	uint8_t m_uiLightPanelCellNumber;
	// Ѕайт 1:
	// ¬ыходы на ћ–1:D0=1 есть вых.на реле 1,D0=0-нет вых на реле 1 и т.д.
	// Ѕайт 2,3,4,5,6,7,8:
	// ¬ыходы на ћ–2,ћ–3,ћ–4,ћ–5,MP6,MP7,MP8:аналог.ћ–1.
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
