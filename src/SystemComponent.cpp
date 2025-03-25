//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include <iostream>
#include "SystemComponent.h"

using namespace std;

////-----------------------------------------------------------------------------------------------------
//CSystemComponentInterface::CSystemComponentInterface()
//{
//
//    SetType(1);
//}
//
////-----------------------------------------------------------------------------------------------------
//CSystemComponentInterface::~CSystemComponentInterface()
//{
//
//}
////-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CSystemComponent::CSystemComponent()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponent::CSystemComponent(uint8_t uiAddress)
{
    cout << "CSystemComponent constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}

//-----------------------------------------------------------------------------------------------------
CSystemComponent::~CSystemComponent()
{

}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponent::IsAbleToReplace(uint8_t uiType)
{
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        cout << "m_auiAbleToReplace[i] " << std::showbase << std::dec << (int)m_auiAbleToReplace[i] << " " << (int)i << " " << (int)uiType << std::endl;
        if (m_auiAbleToReplace[i])
        {
            if (m_auiAbleToReplace[i] == uiType)
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------
bool CSystemComponent::IsReadyToStartWork(void)
{

//    for (int i = 0;
//            i < INTERNAL_MODULE_QUANTITY;
//            i++)
//    {
//        if (m_apxSystemComponents[i] -> IsAbleToReplace(xPlcConfigData.xConfigData.aucModuleType[i]))
//        {
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//
//    }


}

//-----------------------------------------------------------------------------------------------------






//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvdi5::CSystemComponentModuleMvdi5()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvdi5::CSystemComponentModuleMvdi5(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMvdi5 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvdi5::~CSystemComponentModuleMvdi5()
{

}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvds9::CSystemComponentModuleMvds9()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvds9::CSystemComponentModuleMvds9(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMvds9 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvds9::~CSystemComponentModuleMvds9()
{

}
//-----------------------------------------------------------------------------------------------------







//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvps3::CSystemComponentModuleMvps3()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvps3::CSystemComponentModuleMvps3(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMvps3 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvps3::~CSystemComponentModuleMvps3()
{

}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvai5::CSystemComponentModuleMvai5()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvai5::CSystemComponentModuleMvai5(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMvai5 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvai5::~CSystemComponentModuleMvai5()
{

}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvst3::CSystemComponentModuleMvst3()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvst3::CSystemComponentModuleMvst3(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMvst3 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvst3::~CSystemComponentModuleMvst3()
{

}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvao3::CSystemComponentModuleMvao3()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvao3::CSystemComponentModuleMvao3(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMvao3 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMvao3::~CSystemComponentModuleMvao3()
{

}
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr51::CSystemComponentModuleMr51()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr51::CSystemComponentModuleMr51(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMr51 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr51::~CSystemComponentModuleMr51()
{

}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr53::CSystemComponentModuleMr53()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr53::CSystemComponentModuleMr53(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMr53 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr53::~CSystemComponentModuleMr53()
{
}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr54::CSystemComponentModuleMr54()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr54::CSystemComponentModuleMr54(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMr54 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr54::~CSystemComponentModuleMr54()
{
}
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr55::CSystemComponentModuleMr55()
{

}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr55::CSystemComponentModuleMr55(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMr55 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMr55::~CSystemComponentModuleMr55()
{
}
//-----------------------------------------------------------------------------------------------------








//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMtvi5::CSystemComponentModuleMtvi5()
{
}


//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMtvi5::CSystemComponentModuleMtvi5(uint8_t uiAddress)
{
    cout << "CSystemComponentModuleMtvi5 constructor " << std::endl;
    m_uiAddress = uiAddress;
    for (uint8_t i = 0;
            (i < REPLACEMENT_TYPES_NUMBER);
            i++)
    {
        m_auiAbleToReplace[i] =
            m_auiAbleToReplaceInitialization[i];
    }
}

//-----------------------------------------------------------------------------------------------------
CSystemComponentModuleMtvi5::~CSystemComponentModuleMtvi5()
{
}
//-----------------------------------------------------------------------------------------------------
