// eLua platform configuration
// Modified to include support for Hempl

// Simplemachines.it Mizar32 board has 128Kbytes of flash with 8kb of bootloader
// To fit in 120K, build using:
// scons board=mizar32 target=lualong optram=0 allocator=newlib

#ifndef __MIZAR32_CONF_H__
#define __MIZAR32_CONF_H__

#include "sdramc.h"
#include "sermux.h"
#include "usb-cdc.h"
#include "buf.h"

// *****************************************************************************
// Define here what components you want for this platform

#define BUILD_MMCFS
//#define BUILD_ROMFS
#define BUILD_CON_GENERIC
//#define BUILD_RPC
#define BUILD_C_INT_HANDLERS
//#define BUILD_RFS
//#define BUILD_SERMUX

#if defined (ALCOR_CPU_AT32UC3A0128)
# define RAM_SIZE 0x8000 // Build options for 120KB image
#else
# define RAM_SIZE 0x10000 // For 256KB and 512KB flash
#endif

# define BUILD_SHELL
//# define BUILD_ADVANCED_SHELL
# define BUILD_XMODEM
# define BUILD_ADC
# define BUILD_LCD
# define BUILD_RTC
# define BUILD_TERM
// # define BUILD_UIP
// # define BUILD_KS0108B

// Interrupt handler support
#define BUILD_PICOLISP_INT_HANDLERS

# define BUILD_USB_CDC
# define BUILD_EDITOR_IV

// uip support.
#ifdef BUILD_UIP
# define BUILD_DHCPC
# define BUILD_DNS
//#define BUILD_CON_TCP
#endif

// ****************************************************************************
// Auxiliary libraries that will be compiled for this platform

// The name of the platform specific libs table
#define PS_LIB_TABLE_NAME   "mizar32"

// *****************************************************************************
// UART/Timer IDs configuration data (used in main.c)

#define BUF_ENABLE_UART

#ifdef BUILD_USB_CDC
# define CON_UART_ID        CDC_UART_ID
#elif defined( BUILD_SERMUX )
# define CON_UART_ID        ( SERMUX_SERVICE_ID_FIRST + 1 )
#else
# define CON_UART_ID        0
# define CON_UART_SPEED     115200
// As flow control seems not to work, we use a large buffer so that people
// can copy/paste program fragments or data into the serial console.
// An 80x25 screenful is 2000 characters so we use 2048.
# define CON_BUF_SIZE       BUF_SIZE_2048
#endif

#define TERM_LINES          25
#define TERM_COLS           80

// *****************************************************************************
// SPI pins configuration data

#define BOARD_SPI0_SCK_PIN                  AVR32_PIN_PA13
#define BOARD_SPI0_SCK_PIN_FUNCTION         0
#define BOARD_SPI0_MISO_PIN                 AVR32_PIN_PA11
#define BOARD_SPI0_MISO_PIN_FUNCTION        0
#define BOARD_SPI0_MOSI_PIN                 AVR32_PIN_PA12
#define BOARD_SPI0_MOSI_PIN_FUNCTION        0
#define BOARD_SPI0_CS_PIN                   AVR32_PIN_PA10
#define BOARD_SPI0_CS_PIN_FUNCTION          0

#define BOARD_SPI1_SCK_PIN                  AVR32_PIN_PA15
#define BOARD_SPI1_SCK_PIN_FUNCTION         1
#define BOARD_SPI1_MISO_PIN                 AVR32_PIN_PA17
#define BOARD_SPI1_MISO_PIN_FUNCTION        1
#define BOARD_SPI1_MOSI_PIN                 AVR32_PIN_PA16
#define BOARD_SPI1_MOSI_PIN_FUNCTION        1
#define BOARD_SPI1_CS_PIN                   AVR32_PIN_PA14
#define BOARD_SPI1_CS_PIN_FUNCTION          1

// Auxiliary libraries that will be compiled for this platform
#ifdef BUILD_ADC
# define ADCLINE _ROM(ADC)
#else
# define ADCLINE
#endif

// platform library functions
#define PICOLISP_PLATFORM_LIBS_ROM\
  _ROM(PD)\
  _ROM(TERM)\
  _ROM(MIZAR32_LCD)\
  _ROM(ELUA)\
  _ROM(CPU)\
  _ROM(TIMER)\
  _ROM(I2C)\
  _ROM(PWM)\
  _ROM(SPI)\
  _ROM(PIO)\
  _ROM(UART)\
  ADCLINE

// *****************************************************************************
// Configuration data

