/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define CPU_32_BIT

#include "../../core/macros.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"
#include "MarlinSerial.h"

#include "../../inc/MarlinConfigPre.h"

#include <stdint.h>

#ifdef USBCON
  #include <USBSerial.h>
#endif

// ------------------------
// Defines
// ------------------------

#if SERIAL_PORT == 0
  #error "SERIAL_PORT cannot be 0. (Port 0 does not exist.) Please update your configuration."
#elif SERIAL_PORT == -1
  //mp//#define MYSERIAL0 SerialUSB
  //mp//#define MYSERIAL0 Serial
  #define MYSERIAL0 Serial
#elif SERIAL_PORT == 1
  #define MYSERIAL0 MSerial1
#elif SERIAL_PORT == 2
  #define MYSERIAL0 MSerial2
#elif SERIAL_PORT == 3
  #define MYSERIAL0 MSerial3
#elif SERIAL_PORT == 4
  #define MYSERIAL0 MSerial4
#elif SERIAL_PORT == 5
  #define MYSERIAL0 MSerial5
#elif SERIAL_PORT == 6
  #define MYSERIAL0 MSerial6
#else
  #error "SERIAL_PORT must be from -1 to 6. Please update your configuration."
#endif

#ifdef SERIAL_PORT_2
  #define NUM_SERIAL 2
  #if SERIAL_PORT_2 == 0
    #error "SERIAL_PORT_2 cannot be 0. (Port 0 does not exist.) Please update your configuration."
  #elif SERIAL_PORT_2 == SERIAL_PORT
    #error "SERIAL_PORT_2 must be different than SERIAL_PORT. Please update your configuration."
  #elif SERIAL_PORT_2 == -1
    #define MYSERIAL1 SerialUSB
  #elif SERIAL_PORT_2 == 1
    #define MYSERIAL1 MSerial1
  #elif SERIAL_PORT_2 == 2
    #define MYSERIAL1 MSerial2
  #elif SERIAL_PORT_2 == 3
    #define MYSERIAL1 MSerial3
  #elif SERIAL_PORT_2 == 4
    #define MYSERIAL1 MSerial4
  #elif SERIAL_PORT_2 == 5
    #define MYSERIAL1 MSerial5
  #elif SERIAL_PORT_2 == 6
    #define MYSERIAL1 MSerial6
  #else
    #error "SERIAL_PORT_2 must be from -1 to 6. Please update your configuration."
  #endif
#else
  #define NUM_SERIAL 1
#endif

#if HAS_DGUS_LCD
  #if DGUS_SERIAL_PORT == 0
    #error "DGUS_SERIAL_PORT cannot be 0. (Port 0 does not exist.) Please update your configuration."
  #elif DGUS_SERIAL_PORT == SERIAL_PORT
    #error "DGUS_SERIAL_PORT must be different than SERIAL_PORT. Please update your configuration."
  #elif defined(SERIAL_PORT_2) && DGUS_SERIAL_PORT == SERIAL_PORT_2
    #error "DGUS_SERIAL_PORT must be different than SERIAL_PORT_2. Please update your configuration."
  #elif DGUS_SERIAL_PORT == -1
    #define DGUS_SERIAL SerialUSB
  #elif DGUS_SERIAL_PORT == 1
    #define DGUS_SERIAL MSerial1
  #elif DGUS_SERIAL_PORT == 2
    #define DGUS_SERIAL MSerial2
  #elif DGUS_SERIAL_PORT == 3
    #define DGUS_SERIAL MSerial3
  #elif DGUS_SERIAL_PORT == 4
    #define DGUS_SERIAL MSerial4
  #elif DGUS_SERIAL_PORT == 5
    #define DGUS_SERIAL MSerial5
  #elif DGUS_SERIAL_PORT == 6
    #define DGUS_SERIAL MSerial6
  #else
    #error "DGUS_SERIAL_PORT must be from -1 to 6. Please update your configuration."
  #endif

  #define DGUS_SERIAL_GET_TX_BUFFER_FREE DGUS_SERIAL.availableForWrite
#endif


/**
 * TODO: review this to return 1 for pins that are not analog input
 */
#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) (p)
#endif

#define CRITICAL_SECTION_START()  uint32_t primask = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END()    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()
#define cli() __disable_irq()
#define sei() __enable_irq()

// On AVR this is in math.h?
#define square(x) ((x)*(x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))

// ------------------------
// Types
// ------------------------

typedef int16_t pin_t;

#define HAL_SERVO_LIB libServo

// ------------------------
// Public Variables
// ------------------------

// result of last ADC conversion
extern uint16_t HAL_adc_result;

// ------------------------
// Public functions
// ------------------------

// Memory related
#define __bss_end __bss_end__

// Enable hooks into  setup for HAL
void HAL_init();

// Clear reset reason
void HAL_clear_reset_source();

// Reset reason
uint8_t HAL_get_reset_source();

void _delay_ms(const int delay);

extern "C" char* _sbrk(int incr);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static inline int freeMemory() {
  volatile char top;
  return &top - reinterpret_cast<char*>(_sbrk(0));
}

#pragma GCC diagnostic pop

//
// ADC
//

#define HAL_ANALOG_SELECT(pin) pinMode(pin, INPUT)

inline void HAL_adc_init() {}

#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10
#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_result
#define HAL_ADC_READY()     true

void HAL_adc_start_conversion(const uint8_t adc_pin);

uint16_t HAL_adc_get_result();

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define PLATFORM_M997_SUPPORT
void flashFirmware(const int16_t);
