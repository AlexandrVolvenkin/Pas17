
//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------

#include <iostream>
#include <string.h>

#include "MainProductionCycle.h"
#include "PasNewConfig.h"
#include "Platform.h"
#include "Events.h"
#include "Task.h"
#include "EventsDB.h"
#include "RingBuffer.h"
#include "SystemComponentsGroup.h"
#include "Parse.h"
#include "AvailableResources.h"
#include "FunctionalBlocks/MathematicalFunctionalBlocks/MathematicalFunctionalBlocks.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
CMainProductionCycle::CMainProductionCycle()
{
//    pxDataStoreFileSystem = new CDataStore(new CStorageDeviceFileSystem);
	m_pxInternalModulesGroup = new CSystemComponentsGroupInternalModules;
	m_pxInternalModulesGroup -> SetCommunicationDevice(&xSpiCommunicationDevice);
	SetFsmState(SYSTEM_INITIALIZATION);
	cout << "CMainProductionCycle construct" << endl;
}

//-----------------------------------------------------------------------------------------------------
CMainProductionCycle::~CMainProductionCycle()
{
//    delete pxDataStoreFileSystem;
	delete m_pxInternalModulesGroup;
	cout << "CMainProductionCycle destruct" << endl;
}

//-----------------------------------------------------------------------------------------------------
bool CMainProductionCycle::IsDataStoreOk(void)
{
	if (pxDataStoreFileSystem -> Check())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::GlobalVariablesInitInitialization(void)
{
	fbPlcErrorReset = 0;
	fbGlobalKvit = 0;
	fbGlobalReset = 0;
	fbGlobalBlock = 0;
	fbOneSecondPassed = 0;
	fbTenSecondsPassed = 0;
	nucOneSecondCounter = 0;
	nucTenSecondsCounter = 0;
	ucAlarmStateCommon = 0;
	fucZvkMal = 0;
	fucKvSbBef = 0;
	nucBlinkCounter = 0;
	fbBlinkIsOn = 0;
	fbDiscreteInputDataBaseError = 0;
	fbFunctionBlockDataBaseError = 0;
	fbPlcDataBaseFileError = 0;
	fbPlcConfigurationFileError = 0;
	fbPlcConfigurationError = 0;
	fbAnalogueInputModuleDataBaseError = 0;
	fbDiscreteOutputModuleDataBaseError = 0;
	fbLocalNetworkModuleDataBaseError = 0;
	fbAnalogueInputModuleReperPointsAdcDataBaseError = 0;
	fbMindDataExchangeError = 0;
	fbCodeSysDataExchangeError = 0;
	fbModuleMrOutOff = 0;
	ui8CurrentTimeUpdateDelayCounter = 0;
	ui8CurrentTimeSaveDelayCounter = 0;

	memset(&xMainFlagRegister,
		   0,
		   sizeof(xMainFlagRegister));
	memset(&xPlcConfigDataTemp,
		   0,
		   sizeof(struct TPlcConfigData));
	memset(&axAlarmHmi[0],
		   0,
		   sizeof(axAlarmHmi));

	CEvents::OccuredEventsControlReset();
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::PlatformComponentsInitialization(void)
{
	CGpio::Initialization();
	cout << "CGpio::Initialization" << endl;
	CPlatform::LedInitialization();
	cout << "CPlatform::LedInitialization" << endl;
	xSpiCommunicationDevice.Initialization();
	cout << "xSpiCommunicationDevice.Initialization" << endl;
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::MainTaskInitialization(void)
{
	CSettingsRW::PlcSettingsCheck();
	iPlcDataBaseCheck();
	CSettingsRW::PlcSettingsLoad();
	iPlcConfigurationCheck();
	vAnalogueInputDescriptionWorkExstract();
	vAnalogueInputDescriptionWorkToModbus();
	vDimentionsParametersDataBaseExstract();
	vModbusMasterModuleDataBaseExstract();
	vAnalogueOutputDescriptionWorkExstract();
	vAnalogueOutputDescriptionWorkToModbus();
	vDiscreteInputDataBaseExstract();
	vDiscreteSygnalTextDescriptorDataBaseExstract();
	vFunctionBlockDataBaseExstract();



//-----------------------------------------------------------------------------------------------------
// новый вариант.
	delete m_pxAvailableResources;
	m_pxAvailableResources = new CAvailableResources;
	m_pxAvailableResources -> Initialize();

	delete m_pxMathematicalFunctionalBlockManager;
	m_pxMathematicalFunctionalBlockManager =
		new CMathematicalFunctionalBlockManager;
	m_pxMathematicalFunctionalBlockManager ->
	SetAvailableResourcesPointer(m_pxAvailableResources);
	m_pxMathematicalFunctionalBlockManager ->
	ExtractJobFromDataBase();
	m_pxMathematicalFunctionalBlockManager ->
	ExecutorsJobDistribution();
//-----------------------------------------------------------------------------------------------------




//	// вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
//	xCCrc.CreateAll();
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::PlatformComponentsGathering(void)
{
	m_pxInternalModulesGroup -> Gathering();
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::MainTaskComponentsInformationCollection(void)
{
	PlatformComponentsGathering();

	iPlcConfigurationCreate();
	vMainTaskMemoryMapFormation();
}

//-----------------------------------------------------------------------------------------------------
bool CMainProductionCycle::IsSpecificationCorrect(void)
{
	bool bResult = true;
	if (!(m_pxInternalModulesGroup -> IsSpecificationCorrect()))
	{
		bResult = false;
	}

	if (bResult)
	{
		return true;
	}
	else
	{
		CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
		return false;
	}
}

//-----------------------------------------------------------------------------------------------------
bool CMainProductionCycle::IsDataBaseCorrect(void)
{
	if (fbPlcDataBaseFileError)
	{
		printf("DataBase Error Flow Stop.\n");

		// активное состояние события ещё не зарегистрировано?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET))
		{
			// зарегистрируем активное состояние события.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE |
				 CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
				SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET,
				"Ошибка общей б.д");
		}

		return false;
	}
	else
	{
		return true;
	}
}

//-----------------------------------------------------------------------------------------------------
bool CMainProductionCycle::IsConfigurationCorrect(void)
{
	if (fbPlcConfigurationError)
	{
		aucCoilsArray[CONFIGURATION_ERROR_SIGNAL_BIT_ARRAY_OFFSET] = 1;
		printf("Configuration Error Flow Stop.\n");

		// активное состояние события ещё не зарегистрировано?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET))
		{
			// зарегистрируем активное состояние события.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
				SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET,
				"Ошибка конфигурации");
		}

		return false;
	}
	else
	{
		return true;
	}
}


extern struct timeval  xTimeCur;
extern struct timeval xTimeLast;
extern struct timeval  xTimeMax;
extern int iTimeMeasureCounter;


// обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1(MmcCopy.sh), после его окончания.
//-----------------------------------------------------------------------------------------------------
void MainActionHandler(int signo, siginfo_t *info, void *context)
{
	ui8MainRestart = 1;
	cout << "action MainRestart" << endl;
}

//// обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1(MmcCopy.sh), после его окончания.
////-----------------------------------------------------------------------------------------------------
//void ArchiveSaveHandler(int signo, siginfo_t *info, void *context)
//{
//    for(int i = 0; i < 10; i++)
//    {
//        cout << "action ArchiveSaveHandler" << endl;
//        usleep(100000);
//    }
//}

pthread_t xSystemDiskCopyDisplay;
//-----------------------------------------------------------------------------------------------------
// поток копирования диска mmc0 на mmc1.
void *thread_SystemDiskCopyDisplay(void *value)
{
//    // удалим файлы настроек, конфигурации, базы данных.
//    system("sudo rm /home/debian/PasConfig.dat");
//    system("sudo rm /home/debian/ReservPasConfig.dat");
//    system("sudo rm /home/debian/PasDataBase.dat");
//    system("sudo rm /home/debian/ReservPasDataBase.dat");
//    system("sudo rm /home/debian/Pas05ArchiveDataBase.db");
//    system("sudo rm /home/debian/PasSettings.dat");
	// запустим сценарий копирования диска mmc0 на mmc1(MmcCopy.sh).
	// отправим команду.
	system("sudo /home/debian/MmcCopy.sh");
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::MainTaskInitializationEnd(void)
{
	// откроем базу данных.
	if (xCArchiveEventsDB.Connect())
	{
		// error.
	}

	// вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
	xPlcConfigService.usCrc =
		usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
									sizeof(xPlcConfigService.xPlcConfigServiceData));

	ucModbusSlaveAddress = (xPlcDataBase.
							axPlcDataBaseBlocks[NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET].
							aucPlcDataBaseBlockData[0]) + 1;

	usleep(5000);
	vThreadsCreate();
	SharedMemoryCreate();
	usleep(5000);

	xCCrc.CreateAll();

	xCMainCycleMeasureRingBuffer.Create(64);
//            xCCodeSysCycleMeasureRingBuffer.Create(64);
//            xCCodeSysCycleMeasureRingBuffer2.Create(64);
//            xCCodeSysCycleMeasureRingBuffer3.Create(64);
	CAlarm::PlcOnOffEvetnsCreate();

	iTimerCreate(&timerid,
				 &sev);
	iTimerSetTime(&timerid,
				  &its,
				  MAIN_CYCLE_TIME);
	xProgrammTimeMeasure.Begin();
	ucMainCycleTimeStart = 0;

}

int iTempCounterConfigurationTrror = 0;
//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::Fsm(void)
{
	xTimeMax.tv_usec = 0;
	iTimeMeasureCounter = 0;

	uint8_t ui8FlowControl = 0;
	uint8_t ui8MtviCounter = 0;
	unsigned char nucIndexNumber;
	uint8_t ui8ExchangeCounter = 0;

	while (1)
	{
		switch (GetFsmState())
		{
		case IDDLE:
			break;

		case DATA_STORE_CHECK:
			if (IsDataStoreOk())
			{
				SetFsmState(SYSTEM_INITIALIZATION);
			}
			else
			{
				SetFsmState(EMERGENCY_CYCLE);
			}
			break;

		case SYSTEM_INITIALIZATION:
			// можно перезагрузить
			GlobalVariablesInitInitialization();
			// можно перезагрузить
			CodeSysStart();
			// нельзя перезагрузить
			PlatformComponentsInitialization();

			// можно перезагрузить
			MainTaskInitialization();
			// можно перезагрузить
			MainTaskComponentsInformationCollection();
			if (IsSpecificationCorrect())
			{
				SetFsmState(MAIN_CYCLE_START_WAITING);
			}
			else
			{
				SetFsmState(EMERGENCY_CYCLE);
			}

			// нельзя перезагрузить
			MainTaskInitializationEnd();

			// Ожидаем перезагрузки дисплея МИНД.
			for (uint8_t i = 0; i < 20; i++)
			{
				usleep(100000);
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
			}

			IsDataBaseCorrect();
			IsConfigurationCorrect();

			xCTimeMeasure.Begin();

//            // Linux загружен с диска mmc0?
//            if (xCArchiveSaveParse.CheckMountedDiskMmc0())
//            {
//                // Linux загружен с диска mmc0.
//                // запустим сценарий копирования диска mmc0 на mmc1.
//                // создадим обработчик сигнала посылаемого из сценария копирования диска mmc0 на mmc1, после его окончания.
//                if (iHandlerCreate(SIGRTMIN+3,
//                                   &MainAction,
//                                   MainActionHandler) == - 1)
//                {
//                    cout << "Failed to create the action MainAction" << endl;
//                }
//                ui8MainRestart = 0;
//
//                // создадим поток копирования диска mmc0 на mmc1.
//                // create thread, pass reference, addr of the function and data
//                if (pthread_create(&xSystemDiskCopyDisplay,
//                                   NULL,
//                                   thread_SystemDiskCopyDisplay,
//                                   NULL))
//                {
//                    cout << "Failed to create the thread_SystemDiskCopyDisplay" << endl;
//                }
//
//                // первый сеанс связи с дисплеем его перезагрузит.
//                // на следущем сеансе отправим команду - копирование диска mmc0 на mmc1.
//                // если отправить команду сразу, на дисплее не появится окно копирования.
////                ui8FlowControl = 5;
//                SetFsmState(MAIN_CYCLE_DISK_COPY_DISPLAY_READY_WAITING);
//            }
//            else
//            {
//                // Linux загружен с диска mmc1.
//            }
			break;

		case SYSTEM_RE_INITIALIZATION:
			cout << "CMainProductionCycle::SYSTEM_RE_INITIALIZATION begin" << endl;
			// можно перезагрузить
			GlobalVariablesInitInitialization();
//        	// можно перезагрузить
//            CodeSysStart();

			// можно перезагрузить
			MainTaskInitialization();
			// можно перезагрузить
			MainTaskComponentsInformationCollection();
			if (IsSpecificationCorrect())
			{
				SetFsmState(MAIN_CYCLE_START_WAITING);
			}
			else
			{
				SetFsmState(EMERGENCY_CYCLE);
			}

			// вычислим контрольные суммы. потом будем проверять целостность перед каждым главным циклом.
			xPlcConfigService.usCrc =
				usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
											sizeof(xPlcConfigService.xPlcConfigServiceData));

			ucModbusSlaveAddress = (xPlcDataBase.
									axPlcDataBaseBlocks[NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET].
									aucPlcDataBaseBlockData[0]) + 1;
			xCCrc.CreateAll();

			IsDataBaseCorrect();
			IsConfigurationCorrect();

			// обновим настройки интерфейса ModbusRtu, связь с локальной сетью нижнего уровня, RS-485.
			iModbusRtuMasterReset();

			break;

		case MAIN_CYCLE_DISK_COPY_DISPLAY_READY_WAITING:
			cout << "MAIN_CYCLE_BOOT_SOURCE_CHECK" << endl;
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				pthread_mutex_lock(&xMainDataAccessMutex);
				vCurrentTimeUpdate();
//                vBadModulesTest();
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
//                vInternalToModbusBuffDataExchange();
				pthread_mutex_unlock(&xMainDataAccessMutex);

				// ?
				if (nucBlinkCounter == 5)
				{
					// установим флаг - копирование диска mmc0 на mmc1.
					fbGlobalMmcCopy = 1;
//                    ui8FlowControl = 10;
					nucBlinkCounter = 0;
					SetFsmState(MAIN_CYCLE_DISK_COPY_END_WAITING);
				}
				else
				{
					nucBlinkCounter++;
				}
			}
			usleep(1000);
			break;

		case MAIN_CYCLE_DISK_COPY_END_WAITING:
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				pthread_mutex_lock(&xMainDataAccessMutex);
				vCurrentTimeUpdate();
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
				pthread_mutex_unlock(&xMainDataAccessMutex);

				if (ui8MainRestart)
				{
					ui8MainRestart = 0;
					// установим флаг - закончено копирование диска mmc0 на mmc1.
					fbGlobalMmcCopyEnd = 1;
					CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
					SetFsmState(MAIN_CYCLE_DISK_COPY_END_REBOOT_WAITING);
				}
				else
				{
					// управление мигающим синим светодиодом.
					// фаза отсутствия свечения закончилась?
					if ((!fbBlinkIsOn) && (!nucBlinkCounter))
					{
						nucBlinkCounter = 2;
						// фаза свечения.
						fbBlinkIsOn = 1;
						CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
					}
					else
					{
						// фаза свечения закончилась?
						if ((fbBlinkIsOn) && (!nucBlinkCounter))
						{
							nucBlinkCounter = 3;
							// фаза отсутствия свечения.
							fbBlinkIsOn = 0;
							CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
						}
						else
						{
							nucBlinkCounter--;
						}
					}
				}
			}
			usleep(1000);
			break;

		case MAIN_CYCLE_DISK_COPY_END_REBOOT_WAITING:
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				pthread_mutex_lock(&xMainDataAccessMutex);
				vCurrentTimeUpdate();
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
				pthread_mutex_unlock(&xMainDataAccessMutex);
			}
			usleep(1000);
			break;

//        case MAIN_TASK_INITIALIZATION:
//            cout << "MAIN_TASK_INITIALIZATION" << endl;
//            MainTaskInitialization();
//            SetFsmState(MAIN_TASK_COMPONENTS_INFORMATION_COLLECTION);
//            break;
//
//        case MAIN_TASK_COMPONENTS_INFORMATION_COLLECTION:
//            cout << "MAIN_TASK_COMPONENTS_INFORMATION_COLLECTION" << endl;
//            MainTaskComponentsInformationCollection();
//            SetFsmState(SYSTEM_SPECIFICATION_CHECK);
//            break;
//
//        case SYSTEM_SPECIFICATION_CHECK:
//            cout << "SYSTEM_SPECIFICATION_CHECK" << endl;
//            if (IsSpecificationCorrect())
//            {
//                SetFsmState(MAIN_TASK_INITIALIZATION_END);
//            }
//            else
//            {
//                SetFsmState(EMERGENCY_CYCLE);
//            }
//            break;
//
//        case MAIN_TASK_INITIALIZATION_END:
//            cout << "MAIN_TASK_INITIALIZATION_END" << endl;
//            MainTaskInitializationEnd();
//            SetFsmState(MAIN_CYCLE_START_WAITING);
//            break;
//
//        case MAIN_TASK_DISPLAY_INITIALIZATION_WAIT:
//            cout << "MAIN_TASK_DISPLAY_INITIALIZATION_WAIT" << endl;
//            MainTaskInitializationEnd();
//            SetFsmState(MAIN_CYCLE_BOOT_SOURCE_CHECK);
//            break;

		case MAIN_CYCLE_START_WAITING:
			xCTimeMeasure.Begin();

			pthread_mutex_lock(&xMainDataAccessMutex);
			vCurrentTimeUpdate();
			vBadModulesTest();
			SetFsmState(MAIN_CYCLE_MODULES_INTERACTION);

			xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);

			pthread_mutex_unlock(&xMainDataAccessMutex);
			nucIndexNumber = 0;
			break;

		case MAIN_CYCLE_MODULES_INTERACTION:
			// обработаны не все внутренние модули?
			if (nucIndexNumber < (xPlcConfigService.
								  xPlcConfigServiceData.
								  ucInternalModulesQuantity))
			{
				// какой обрабатывается модуль?
				switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
						xModuleContextStatic.ucModuleType)
				{
				case MODULE_TYPE_MVDI:
					iModuleMvdiDataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MVDS9:
					iModuleMvds9DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MVPS3:
				case MODULE_TYPE_MVAI5:
				case MODULE_TYPE_MVST3:
				case MODULE_TYPE_MVAO3:
					vModuleMvai5DataHandlerNew(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MR53:
				case MODULE_TYPE_MR54:
				case MODULE_TYPE_MR55:
				case MODULE_TYPE_MR51:
					vModuleMrXXDataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MTVI5:
					iModuleMtvi5DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					ui8MtviCounter++;
					break;
				default:
					break;
				};

				nucIndexNumber++;
			}
			else
			{
				SetFsmState(MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING);
			}
			break;

		case MAIN_CYCLE_DISCRETE_SIGNALS_PROCESSING:
			// обработаны все внутренние модули, выполним оставшиеся задачи.
			pthread_mutex_lock(&xMainDataAccessMutex);

			m_pxMathematicalFunctionalBlockManager ->
			Fsm();

			vAlarmHandler();
			vLinuxToCodeSysDataExchange();
			vInternalToModbusBuffDataExchange();
			// сбросим флаги квитирование и сброс, если установлены.
			fbGlobalKvit = 0;
			fbGlobalReset = 0;
			pthread_mutex_unlock(&xMainDataAccessMutex);

			// размер базы данных достиг максимума?
			if (xCArchiveEventsDB.DataBaseIsFull())
			{
				xCArchiveEventsDB.DataBaseIsFullReset();
				xCArchiveEventsDB.DataBaseRingCreate();
			}
			xCTimeMeasure.End();
			SetFsmState(MAIN_CYCLE_END);
			break;

		case MAIN_CYCLE_END:
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				SetFsmState(MAIN_CYCLE_START_WAITING);
			}
			usleep(1000);
			break;

		case EMERGENCY_CYCLE:
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				pthread_mutex_lock(&xMainDataAccessMutex);
				vCurrentTimeUpdate();
				vBadModulesTest();
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
				vInternalToModbusBuffDataExchange();
				pthread_mutex_unlock(&xMainDataAccessMutex);
				if (fbPlcErrorReset)
				{
//                    vThreadsDestroy();
					fbPlcErrorReset = 0;
					SetFsmState(MAIN_CYCLE_START_WAITING);
				}

