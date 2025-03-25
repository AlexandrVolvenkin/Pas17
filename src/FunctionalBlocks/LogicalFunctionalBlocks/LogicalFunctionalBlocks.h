#ifndef LOGICALFUNCTIONALBLOCKS_H_INCLUDED
#define LOGICALFUNCTIONALBLOCKS_H_INCLUDED
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


//-----------------------------------------------------------------------------------------------------
// Job section.

//-----------------------------------------------------------------------------------------------------

// ��� ������������, ������� ������ ���������� �� ����������� �������� ������.
#pragma pack(push)
#pragma pack(1)
// ��������� ���������� �� �������������� ������.
// ��������: PAS_05_PO.DOC, ��� - 3,
// 3. ���� ������ ��������� ���������� ��������
struct TGeneralLogicalFunctionalBlockDescriptor
{
	uint8_t uiFunctionalBlockType;
	// ������ ������� ����������� �������� � ��.
	// ��� ������� ���� ��������������� ����� ���� ������.
	uint8_t auiInputAddress[8];
	// D0-D4 - ��� (������): 0,1-24(0-��� ���.�� �����).
	// D5 - ��� =1 ������������, 0 � �� ������������.
	// D6 - ���1 � ��� ������������ ��������� ������ ���:
	// D7 - ���2 � ��� ������������ ��������� ������ ���:
	// 00 � ��� ������������
	// 01 � ����������������� ������������
	// 10 � ��������� ������������
	// 11 � ��������� (������ ���� ��� ��������� �������)
	uint8_t uiTalArhGrp;
	// ���� 1:
	// ������ �� ��1:D0=1 ���� ���.�� ���� 1,D0=0-��� ��� �� ���� 1 � �.�.
	// ���� 2,3,4,5,6,7,8:
	// ������ �� ��2,��3,��4,��5,MP6,MP7,MP8:������.��1.
	uint8_t auiRelayOut[8];
	// ����������� ����� ($ff+����� ������ 1-10)    *).
	uint8_t uiCrc;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------
class CGeneralLogicalFunctionalBlockJob :
	public CDfaLite,
	public CGeneralJobInterface
{
public:

//    CGeneralLogicalFunctionalBlockJob();
//    virtual ~CGeneralLogicalFunctionalBlockJob();
//    virtual void Fsm(void) {};

	virtual void SetFunctionalBlockType(uint8_t uiFunctionalBlockType)
	{
		m_uiFunctionalBlockType = uiFunctionalBlockType;
	};
	virtual uint8_t GetFunctionalBlockType(void)
	{
		return m_uiFunctionalBlockType;
	};

	virtual uint8_t* GetLinkedDiscreteOutputsPointer(void)
	{
		return m_auiLinkedDiscreteOutputs;
	};

	virtual void ExtractJobFromDataBase(TGeneralLogicalFunctionalBlockDescriptor*
										pxGeneralLogicalFunctionalBlockDescriptor) {};
//	virtual void InsertJobToDataBase(CGeneralLogicalFunctionalBlockJob*
//									 pxGeneralLogicalFunctionalBlockJob) {};

protected:
	uint8_t m_uiFunctionalBlockType;
	// ������ �� ��.
	uint8_t m_auiLinkedDiscreteOutputs[8];//[DISCRETE_OUTPUT_MODULE_MAX_NUMBER];
	uint8_t m_uiCrc;
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Manager section.

//-----------------------------------------------------------------------------------------------------
class CGeneralLogicalFunctionalBlockManager : public CDfaLite
{
public:

//	CGeneralLogicalFunctionalBlockManager();
//	virtual ~CGeneralLogicalFunctionalBlockManager();
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
class CLogicalFunctionalBlockManager :
	public CGeneralLogicalFunctionalBlockManager,
//	public CExecutor,
public CManagerInterface
{
	typedef enum
	{
		LOGICAL_FUNCTION_BLOCK_DATA_BASE_BLOCK_OFFSET = 70,
		MAX_HANDLED_LOGICAL_FUNCTIONAL_BLOCKS_NUMBER = 120,
		// ���������� ������ � ���� ������ � ����������� ���������� �������������� ������.
		NUMBER_OF_DATA_BASE_BLOCKS_OF_LOGICAL_FUNCTIONAL_BLOCKS = 12,
		// ���������� ���������� ���������� �������������� ������ � ����� ����� ���� ������.
		NUMBER_OF_LOGICAL_FUNCTIONAL_BLOCKS_IN_ONE_DATA_BASE_BLOCK = 10,
	};

	typedef enum
	{
		ABSENT_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 0,
		AND_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 1,
		AND_NOT_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 2,
		OR_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 3,
		OR_NOT_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 4,
		HISTERESIS_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 5,
		TIMER_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 6,
		TRIGGER_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 7,
		ANALOGUE_COMPARATOR_LOGICAL_FUNCTIONAL_BLOCK_TYPE = 8,
	} TLogicalFunctionalBlockType;


public:
	CLogicalFunctionalBlockManager();
	virtual ~CLogicalFunctionalBlockManager();

//	void Initialization(void);
//	void ExecutorsGathering(void);
	virtual void ExtractJobFromDataBase(void);
//	virtual void InsertJobToDataBase(void);
//	virtual void ExtractJobFromDataBase(TPlcDataBaseBlock*
//										pxDataBaseJobsBlockAddress);
//	virtual void InsertJobToDataBase(TPlcDataBaseBlock*
//									 pxDataBaseJobsBlockAddress);
	virtual void ExecutorsJobDistribution(void);

	virtual void SetDataBaseJobsBlockAddress(TPlcDataBaseBlock*
			pxDataBaseJobsBlockAddress)
	{
		m_pxDataBaseJobsBlockAddress = pxDataBaseJobsBlockAddress;
	};
	virtual TPlcDataBaseBlock*
	GetDataBaseJobsBlockAddress(void)
	{
		return m_pxDataBaseJobsBlockAddress;
	};

	virtual void SetDataBaseJobAddress(TGeneralLogicalFunctionalBlockDescriptor*
									   pxDataBaseJobAddress)
	{
		m_pxDataBaseJobAddress = pxDataBaseJobAddress;
	};
	virtual TGeneralLogicalFunctionalBlockDescriptor*
	GetDataBaseJobAddress(void)
	{
		return m_pxDataBaseJobAddress;
	};

	virtual list<CGeneralLogicalFunctionalBlockJob*>* JobsListPointer(void)
	{
		return &m_lJobsList;
	};

	virtual list<CExecutor*>* ExecutorsListPointer(void)
	{
		return &m_lExecutorsList;
	};

protected:
	std::list<CGeneralLogicalFunctionalBlockJob*> m_lJobsList;
	std::list<CExecutor*> m_lExecutorsList;

private:
	// ��������� �� ���� � ���� ������ � ����������� �������������� ������.
	TPlcDataBaseBlock*
	m_pxDataBaseJobsBlockAddress;
	// ��������� �� ��������� ��������������� �����.
	TGeneralLogicalFunctionalBlockDescriptor*
	m_pxDataBaseJobAddress;

//	TGeneralLogicalFunctionalBlockDescriptor *m_pxGeneralLogicalFunctionalBlockDescriptor;
//	uint16_t m_uiDataBaseBlocksNumber;
//	list<TGeneralLogicalFunctionalBlockDescriptor*> m_lDataBaseBlockMapList;
//CGeneralLogicalFunctionalBlockJob
};
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
// Executor section.

//-----------------------------------------------------------------------------------------------------
class CGeneralLogicalFunctionalBlockExecutor :
	public CDfaLite
{
public:
	virtual uint8_t ALARM_TYPE(void)
	{
		return 0;
	};

	virtual uint8_t ACTIVE_LEVEL(void)
	{
		return 0;
	};

//    CGeneralLogicalFunctionalBlock();
//    virtual ~CGeneralLogicalFunctionalBlock();
	virtual void SetJob(CGeneralLogicalFunctionalBlockJob*
						pxGeneralMathematicalFunctionalBlockJob) {};
//    virtual void Fsm(void) {};

	virtual void SetDiscreteStateIndex(uint8_t uiDiscreteStateIndex)
	{
		m_uiDiscreteStateIndex = uiDiscreteStateIndex;
	};
	virtual uint8_t GetDiscreteStateIndex(void)
	{
		return m_uiDiscreteStateIndex;
	};

	virtual void SetAlarmWindowIndex(uint8_t uiAlarmWindowIndex)
	{
		m_uiAlarmWindowIndex = uiAlarmWindowIndex;
	};
	virtual uint8_t GetAlarmWindowIndex(void)
	{
		return m_uiAlarmWindowIndex;
	};

	virtual uint8_t* GetLinkedDiscreteOutputsPointer(void)
	{
		return m_auiLinkedDiscreteOutputs;
	};

protected:
private:
	// ������ ���� ���������� - ��������� �����.
	uint8_t m_uiAlarmWindowIndex;
	// ������ �� ��.
	uint8_t m_auiLinkedDiscreteOutputs[1];//[DISCRETE_OUTPUT_MODULE_MAX_NUMBER];
	// �������� ����������� �������.
	uint8_t m_uiDiscreteStateIndex;
};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
#endif // LOGICALFUNCTIONALBLOCKS_H_INCLUDED
