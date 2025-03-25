#ifndef CAVAILABLERESOURCES_H
#define CAVAILABLERESOURCES_H
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <stdint.h>

//-----------------------------------------------------------------------------------------------------
class CAvailableResources
{
public:
	CAvailableResources();
	virtual ~CAvailableResources();
	virtual void Initialize(void);

	virtual void InitializeAnalogInputValuesPointer(float* pfAnalogInputValues)
	{
		m_pfAnalogInputValues = pfAnalogInputValues;
		m_uiUsedAnalogInputValues = 0;
	};
	virtual float* AllocateAnalogInputValuesPointer(uint16_t uiUsedAnalogInputValues)
	{
		float* pfAnalogInputValues;
		pfAnalogInputValues = &m_pfAnalogInputValues[m_uiUsedAnalogInputValues];
		m_uiUsedAnalogInputValues += uiUsedAnalogInputValues;
		return pfAnalogInputValues;
	};
	virtual float* GetAnalogInputValuesPointer(uint16_t uiIndex)
	{
		return &m_pfAnalogInputValues[uiIndex];
	};

	virtual void InitializeCalculatedValuesPointer(float* pfCalculatedValues)
	{
		m_pfCalculatedValues = pfCalculatedValues;
		m_uiUsedCalculatedValues = 0;
	};
	virtual float* AllocateCalculatedValuesPointer(uint16_t uiUsedCalculatedValues)
	{
		float *pfCalculatedValues;
		pfCalculatedValues = &m_pfCalculatedValues[m_uiUsedCalculatedValues];
		m_uiUsedCalculatedValues += uiUsedCalculatedValues;
		return pfCalculatedValues;
	};
	virtual float* GetCalculatedValuesPointer(uint16_t uiIndex)
	{
		return &m_pfCalculatedValues[uiIndex];
	};

protected:
	// измеренные значения входных аналоговых сигналов.
	float* m_pfAnalogInputValues;
	uint16_t m_uiUsedAnalogInputValues;
	// значения расчётных величин.
	float* m_pfCalculatedValues;
	uint16_t m_uiUsedCalculatedValues;
	uint8_t *m_puiDiscreteInputs;
	uint16_t m_uiUsedDiscreteInputs;
	uint8_t *m_puiDiscreteOutputs;
	uint16_t m_uiUsedDiscreteOutputs;
	uint8_t *m_puiDiscreteOutputState;
	uint16_t m_uiUsedDiscreteOutputState;
	uint8_t* m_puiErrorAlarmDataArray;
	uint8_t m_uiUsedErrorAlarmDataArray;
};
//-----------------------------------------------------------------------------------------------------
#endif // CAVAILABLERESOURCES_H
