#ifndef CJOB_H
#define CJOB_H
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <list>

////-----------------------------------------------------------------------------------------------------
//class CJob
//{
//public:
//	CJob();
//	virtual ~CJob();
//protected:
//private:
//};
////-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
class CGeneralJobInterface
{
public:
	CGeneralJobInterface();
	virtual ~CGeneralJobInterface();

//	virtual void ExtractJobFromDataBase(uint16_t uiJobDescriptorIndex) {};
//	virtual void InsertJobToDataBase(uint16_t uiJobDescriptorIndex) {};

//	void SetJobDescriptorIndex(uint16_t uiJobDescriptorIndex)
//	{
//		m_uiJobDescriptorIndex = uiJobDescriptorIndex;
//	};
//
//	uint16_t GetJobDescriptorIndex(void)
//	{
//		return m_uiJobDescriptorIndex;
//	};

	void SetJobType(uint16_t uiJobType)
	{
		m_uiJobType = uiJobType;
	};

	uint16_t GetJobType(void)
	{
		return m_uiJobType;
	};

protected:
private:
//	uint16_t m_uiJobDescriptorIndex;
	uint16_t m_uiJobType;
};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
#endif // CJOB_H
