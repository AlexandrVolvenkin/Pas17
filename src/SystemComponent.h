//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CSYSTEMCOMPONENT_H
#define CSYSTEMCOMPONENT_H

#include <stdint.h>

enum
{
    MISSING_COMPONENT = 0,
    NORMAL_WORK,
    // В отсутствие компонента можно продолжать работу.
    VACATION_COMPONENT,
};

//-----------------------------------------------------------------------------------------------------
class CSystemComponentInterface
{
public:
    enum
    {
        REPLACEMENT_TYPES_NUMBER = 8,
    };

    virtual uint8_t GetType(void) {};
    virtual uint8_t GetWorkingStatus(void) {};
    virtual void SetAddress(uint8_t uiAddress) {};
    virtual uint8_t GetAddress(void) {};
    virtual bool IsReadyToStartWork(void) {};
    virtual bool IsAbleToReplace(uint8_t uiType) {};
};

//-----------------------------------------------------------------------------------------------------










//-----------------------------------------------------------------------------------------------------
class CSystemComponent : public CSystemComponentInterface
{
public:
    CSystemComponent();
    CSystemComponent(uint8_t muiAddress);
    virtual ~CSystemComponent();

    uint8_t GetType(void)
    {
        return MISSING_COMPONENT;
    };

    uint8_t GetWorkingStatus(void)
    {
        return MISSING_COMPONENT;
    };

    void SetAddress(uint8_t uiAddress)
    {
        m_uiAddress = uiAddress;
    }
    uint8_t GetAddress(void)
    {
        return m_uiAddress;
    };

    bool IsReadyToStartWork(void);
    bool IsAbleToReplace(uint8_t uiType);

private:

protected:
    uint8_t m_uiAddress;
    // Массив с типами компонентов, которые может заменять.
    uint8_t m_auiAbleToReplace[REPLACEMENT_TYPES_NUMBER];

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------









//-----------------------------------------------------------------------------------------------------
enum
{
    MODULE_TYPE_MVDI = 0x01,
};

class CSystemComponentModuleMvdi5 : public CSystemComponent
{
public:
    CSystemComponentModuleMvdi5();
    CSystemComponentModuleMvdi5(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMvdi5();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MVDI;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MVDI,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
enum
{
    MODULE_TYPE_MVDS9 = 0x0A,
};

class CSystemComponentModuleMvds9 : public CSystemComponent
{
public:
    CSystemComponentModuleMvds9();
    CSystemComponentModuleMvds9(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMvds9();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MVDS9;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MVDS9,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------









//-----------------------------------------------------------------------------------------------------
enum
{
    MODULE_TYPE_MVPS3 = 0x06,
    MODULE_TYPE_MVAI5 = 0x07,
    MODULE_TYPE_MVST3 = 0x08,
    MODULE_TYPE_MVAO3 = 0x09,
};

class CSystemComponentModuleMvps3 : public CSystemComponent
{
public:
    CSystemComponentModuleMvps3();
    CSystemComponentModuleMvps3(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMvps3();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MVPS3;
    };

    uint8_t GetWorkingStatus(void)
    {
        return NORMAL_WORK;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MVPS3,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
class CSystemComponentModuleMvai5 : public CSystemComponent
{
public:
    CSystemComponentModuleMvai5();
    CSystemComponentModuleMvai5(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMvai5();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MVAI5;
    };

    uint8_t GetWorkingStatus(void)
    {
        return NORMAL_WORK;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MVAI5,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
class CSystemComponentModuleMvst3 : public CSystemComponent
{
public:
    CSystemComponentModuleMvst3();
    CSystemComponentModuleMvst3(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMvst3();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MVST3;
    };

    uint8_t GetWorkingStatus(void)
    {
        return NORMAL_WORK;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MVST3,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------
class CSystemComponentModuleMvao3 : public CSystemComponent
{
public:
    CSystemComponentModuleMvao3();
    CSystemComponentModuleMvao3(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMvao3();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MVAO3;
    };

    uint8_t GetWorkingStatus(void)
    {
        return NORMAL_WORK;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MVAO3,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------










//-----------------------------------------------------------------------------------------------------
enum
{
    MODULE_TYPE_MR53 = 0x03,
    MODULE_TYPE_MR54 = 0x04,
    MODULE_TYPE_MR55 = 0x05,
    MODULE_TYPE_MR51 = 0x0C,
};

class CSystemComponentModuleMr51 : public CSystemComponent
{
public:
    CSystemComponentModuleMr51();
    CSystemComponentModuleMr51(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMr51();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MR51;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MR53,
        MODULE_TYPE_MR54,
        MODULE_TYPE_MR55,
        MODULE_TYPE_MR51,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
class CSystemComponentModuleMr53 : public CSystemComponent
{
public:
    CSystemComponentModuleMr53();
    CSystemComponentModuleMr53(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMr53();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MR53;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MR53,
        MODULE_TYPE_MR54,
        MODULE_TYPE_MR55,
        MODULE_TYPE_MR51,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
class CSystemComponentModuleMr54 : public CSystemComponent
{
public:
    CSystemComponentModuleMr54();
    CSystemComponentModuleMr54(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMr54();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MR54;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MR53,
        MODULE_TYPE_MR54,
        MODULE_TYPE_MR55,
        MODULE_TYPE_MR51,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
class CSystemComponentModuleMr55 : public CSystemComponent
{
public:
    CSystemComponentModuleMr55();
    CSystemComponentModuleMr55(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMr55();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MR55;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MR53,
        MODULE_TYPE_MR54,
        MODULE_TYPE_MR55,
        MODULE_TYPE_MR51,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------










//-----------------------------------------------------------------------------------------------------
enum
{
    MODULE_TYPE_MTVI5 = 0x0E,
};

class CSystemComponentModuleMtvi5 : public CSystemComponent
{
public:
    CSystemComponentModuleMtvi5();
    CSystemComponentModuleMtvi5(uint8_t muiAddress);
    virtual ~CSystemComponentModuleMtvi5();

    uint8_t GetType(void)
    {
        return MODULE_TYPE_MTVI5;
    };

    uint8_t GetWorkingStatus(void)
    {
        return VACATION_COMPONENT;
    };

private:
    // Массив с типами компонентов, которые может заменять.
    const uint8_t m_auiAbleToReplaceInitialization[REPLACEMENT_TYPES_NUMBER] =
    {
        MODULE_TYPE_MTVI5,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
};

//-----------------------------------------------------------------------------------------------------

#endif // CSYSTEMCOMPONENT_H
