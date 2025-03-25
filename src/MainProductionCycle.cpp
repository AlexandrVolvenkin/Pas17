
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
// ����� �������.
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




//	// �������� ����������� �����. ����� ����� ��������� ����������� ����� ������ ������� ������.
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

		// �������� ��������� ������� ��� �� ����������������?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET))
		{
			// �������������� �������� ��������� �������.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_SYSTEM_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_ARCHIVE |
				 CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
				SYSTEM_ERROR_COMMON_DATA_BASE_ERROR_OFFSET,
				"������ ����� �.�");
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

		// �������� ��������� ������� ��� �� ����������������?
		if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
					SYSTEM_ERROR_GROUP_OFFSET,
					SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET))
		{
			// �������������� �������� ��������� �������.
			CEvents::EventRegistration(
				SYSTEM_ERROR_GROUP_OFFSET,
				(CEvents::HANDLED_EVENTS_CONFIGURATION_ERROR_TYPE |
				 CEvents::HANDLED_EVENTS_IS_POPUP |
				 CEvents::HANDLED_EVENTS_IS_SOUND |
				 CEvents::HANDLED_EVENTS_IS_OCCURED_ON_START),
				SYSTEM_ERROR_CONFIGURATION_ERROR_OFFSET,
				"������ ������������");
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


// ���������� ������� ����������� �� �������� ����������� ����� mmc0 �� mmc1(MmcCopy.sh), ����� ��� ���������.
//-----------------------------------------------------------------------------------------------------
void MainActionHandler(int signo, siginfo_t *info, void *context)
{
	ui8MainRestart = 1;
	cout << "action MainRestart" << endl;
}

//// ���������� ������� ����������� �� �������� ����������� ����� mmc0 �� mmc1(MmcCopy.sh), ����� ��� ���������.
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
// ����� ����������� ����� mmc0 �� mmc1.
void *thread_SystemDiskCopyDisplay(void *value)
{
//    // ������ ����� ��������, ������������, ���� ������.
//    system("sudo rm /home/debian/PasConfig.dat");
//    system("sudo rm /home/debian/ReservPasConfig.dat");
//    system("sudo rm /home/debian/PasDataBase.dat");
//    system("sudo rm /home/debian/ReservPasDataBase.dat");
//    system("sudo rm /home/debian/Pas05ArchiveDataBase.db");
//    system("sudo rm /home/debian/PasSettings.dat");
	// �������� �������� ����������� ����� mmc0 �� mmc1(MmcCopy.sh).
	// �������� �������.
	system("sudo /home/debian/MmcCopy.sh");
}

//-----------------------------------------------------------------------------------------------------
void CMainProductionCycle::MainTaskInitializationEnd(void)
{
	// ������� ���� ������.
	if (xCArchiveEventsDB.Connect())
	{
		// error.
	}

	// �������� ����������� �����. ����� ����� ��������� ����������� ����� ������ ������� ������.
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
			// ����� �������������
			GlobalVariablesInitInitialization();
			// ����� �������������
			CodeSysStart();
			// ������ �������������
			PlatformComponentsInitialization();

			// ����� �������������
			MainTaskInitialization();
			// ����� �������������
			MainTaskComponentsInformationCollection();
			if (IsSpecificationCorrect())
			{
				SetFsmState(MAIN_CYCLE_START_WAITING);
			}
			else
			{
				SetFsmState(EMERGENCY_CYCLE);
			}

			// ������ �������������
			MainTaskInitializationEnd();

			// ������� ������������ ������� ����.
			for (uint8_t i = 0; i < 20; i++)
			{
				usleep(100000);
				xCPingTaskEWWE.DataExchange(CTaskSerialMT::apxTasksProcessed, &xCSpiSerialMT);
			}

			IsDataBaseCorrect();
			IsConfigurationCorrect();

			xCTimeMeasure.Begin();

//            // Linux �������� � ����� mmc0?
//            if (xCArchiveSaveParse.CheckMountedDiskMmc0())
//            {
//                // Linux �������� � ����� mmc0.
//                // �������� �������� ����������� ����� mmc0 �� mmc1.
//                // �������� ���������� ������� ����������� �� �������� ����������� ����� mmc0 �� mmc1, ����� ��� ���������.
//                if (iHandlerCreate(SIGRTMIN+3,
//                                   &MainAction,
//                                   MainActionHandler) == - 1)
//                {
//                    cout << "Failed to create the action MainAction" << endl;
//                }
//                ui8MainRestart = 0;
//
//                // �������� ����� ����������� ����� mmc0 �� mmc1.
//                // create thread, pass reference, addr of the function and data
//                if (pthread_create(&xSystemDiskCopyDisplay,
//                                   NULL,
//                                   thread_SystemDiskCopyDisplay,
//                                   NULL))
//                {
//                    cout << "Failed to create the thread_SystemDiskCopyDisplay" << endl;
//                }
//
//                // ������ ����� ����� � �������� ��� ������������.
//                // �� �������� ������ �������� ������� - ����������� ����� mmc0 �� mmc1.
//                // ���� ��������� ������� �����, �� ������� �� �������� ���� �����������.
////                ui8FlowControl = 5;
//                SetFsmState(MAIN_CYCLE_DISK_COPY_DISPLAY_READY_WAITING);
//            }
//            else
//            {
//                // Linux �������� � ����� mmc1.
//            }
			break;

		case SYSTEM_RE_INITIALIZATION:
			cout << "CMainProductionCycle::SYSTEM_RE_INITIALIZATION begin" << endl;
			// ����� �������������
			GlobalVariablesInitInitialization();
//        	// ����� �������������
//            CodeSysStart();

			// ����� �������������
			MainTaskInitialization();
			// ����� �������������
			MainTaskComponentsInformationCollection();
			if (IsSpecificationCorrect())
			{
				SetFsmState(MAIN_CYCLE_START_WAITING);
			}
			else
			{
				SetFsmState(EMERGENCY_CYCLE);
			}

			// �������� ����������� �����. ����� ����� ��������� ����������� ����� ������ ������� ������.
			xPlcConfigService.usCrc =
				usCrcSummTwoByteCalculation((unsigned char*)&xPlcConfigService.xPlcConfigServiceData,
											sizeof(xPlcConfigService.xPlcConfigServiceData));

			ucModbusSlaveAddress = (xPlcDataBase.
									axPlcDataBaseBlocks[NETWORK_ADDRESS_DATA_BASE_BLOCK_OFFSET].
									aucPlcDataBaseBlockData[0]) + 1;
			xCCrc.CreateAll();

			IsDataBaseCorrect();
			IsConfigurationCorrect();

			// ������� ��������� ���������� ModbusRtu, ����� � ��������� ����� ������� ������, RS-485.
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
					// ��������� ���� - ����������� ����� mmc0 �� mmc1.
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
					// ��������� ���� - ��������� ����������� ����� mmc0 �� mmc1.
					fbGlobalMmcCopyEnd = 1;
					CGpio::ClearPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
					SetFsmState(MAIN_CYCLE_DISK_COPY_END_REBOOT_WAITING);
				}
				else
				{
					// ���������� �������� ����� �����������.
					// ���� ���������� �������� �����������?
					if ((!fbBlinkIsOn) && (!nucBlinkCounter))
					{
						nucBlinkCounter = 2;
						// ���� ��������.
						fbBlinkIsOn = 1;
						CGpio::SetPin(PRD_EN_PIN_PORT, PRD_EN_PIN);
					}
					else
					{
						// ���� �������� �����������?
						if ((fbBlinkIsOn) && (!nucBlinkCounter))
						{
							nucBlinkCounter = 3;
							// ���� ���������� ��������.
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
			// ���������� �� ��� ���������� ������?
			if (nucIndexNumber < (xPlcConfigService.
								  xPlcConfigServiceData.
								  ucInternalModulesQuantity))
			{
				// ����� �������������� ������?
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
			// ���������� ��� ���������� ������, �������� ���������� ������.
			pthread_mutex_lock(&xMainDataAccessMutex);

			m_pxMathematicalFunctionalBlockManager ->
			Fsm();

			vAlarmHandler();
			vLinuxToCodeSysDataExchange();
			vInternalToModbusBuffDataExchange();
			// ������� ����� ������������ � �����, ���� �����������.
			fbGlobalKvit = 0;
			fbGlobalReset = 0;
			pthread_mutex_unlock(&xMainDataAccessMutex);

			// ������ ���� ������ ������ ���������?
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

				// ���� ����������?
				if (fbGlobalBlock)
				{
					// �������� ��������� ������� ��� �� ����������������?
					if(xCInternalModuleErrorEvent.EventOnIsNotRegistered(
								SYSTEM_ERROR_GROUP_OFFSET,
								SYSTEM_ERROR_BLOCK_MODE_OFFSET))
					{
						// �������������� �������� ��������� �������.
						// ��������� ������ ����������.
						CEvents::EventRegistration(
							0,
							(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
//                         CEvents::HANDLED_EVENTS_IS_POPUP |
//                         CEvents::HANDLED_EVENTS_IS_SOUND |
							 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
							SYSTEM_ERROR_BLOCK_MODE_OFFSET,
							"���. ����������");
					}
				}
				else
				{
					// ���������� ��������� ������� ��� �� ����������������?
					if(xCInternalModuleErrorEvent.EventOffIsNotRegistered(
								SYSTEM_ERROR_GROUP_OFFSET,
								SYSTEM_ERROR_BLOCK_MODE_OFFSET))
					{
						// �������������� ���������� ��������� �������.
						// ���������� ������ ����������.
						CEvents::EventRegistration(
							0,
							(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
							 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
							SYSTEM_ERROR_BLOCK_MODE_OFFSET,
							"����. ����������");
					}
				}

			}
			usleep(1000);
			break;

		case PROGRAMMING_CYCLE_START:
			// ����� ����, ������ ����������������.
			// �� ����������� ������������� �������� �������� �����, � �������:
			// void vDeviceControlDataBaseBlockWriteHandler(modbus_mapping_t *pxModbusMapping)
			// ��������:
			// xMainProductionCycle.SetFsmState(CMainProductionCycle::PROGRAMMING_CYCLE_START);
			// ��� ������� ������� ������ ���, ����� �� ����������� ����� �������� ���������
			// � ������ ���� ������ ��� ������ � ������ �������.
			// ������� ����� ������ �� �� �����, �� �����, ��� ��������� ������, � �������������
			// � ������ ������ ��������� ��� ���� ����� �������� �� ������� ������ � �������
			// ������������ �������.
			// ��������� ��� �����.
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

			// ������ ���������������� �����������?
			if (m_xTimer.IsOverflow())
			{
				// �������������� �������.
				CEvents::EventRegistration(
					0,
					(CEvents::HANDLED_EVENTS_SYSTEM_EVENTS_TYPE |
					 CEvents::HANDLED_EVENTS_IS_ARCHIVE),
					0,
					"����������������");
				// ����� ������� ���������� � ���� ������, ����� ����� ���������� ������ �������
				// ���� � �������� ������ �� ��������� ����� ��������� ������� � �������:
				// void CEvents::EventsHandler(void)
				// ��� ����� ����� ����� ��� ���������� ������ ����� ����� ������������������ �������
				// � �������� ��� ������.
				// �������� ������ �� ��� �����.
				m_xTimer.Set(500);
				SetFsmState(PROGRAMMING_CYCLE_EVENT_WRITE_END_WAIT);
			}

			usleep(1000);
			break;

		case PROGRAMMING_CYCLE_MODULES_INTERACTION:
			// ���������� �� ��� ���������� ������?
			if (nucIndexNumber < (xPlcConfigService.
								  xPlcConfigServiceData.
								  ucInternalModulesQuantity))
			{
				// ����� �������������� ������?
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

			// ������ ������ ������� �����������?
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