// Virtual timers (0 if not used)
#define VTMR_NUM_TIMERS       4
#define VTMR_FREQ_HZ          10
#define VTMR_CH               2    // Which hardware timer to use for VTMR

// Number of resources (0 if not available/not implemented)
#define NUM_PIO               4
#define NUM_SPI               8
#define NUM_UART              2
#if VTMR_NUM_TIMERS > 0
#define NUM_TIMER             2
#else
#define NUM_TIMER             3
#endif
#define NUM_PWM               6         // PWM7 is on GPIO50
#define NUM_I2C               1
#define NUM_ADC               8         // Though ADC3 pin is the Ethernet IRQ
#define NUM_CAN               0


// RPC boot options
#define RPC_UART_ID           0
#define RPC_UART_SPEED        115200

// ADC Configuration Params
#define ADC_BIT_RESOLUTION    10
#define BUF_ENABLE_ADC
#define ADC_BUF_SIZE          BUF_SIZE_2

// These should be adjusted to support multiple ADC devices
#define ADC_TIMER_FIRST_ID    0
#define ADC_NUM_TIMERS        0

// SD/MMC Filesystem Setup
#define MMCFS_SPI_NUM          4
#define MMCFS_CS_PORT          0
#define MMCFS_CS_PIN           SD_MMC_SPI_NPCS_PIN

// CPU frequency (needed by the CPU module and MMCFS code, 0 if not used)
#define CPU_FREQUENCY         REQ_CPU_FREQ

// PIO prefix ('0' for P0, P1, ... or 'A' for PA, PB, ...)
#define PIO_PREFIX            'A'
// Pins per port configuration:
// #define PIO_PINS_PER_PORT (n) if each port has the same number of pins, or
// #define PIO_PIN_ARRAY { n1, n2, ... } to define pins per port in an array
// Use #define PIO_PINS_PER_PORT 0 if this isn't needed
#define PIO_PIN_ARRAY         { 31, 32, 32, 14 }
#define AVR32_NUM_GPIO        110 // actually 109, but consider also PA31

#ifdef BOOTLOADER_EMBLOD
# define ALCOR_FIRMWARE_SIZE 0x80000
#else
# define ALCOR_FIRMWARE_SIZE 0
#endif

// Allocator data: define your free memory zones here in two arrays
// (start address and end address)
#ifdef USE_MULTIPLE_ALLOCATOR
#define MEM_START_ADDRESS     { ( void* )end, ( void* )( SDRAM + ALCOR_FIRMWARE_SIZE ) }
#define MEM_END_ADDRESS       { ( void* )( RAM_SIZE - STACK_SIZE_TOTAL - 1 ), ( void* )( SDRAM + SDRAM_SIZE - 1 ) }
#else
// Newlib<1.19.0 has a bug in their dlmalloc that corrupts memory when there
// are multiple regions, and it appears that simple allocator also has problems.
// So with these allocators, only use a single region - the slower 32MB one.
#define MEM_START_ADDRESS     { ( void* )( SDRAM + ALCOR_FIRMWARE_SIZE ) }
#define MEM_END_ADDRESS       { ( void* )( SDRAM + SDRAM_SIZE - 1 ) }
#endif

#define RFS_BUFFER_SIZE       BUF_SIZE_512
#define RFS_UART_ID           ( SERMUX_SERVICE_ID_FIRST )
#define RFS_TIMEOUT           100000
#define RFS_UART_SPEED        115200

//#define SERMUX_PHYS_ID        0
//#define SERMUX_PHYS_SPEED     115200
//#define SERMUX_NUM_VUART      2
//#define SERMUX_BUFFER_SIZES   { RFS_BUFFER_SIZE, CON_BUF_SIZE }

// *****************************************************************************
// CPU constants that should be exposed to the eLua "cpu" module

// Static TCP/IP configuration

#define ELUA_CONF_IPADDR0     192
#define ELUA_CONF_IPADDR1     168
#define ELUA_CONF_IPADDR2     1
#define ELUA_CONF_IPADDR3     10

#define ELUA_CONF_NETMASK0    255
#define ELUA_CONF_NETMASK1    255
#define ELUA_CONF_NETMASK2    255
#define ELUA_CONF_NETMASK3    0

#define ELUA_CONF_DEFGW0      192
#define ELUA_CONF_DEFGW1      168
#define ELUA_CONF_DEFGW2      1
#define ELUA_CONF_DEFGW3      1

#define ELUA_CONF_DNS0        192
#define ELUA_CONF_DNS1        168
#define ELUA_CONF_DNS2        1
#define ELUA_CONF_DNS3        1

#endif // #ifndef __MIZAR32_CONF_H__