//                if (iTempCounterConfigurationTrror == 10)
//                {
//                    cout << "EMERGENCY_CYCLE iTempCounterConfigurationTrror == 10" << endl;
//                    IsConfigurationCorrect();
//                    iTempCounterConfigurationTrror++;
//                }
//                else
//                {
//                    iTempCounterConfigurationTrror++;
//                }

				// есть блокировка?
				if (fbGlobalBlock)
				{
					// активное состояние события ещё не зарегистрировано?
					if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
								SYSTEM_ERROR_GROUP_OFFSET,
								SYSTEM_ERROR_BLOCK_MODE_OFFSET))
					{
						// зарегистрируем активное состояние события.
						// включение режима блокировка.
						CEvents::EventRegistration(
							0,
							(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//                         CEvents::HANDLED_EVENTS_IS_POPUP |
//                         CEvents::HANDLED_EVENTS_IS_SOUND |
							 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
							SYSTEM_ERROR_BLOCK_MODE_OFFSET,
							"Вкл. блокировки");
					}
				}
				else
				{
					// неактивное состояние события ещё не зарегистрировано?
					if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
								SYSTEM_ERROR_GROUP_OFFSET,
								SYSTEM_ERROR_BLOCK_MODE_OFFSET))
					{
						// зарегистрируем неактивное состояние события.
						// выключение режима блокировка.
						CEvents::EventRegistration(
							0,
							(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
							 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
							SYSTEM_ERROR_BLOCK_MODE_OFFSET,
							"Выкл. блокировки");
					}
				}

			}
			usleep(1000);
			break;

		case PROGRAMMING_CYCLE_START:
			// начнём цикл, сессию программирования.
			// он запускается переключением автомата главного цикла, в функции:
			// void vDeviceControlDataBaseBlockWriteHandler(modbus_mapping_t *pxModbusMapping)
			// командой:
			// xMainProductionCycle.SetFsmState(CMainProductionCycle::PROGRAMMING_CYCLE_START);
			// эта команда отдаётся всякий раз, когда по интерфейсам связи приходит сообщение
			// с блоком базы данных для записи в память прибора.
			// сколько придёт блоков мы не знаем, но знаем, что последним блоком, а следовательно
			// и концом сессии считается тот блок после которого не следуют другие в течение
			// определённого времени.
			// установим это время.
			m_xTimer.Set(5000);
			ucMainCycleTimeStart = 0;
			iTimerRestart(&timerid, &its);
			SetFsmState(PROGRAMMING_CYCLE);
			break;

		case PROGRAMMING_CYCLE:
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				pthread_mutex_lock(&xMainDataAccessMutex);
				vCurrentTimeUpdate();
				vBadModulesTest();
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
				vInternalToModbusBuffDataExchange();
				pthread_mutex_unlock(&xMainDataAccessMutex);

				nucIndexNumber = 0;
				SetFsmState(PROGRAMMING_CYCLE_MODULES_INTERACTION);
			}

			// сессия программирования закончилась?
			if (m_xTimer.IsOverflow())
			{
				// зарегистрируем событие.
				CEvents::EventRegistration(
					0,
					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
					0,
					"Программирование");
				// чтобы событие записалось в базу данных, нужно чтобы сохранённые данные события
				// были в доступны буфере на следующем цикле обработки событий в функции:
				// void CEvents::EventsHandler(void)
				// для этого дадим время для нормальной работы цикла перед переинициализацией прибора
				// и очисткой вех данных.
				// запустим таймер на это время.
				m_xTimer.Set(500);
				SetFsmState(PROGRAMMING_CYCLE_EVENT_WRITE_END_WAIT);
			}

			usleep(1000);
			break;

		case PROGRAMMING_CYCLE_MODULES_INTERACTION:
			// обработаны не все внутренние модули?
			if (nucIndexNumber < (xPlcConfigService.
								  xPlcConfigServiceData.
								  ucInternalModulesQuantity))
			{
				// какой обрабатывается модуль?
				switch(xAllModulesContext.axAllModulesContext[nucIndexNumber].
						xModuleContextStatic.ucModuleType)
				{
				case MODULE_TYPE_MVDI:
					iModuleMvdiDataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MVDS9:
					iModuleMvds9DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MVPS3:
				case MODULE_TYPE_MVAI5:
				case MODULE_TYPE_MVST3:
				case MODULE_TYPE_MVAO3:
					vModuleMvai5DataHandlerNew(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MR53:
				case MODULE_TYPE_MR54:
				case MODULE_TYPE_MR55:
				case MODULE_TYPE_MR51:
					vModuleMrXXDataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					break;
				case MODULE_TYPE_MTVI5:
					iModuleMtvi5DataHandler(&xAllModulesContext.axAllModulesContext[nucIndexNumber]);
					ui8MtviCounter++;
					break;
				default:
					break;
				};

				nucIndexNumber++;
			}
			else
			{
				SetFsmState(PROGRAMMING_CYCLE);
			}
			break;

		case PROGRAMMING_CYCLE_EVENT_WRITE_END_WAIT:
			if (ucMainCycleTimeStart)
			{
				ucMainCycleTimeStart = 0;
				iTimerRestart(&timerid, &its);
				pthread_mutex_lock(&xMainDataAccessMutex);
				vCurrentTimeUpdate();
				vBadModulesTest();
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
				pthread_mutex_unlock(&xMainDataAccessMutex);
			}

			// сессия записи события закончилась?
			if (m_xTimer.IsOverflow())
			{
				iPlcConfigurationCreate();
				vMainTaskMemoryMapFormation();
				iPlcConfigurationSave();

				SetFsmState(PROGRAMMING_CYCLE_END);
			}

			usleep(1000);
			break;

		case PROGRAMMING_CYCLE_END:
			SetFsmState(SYSTEM_RE_INITIALIZATION);
			break;

		default:
			break;
		}

//-----------------------------------------------------------------------------------------------------
		pthread_mutex_lock(&xMainDataAccessMutex);
//#ifdef MODBUS_TCP_ENABLE
		vDeviceControlModbusMessageHandler(pxTcpModbusMapping);
//#endif // MODBUS_TCP_ENABLE

//#ifdef MODBUS_RTU_ENABLE
		vDeviceControlModbusMessageHandler(pxRtuModbusMapping);
//#endif // MODBUS_RTU_ENABLE

//#ifdef MODBUS_HMI_ENABLE
		vDeviceControlModbusMessageHandler(pxHmiRtuModbusMapping);
//#endif // MODBUS_HMI_ENABLE
		pthread_mutex_unlock(&xMainDataAccessMutex);
//    cout << "pthread_mutex_unlock" << endl;
	}
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::Run(void)
{
//    while (1)
//    {
	Fsm();
//    }
}

//-----------------------------------------------------------------------------------------------------
