//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#ifndef CMESSAGEBOX_H
#define CMESSAGEBOX_H


class CDataContainerInterface;

//-------------------------------------------------------------------------------
class CMessageBoxInterface
{
    public:

    protected:

    private:
};

//-------------------------------------------------------------------------------





//-------------------------------------------------------------------------------
class CMessageBoxGeneral : public CMessageBoxInterface
{
public:
    CMessageBoxGeneral();
    virtual ~CMessageBoxGeneral();

    void SetMessage(uint16_t uiDataIndex,
                          uint8_t *puiDataPointer,
                          uint32_t uiDataOffset,
                          uint32_t uiDataLength);

private:
    uint8_t m_uiFsmCommandState;
    CDataContainerInterface* m_pxCommandDataPointer;
    uint16_t m_uiDataIndex;
    uint8_t* m_puiDataPointer;
    uint32_t m_uiDataOffset;
    uint32_t m_uiDataLength;
};

//-------------------------------------------------------------------------------
#endif // CMESSAGEBOX_H
