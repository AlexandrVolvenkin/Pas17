#ifndef CSQRTMATHEMATICALFUNCTIONALBLOCK_H
#define CSQRTMATHEMATICALFUNCTIONALBLOCK_H
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

//// для сериализации, разбора данных полученных по интерфейсам передачи данных.
//#pragma pack(push)
//#pragma pack(1)
//// Структура описателей БД математических функциональных блоков.
//// источник: PAS_05_NEW_PO.doc, стр - 9,
//// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
//struct TSqrtMathematicalFunctionalBlockDescriptor
//{
//	uint8_t uiFunctionalBlockType;
//	// Операнд Х1 – адрес первой переменной,
//	// если бит 7 этого реквизита = 0,
//	// то биты 0-6 – это номер входной аналоговой величины ANA_ADDR (1-96),
//	// если бит 7 этого реквизита = 1,
//	// то биты 0-6 – это номер расчетной величины CALC_VAL_ADDR (1-48).
//	// Если адрес переменной Х1 (Х2) = 0, то суммирование (вычитание) производится с константой С1 (С2)
//	uint8_t uiOperandAddressX1;
//	// Константы С1, С2 – в формате IEEE754.
//	// float - представление в описателе.
//	uint8_t auiConstantC1[4];
//	// Операнд Х2 – аналогично операнду Х1.
//	uint8_t uiOperandAddressX2;
//	// Константы С1, С2 – в формате IEEE754.
//	// float - представление в описателе.
//	uint8_t auiConstantC2[4];
//	// Y_ADDR – адрес в массиве расчетных величин CALC_VAL, куда помещается результат.
//	uint8_t uiResultLocationAddress;
//	// контрольная сумма ($ff+сумма байтов 1-10)    *).
//	uint8_t uiCrc;
//};
//#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
class CSqrtMathematicalFunctionalBlockJob :
	public CGeneralMathematicalFunctionalBlockJob
{
public:

	CSqrtMathematicalFunctionalBlockJob();
	virtual ~CSqrtMathematicalFunctionalBlockJob();

//	virtual void ExtractJobFromDataBase(
//		TGeneralMathematicalFunctionalBlockDescriptor* pxGeneralMathematicalFunctionalBlockDescriptor);
//	virtual void InsertJobToDataBase(
//		CGeneralMathematicalFunctionalBlockJob* pxGeneralMathematicalFunctionalBlockJob);
//    virtual void Fsm(void);

protected:
//	// Операнд Х1 – адрес первой переменной,
//	// если бит 7 этого реквизита = 0,
//	// то биты 0-6 – это номер входной аналоговой величины ANA_ADDR (1-96),
//	// если бит 7 этого реквизита = 1,
//	// то биты 0-6 – это номер расчетной величины CALC_VAL_ADDR (1-48).
//	// Если адрес переменной Х1 (Х2) = 0, то суммирование (вычитание) производится с константой С1 (С2)
//	uint8_t m_uiOperandAddressX1;
//	uint8_t m_uiOperandAddressX1SourceType;
//	// Константы С1, С2 – в формате IEEE754.
//	float m_fConstantC1;
//	// Операнд Х2 – аналогично операнду Х1.
//	uint8_t m_uiOperandAddressX2;
//	uint8_t m_uiOperandAddressX2SourceType;
//	// Константы С1, С2 – в формате IEEE754.
//	float m_fConstantC2;
//	// Y_ADDR – адрес в массиве расчетных величин CALC_VAL, куда помещается результат.
//	uint8_t m_uiResultLocationAddress;
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
class CSqrtMathematicalFunctionalBlockManager :
	public CGeneralMathematicalFunctionalBlockManager
{
public:

	CSqrtMathematicalFunctionalBlockManager();
	virtual ~CSqrtMathematicalFunctionalBlockManager();
//    virtual void Fsm(void);

protected:
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
class CSqrtMathematicalFunctionalBlockExecutor :
	public CGeneralMathematicalFunctionalBlockExecutor
{
public:

	CSqrtMathematicalFunctionalBlockExecutor();
	virtual ~CSqrtMathematicalFunctionalBlockExecutor();
	virtual void Fsm(void);

protected:
private:
};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------

#endif // CSUMMATHEMATICALFUNCTIONALBLOCK_H
