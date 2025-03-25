#ifndef DEVICECONTROL_H_INCLUDED
#define DEVICECONTROL_H_INCLUDED

#include "PasNewConfig.h"


//============================================================================
// команды для драйверов модулей передаваемые через контекст.
// инкремент уставок - SP, OUT.
#define DEVICE_CONTROL_MTVI5_INCREMENT_SP_OUT  0x01
// декремент уставок - SP, OUT.
#define DEVICE_CONTROL_MTVI5_DECREMENT_SP_OUT  0x02
// записать байт-команду в массив состояния регуляторов «CONT_ST».
#define DEVICE_CONTROL_MTVI5_CONT_ST_WRITE  0x04
// записать параметры в массив аналоговых переменных регуляторов «CONT_AV».
#define DEVICE_CONTROL_MTVI5_CONT_AV_WRITE  0x05
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_PROGRAMMER  0x06
#define DEVICE_CONTROL_MTVI5_DATA_BASE_WRITE_BY_HMI  0x07
#define DEVICE_CONTROL_MTVI5_LOAD_OUTS_OSF  0x08
#define DEVICE_CONTROL_MTVI5_CONT_ST_CHANGE  0x09

// адреса-команды modbus функции 5.
// включение-выключение режима калибровки.
#define DEVICE_CONTROL_CALIBRATION_ON_OFF 0x1000
// калибровка начала шкалы.
#define DEVICE_CONTROL_SET_BOTTOM_OF_SCALE  0x1100
// калибровка конца шкалы.
#define DEVICE_CONTROL_SET_TOP_OF_SCALE  0x1200
// включение-выключение режима блоуировки.
#define DEVICE_CONTROL_BLOCK 0x007D
// квитирование с верхнего уровня.
#define DEVICE_CONTROL_PC_KVIT  0x007E
// сброс с верхнего уровня.
#define DEVICE_CONTROL_PC_RESET 0x007F

#endif // DEVICECONTROL_H_INCLUDED
