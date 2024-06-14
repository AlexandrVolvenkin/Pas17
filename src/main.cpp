//-------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------




#include <iostream>

#include "Dfa.h"
#include "MainProductionCycle.h"

//-------------------------------------------------------------------------------
using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    CMainProductionCycle xMainProductionCycle;

    while (1)
    {
        xMainProductionCycle.Fsm();
    }

    return 0;
}

//-------------------------------------------------------------------------------


