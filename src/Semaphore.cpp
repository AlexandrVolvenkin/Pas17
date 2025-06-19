
//-------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-------------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdlib>   // ��� ������� exit()
#include <string>   // ��� ������������� std::to_string()
#include <cerrno>  // ��� ������������� errno
#include <unistd.h>  // ��� ������� sleep()
#include <sys/types.h> // ��� ����� ������ ���� �������� � �������
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include "Semaphore.h"

//-------------------------------------------------------------------------------
CSemaphore::CSemaphore()
{
    //ctor
}

//-------------------------------------------------------------------------------
CSemaphore::CSemaphore(const char* key, int value)
{
    key_t keyVal = ftok(key, 'A');
    if (keyVal == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    semId = semget(keyVal, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (semId == -1)
    {
        if (errno == EEXIST)
        {
            std::cerr << "Semaphore already exists. Try another key.\n";
            exit(EXIT_FAILURE);
        }
        else
        {
            perror("semget");
            exit(EXIT_FAILURE);
        }
    }

    semctl(semId, 0, SETVAL, value); // ��������� ���������� �������� ��������
}

//-------------------------------------------------------------------------------
CSemaphore::CSemaphore(key_t keyVal, int value)
{
    std::cout << "CSemaphore::CSemaphore 1"  << std::endl;
//    key_t keyVal = ftok(key, 'A');
//    if (keyVal == -1)
//    {
//        perror("ftok");
//        exit(EXIT_FAILURE);
//    }

// �������� �������� ������� � ������ ������� IPC_CREAT. ���� ������� ��� ����������
// ����� ������ errno == EEXIST. � ���� ������ �� ������������� �������� � ������:
// semctl(semId, 0, SETVAL, value); // ��������� ���������� �������� ��������
    semId = semget(keyVal, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (semId == -1)
    {
        std::cout << "CSemaphore::CSemaphore 2"  << std::endl;
        if (errno == EEXIST)
        {
            std::cout << "CSemaphore::CSemaphore 3"  << std::endl;
            std::cerr << "Semaphore already exists. Try another key.\n";
            // �������� ��� ������������ �������.
            semId = semget(keyVal, 1, IPC_EXCL | S_IRUSR | S_IWUSR);
            if (semId == -1)
            {
                std::cout << "CSemaphore::CSemaphore 32"  << std::endl;
                perror("semget");
            }
            else
            {
                std::cout << "CSemaphore::CSemaphore 35"  << std::endl;
            }
        }
        else
        {
            std::cout << "CSemaphore::CSemaphore 36"  << std::endl;
        }
    }
    else
    {
        std::cout << "CSemaphore::CSemaphore 5"  << std::endl;
        semctl(semId, 0, SETVAL, value); // ��������� ���������� �������� ��������
    }
}

//-------------------------------------------------------------------------------
CSemaphore::~CSemaphore()
{
    std::cout << "CSemaphore::~CSemaphore 1"  << std::endl;
    semctl(semId, 0, IPC_RMID); // �������� ��������
}

//-------------------------------------------------------------------------------
bool CSemaphore::Acquire()
{
//    std::cout << "CSemaphore::Acquire 1"  << std::endl;
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1; // ���������� �������� �������� �� 1
    sops.sem_flg = SEM_UNDO;
//    sops.sem_flg = IPC_NOWAIT;

    int result = semop(semId, &sops, 1);
    if (result == -1)
    {
        std::cout << "CSemaphore::Acquire 2"  << std::endl;
        perror("semop");
        return false;
    }
    else
    {
//        std::cout << "CSemaphore::Acquire 3"  << std::endl;
        return true;
    }
}

//-------------------------------------------------------------------------------
bool CSemaphore::Release()
{
//    std::cout << "CSemaphore::Release 1"  << std::endl;
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 1; // ���������� �������� �������� �� 1
    sops.sem_flg = SEM_UNDO;

    int result = semop(semId, &sops, 1);
    if (result == -1)
    {
        std::cout << "CSemaphore::Release 2"  << std::endl;
        perror("semop");
        return false;
    }
    else
    {
//        std::cout << "CSemaphore::Release 3"  << std::endl;
        return true;
    }
}

//-------------------------------------------------------------------------------
