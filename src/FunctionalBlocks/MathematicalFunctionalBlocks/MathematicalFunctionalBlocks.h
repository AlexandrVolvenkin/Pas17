#ifndef MATHEMATICALFUNCTIONALBLOCKS_H_INCLUDED
#define MATHEMATICALFUNCTIONALBLOCKS_H_INCLUDED
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <list>

#include "../Dfa.h"
//#include "../Job.h"
#include "../Manager.h"
#include "../Executor.h"
#include "../PasNewConfig.h"
#include "../AvailableResources.h"


class CGeneralMathematicalFunctionalBlockExecutor;

//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------

// для сериализации, разбора данных полученных по интерфейсам передачи данных.
#pragma pack(push)
#pragma pack(1)
// Структура описателей БД математических функциональных блоков.
// источник: PAS_05_NEW_PO.doc, стр - 9,
// 3. БАЗА ДАННЫХ ОБРАБОТКИ ДИСКРЕТНЫХ СИГНАЛОВ
struct TGeneralMathematicalFunctionalBlockDescriptor
{
	uint8_t uiFunctionalBlockType;
	// Операнд Х1 – адрес первой переменной,
	// если бит 7 этого реквизита = 0,
	// то биты 0-6 – это номер входной аналоговой величины ANA_ADDR (1-96),
	// если бит 7 этого реквизита = 1,
	// то биты 0-6 – это номер расчетной величины CALC_VAL_ADDR (1-48).
	// Если адрес переменной Х1 (Х2) = 0, то суммирование (вычитание) производится с константой С1 (С2)
	uint8_t uiOperandAddressX1;
	// Константы С1, С2 – в формате IEEE754.
	// float - представление в описателе.
	uint8_t auiConstantC1[4];
	// Операнд Х2 – аналогично операнду Х1.
	uint8_t uiOperandAddressX2;
	// Константы С1, С2 – в формате IEEE754.
	// float - представление в описателе.
	uint8_t auiConstantC2[4];
	// Y_ADDR – адрес в массиве расчетных величин CALC_VAL, куда помещается результат.
	uint8_t uiResultLocationAddress;
	// контрольная сумма ($ff+сумма байтов 1-10)    *).
	uint8_t uiCrc;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
class CGeneralMathematicalFunctionalBlockJob :
	public CDfaLite,
	public CGeneralJobInterface
{
public:
	enum
	{
		ANALOG_INPUT_VALUES_OPERAND_SOURCE = 0,
		CALCULATED_VALUES_OPERAND_SOURCE = 1,
	} EOperandAddressSourceType;

    CGeneralMathematicalFunctionalBlockJob();
    virtual ~CGeneralMathematicalFunctionalBlockJob();
//    virtual void Fsm(void) {};

	virtual void SetFunctionalBlockType(uint8_t uiFunctionalBlockType)
	{
		m_uiFunctionalBlockType = uiFunctionalBlockType;
	};
	virtual uint8_t GetFunctionalBlockType(void)
	{
		return m_uiFunctionalBlockType;
	};

	virtual uint8_t GetOperandAddressX1(void)
	{
		return m_uiOperandAddressX1;
	};
	virtual uint8_t GetOperandAddressX1SourceType(void)
	{
		return m_uiOperandAddressX1SourceType;
	};
	virtual float GetConstantC1(void)
	{
		return m_fConstantC1;
	};

	virtual uint8_t GetOperandAddressX2(void)
	{
		return m_uiOperandAddressX2;
	};
	virtual uint8_t GetOperandAddressX2SourceType(void)
	{
		return m_uiOperandAddressX2SourceType;
	};
	virtual float GetConstantC2(void)
	{
		return m_fConstantC2;
	};

	virtual uint8_t GetResultLocationAddress(void)
	{
		return m_uiResultLocationAddress;
	};

	virtual void ExtractJobFromDataBase(TGeneralMathematicalFunctionalBlockDescriptor*
										pxGeneralMathematicalFunctionalBlockDescriptor);
//	virtual void InsertJobToDataBase(CGeneralMathematicalFunctionalBlockJob*
//									 pxGeneralMathematicalFunctionalBlockJob) {};

protected:
	uint8_t m_uiFunctionalBlockType;
	// Операнд Х1 – адрес первой переменной,
	// если бит 7 этого реквизита = 0,
	// то биты 0-6 – это номер входной аналоговой величины ANA_ADDR (1-96),
	// если бит 7 этого реквизита = 1,
	// то биты 0-6 – это номер расчетной величины CALC_VAL_ADDR (1-48).
	// Если адрес переменной Х1 (Х2) = 0, то суммирование (вычитание) производится с константой С1 (С2)
	uint8_t m_uiOperandAddressX1;
	uint8_t m_uiOperandAddressX1SourceType;
	// Константы С1, С2 – в формате IEEE754.
	float m_fConstantC1;
	// Операнд Х2 – аналогично операнду Х1.
	uint8_t m_uiOperandAddressX2;
	uint8_t m_uiOperandAddressX2SourceType;
	// Константы С1, С2 – в формате IEEE754.
	float m_fConstantC2;
	// Y_ADDR – адрес в массиве расчетных величин CALC_VAL, куда помещается результат.
	uint8_t m_uiResultLocationAddress;
	uint8_t m_uiCrc;
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
class CGeneralMathematicalFunctionalBlockManager : public CDfaLite
{
public:

//	CGeneralMathematicalFunctionalBlockManager();
//	virtual ~CGeneralMathematicalFunctionalBlockManager();
//    virtual void Fsm(void) {};
	virtual void ExtractJobFromDataBase(void) {};
//	virtual void InsertJobToDataBase(void) {};
//	virtual void ExtractJobFromDataBase(TPlcDataBaseBlock*
//										pxDataBaseJobsBlockAddress) {};
//	virtual void InsertJobToDataBase(TPlcDataBaseBlock*
//									 pxDataBaseJobsBlockAddress) {};

protected:
};

//-----------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
class CMathematicalFunctionalBlockManager :
	public CGeneralMathematicalFunctionalBlockManager,
//	public CExecutor,
public CManagerInterface
{
	typedef enum
	{
		MATHEMATICAL_FUNCTIONAL_BLOCK_DATA_BASE_BLOCK_OFFSET = 82,
		MAX_HANDLED_MATHEMATICAL_FUNCTIONAL_BLOCKS_NUMBER = 48,
		// количество блоков в базе данных с описателями математических функциональных блоков.
		NUMBER_OF_DATA_BASE_BLOCKS_OF_MATHEMATICAL_FUNCTIONAL_BLOCKS = 3,
		// количество описателей математических функциональных блоков в одном блоке базы данных.
		NUMBER_OF_MATHEMATICAL_FUNCTIONAL_BLOCKS_IN_ONE_DATA_BASE_BLOCK = 16,
	};

	typedef enum
	{
		ABSENT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE = 0,
		SUM_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE = 1,
		SUBTRACTION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE = 2,
		MULTIPLICATION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE = 3,
		DIVISION_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE = 4,
		SQRT_MATHEMATICAL_FUNCTIONAL_BLOCK_TYPE = 5,
	} TMathematicalFunctionalBlockType;


public:
	CMathematicalFunctionalBlockManager();
	virtual ~CMathematicalFunctionalBlockManager();

	virtual void DestroyProductionFactors(void);
//	void Initialization(void);
//	void ExecutorsGathering(void);
	virtual void ExtractJobFromDataBase(void);
//	virtual void InsertJobToDataBase(void);
//	virtual void ExtractJobFromDataBase(TPlcDataBaseBlock*
//										pxDataBaseJobsBlockAddress);
//	virtual void InsertJobToDataBase(TPlcDataBaseBlock*
//									 pxDataBaseJobsBlockAddress);
	virtual void ExecutorsJobDistribution(void);
	virtual void Fsm(void);

	virtual void SetAvailableResourcesPointer(CAvailableResources*
			pxAvailableResources)
	{
		m_pxAvailableResources = pxAvailableResources;
	};
	virtual CAvailableResources*
	GetAvailableResourcesPointer(void)
	{
		return m_pxAvailableResources;
	};

	virtual void SetDataBaseJobAddress(TGeneralMathematicalFunctionalBlockDescriptor*
									   pxDataBaseJobAddress)
	{
		m_pxDataBaseJobAddress = pxDataBaseJobAddress;
	};
	virtual TGeneralMathematicalFunctionalBlockDescriptor*
	GetDataBaseJobAddress(void)
	{
		return m_pxDataBaseJobAddress;
	};

	virtual list<CGeneralMathematicalFunctionalBlockJob*>* JobsListPointer(void)
	{
		return &m_lJobsList;
	};

	virtual list<CGeneralMathematicalFunctionalBlockExecutor*>* ExecutorsListPointer(void)
	{
		return &m_lExecutorsList;
	};

protected:
	std::list<CGeneralMathematicalFunctionalBlockJob*> m_lJobsList;
	std::list<CGeneralMathematicalFunctionalBlockExecutor*> m_lExecutorsList;

private:
	CAvailableResources* m_pxAvailableResources;
	// указатель на блок в базе данных с описателями функциональных блоков.
	TPlcDataBaseBlock*
	m_pxDataBaseJobsBlockAddress;
	// указатель на описатель функционального блока.
	TGeneralMathematicalFunctionalBlockDescriptor*
	m_pxDataBaseJobAddress;

//	TGeneralMathematicalFunctionalBlockDescriptor *m_pxGeneralMathematicalFunctionalBlockDescriptor;
//	uint16_t m_uiDataBaseBlocksNumber;
//	list<TGeneralMathematicalFunctionalBlockDescriptor*> m_lDataBaseBlockMapList;
//CGeneralMathematicalFunctionalBlockJob
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
class CGeneralMathematicalFunctionalBlockExecutor :
	public CDfaLite
{
public:

    CGeneralMathematicalFunctionalBlockExecutor();
    virtual ~CGeneralMathematicalFunctionalBlockExecutor();
//	virtual void SetJob(CGeneralMathematicalFunctionalBlockJob*
//						pxGeneralMathematicalFunctionalBlockJob) {};
	virtual void SetJob(CAvailableResources*
						pxAvailableResources,
						CGeneralMathematicalFunctionalBlockJob*
						pxGeneralMathematicalFunctionalBlockJob);
//	virtual void AllocateResources(CAvailableResources*
//								   pxAvailableResources) {};
//    virtual void Fsm(void) {};

protected:
	float* m_pfOperandX1;
	// Константы С1, С2 – в формате IEEE754.
	float m_fConstantC1;
	float* m_pfOperandX2;
	// Константы С1, С2 – в формате IEEE754.
	float m_fConstantC2;
	float* m_pfResultLocation;
};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
#endif // MATHEMATICALFUNCTIONALBLOCKS_H_INCLUDED
