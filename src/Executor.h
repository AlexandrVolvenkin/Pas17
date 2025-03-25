#ifndef CEXECUTOR_H
#define CEXECUTOR_H
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <string>

using namespace std;

class CManagerInterface;

//-----------------------------------------------------------------------------------------------------
class CExecutorInterface
{
public:

	virtual void SetName(std::string sName) {};
	virtual std::string GetName(void) {};
	virtual void SetManager(CManagerInterface* pxManager) {};
	virtual CManagerInterface* GetManager(void) {};
//	virtual void SetExercise(CExerciseInterface* pxExercise) {};
//	virtual CExerciseInterface* GetExercise(void) {};
};
//-----------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------
class CExecutor : public CExecutorInterface
{
public:
	CExecutor();
	virtual ~CExecutor();

	void SetName(std::string sName)
	{
		m_sName = sName;
	}

	std::string GetName(void)
	{
		return m_sName;
	}

	void SetManager(CManagerInterface* pxManager)
	{
		m_pxManager = pxManager;
	}

	CManagerInterface* GetManager(void)
	{
		return m_pxManager;
	}

//	void SetExercise(CExerciseInterface* pxExercise)
//	{
//		m_pxExercise = pxExercise;
//	}
//
//	CExerciseInterface* GetExercise(void)
//	{
//		return m_pxExercise;
//	}

protected:
	std::string m_sName;
	CManagerInterface* m_pxManager;
//	CExerciseInterface* m_pxExercise;
};
//-----------------------------------------------------------------------------------------------------
#endif // CEXECUTOR_H
