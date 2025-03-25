#ifndef CMANAGER_H
#define CMANAGER_H
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <list>
#include "Job.h"
#include "Executor.h"

//-----------------------------------------------------------------------------------------------------
class CManagerInterface
{
public:
	CManagerInterface();
	virtual ~CManagerInterface();

	virtual void Initialization(void) {};
	virtual void ExecutorsGathering(void) {};
	virtual void ExecutorsJobDistribution(void) {};

//	virtual void ExtractJobFromDataBase(uint16_t uiJobBlockIndex) {};
//	virtual void InsertJobToDataBase(uint16_t uiJobBlockIndex) {};
//	virtual void ExtractJobFromDataBase(void) {};
//	virtual void InsertJobToDataBase(void) {};

//	void SetJobBlockIndex(uint16_t uiJobBlockIndex)
//	{
//		m_uiJobBlockIndex = uiJobBlockIndex;
//	};
//
//	uint16_t GetJobBlockIndex(void)
//	{
//		return m_uiJobBlockIndex;
//	};

	virtual void SetJobsNumber(uint16_t uiJobsNumber)
	{
		m_uiJobsNumber = uiJobsNumber;
	};
	virtual uint16_t GetJobsNumber(void)
	{
		return m_uiJobsNumber;
	};

	virtual void SetExecutorsNumber(uint16_t uiExecutorsNumber)
	{
		m_uiExecutorsNumber = uiExecutorsNumber;
	};
	virtual uint16_t GetExecutorsNumber(void)
	{
		return m_uiExecutorsNumber;
	};

//	virtual list<CGeneralJobInterface*>* JobsListPointer(void)
//	{
//		return &m_lJobsList;
//	};
//
//	virtual list<CExecutor*>* ExecutorsListPointer(void)
//	{
//		return &m_lExecutorsList;
//	};

protected:
//	uint16_t m_uiJobBlockIndex;
	uint8_t m_uiManagerType;
	uint16_t m_uiJobsNumber;
//	std::list<CGeneralJobInterface*> m_lJobsList;
	uint16_t m_uiExecutorsNumber;
//	std::list<CExecutor*> m_lExecutorsList;

};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
#endif // CMANAGER_H
