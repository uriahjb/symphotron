/** 
** This file provides an interface to each peripheral.
** Edit the #define statements at the top of the file to 
** enable and specifiy the location of the peripherals.
** The remainder of the file does the setting up of the peripherals.
**/

#ifndef PROJECT_PERIPHERALS_STM32F37X_H
#define PROJECT_PERIPHERALS_STM32F37X_H

/// MAEVARM M4
// If compiling for an M4 module, uncomment the below
//#define M4 // use for any M4
//#define M4_WHITE // use for the white M4


/// Onboard LED Setup
/// do not define unused LEDS
/// eg. if LED_WHITE_PORT not defined, then mWhiteON compiles to whitespace
// #define LED_AMBER_PORT B // Letter, ex: A, B, ...
// #define LED_AMBER_PIN 15 // Number, ex: 0, 1, 2, ...
//#define LED_AMBER_SET_ON // define iff on-when-set

/*
#define LED_BLUE_PORT ? // Letter, ex: A, B, ...
#define LED_BLUE_PIN ? // Number, ex: 0, 1, 2, ...
#define LED_BLUE_SET_ON // define iff on-when-set
*/
/*
#define LED_CYAN_PORT ? // Letter, ex: A, B, ...
#define LED_CYAN_PIN ? // Number, ex: 0, 1, 2, ...
#define LED_CYAN_SET_ON // define iff on-when-set
*/

// #define LED_GREEN_PORT D // Letter, ex: A, B, ...
// #define LED_GREEN_PIN 8 // Number, ex: 0, 1, 2, ...
//#define LED_GREEN_SET_ON // define iff on-when-set

/*
#define LED_ORANGE_PORT ? // Letter, ex: A, B, ...
#define LED_ORANGE_PIN ? // Number, ex: 0, 1, 2, ...
//#define LED_ORANGE_SET_ON // define iff on-when-set
*/
/*
#define LED_PINK_PORT ? // Letter, ex: A, B, ...
#define LED_PINK_PIN ? // Number, ex: 0, 1, 2, ...
//#define LED_PINK_SET_ON // define iff on-when-set
*/
/*
#define LED_PURPLE_PORT ? // Letter, ex: A, B, ...
#define LED_PURPLE_PIN ? // Number, ex: 0, 1, 2, ...
#define LED_PURPLE_SET_ON // define iff on-when-set
*/

// #define LED_RED_PORT A // Letter, ex: A, B, ...
// #define LED_RED_PIN 9 // Number, ex: 0, 1, 2, ...
//#define LED_RED_SET_ON // define iff on-when-set

/*
#define LED_TURQUOISE_PORT ? // Letter, ex: A, B, ...
#define LED_TURQUOISE_PIN ? // Number, ex: 0, 1, 2, ...
#define LED_TURQUOISE_SET_ON // define iff on-when-set
*/

// #define LED_WHITE_PORT A // Letter, ex: A, B, ...
// #define LED_WHITE_PIN 8 // Number, ex: 0, 1, 2, ...
//#define LED_WHITE_SET_ON // define iff on-when-set

/*
#define LED_YELLOW_PORT ? // Letter, ex: A, B, ...
#define LED_YELLOW_PIN ? // Number, ex: 0, 1, 2, ...
#define LED_YELLOW_SET_ON // define iff on-when-set
*/

/// IMU I2C Setup
/// Used with: Sensor_IMU_MPU-6050_I2C
/*
#define IMU_I2C ? //Number, 1 or 2
#define IMU_SCL_PORT ? //Letter, ex: A
#define IMU_SCL_PIN ? //Number, ex: 1
#define IMU_SDA_PORT ? //Letter, ex: A
#define IMU_SDA_PIN ? //Number, ex: 1
*/


/// IMU SPI Setup
/// Used with: Sensor_IMU_MPU-6050_SPI

/// ms Counter Setup

/// Pressure Sensor I2C Setup
/// Used with: Sensor_Pressure_MS5611-01BA03_I2C

/// Pressure Sensor SPI Setup
/// Used with: Sensor_Pressure_MS5611-01BA03_SPI

/// Quadrature Encoder Setup
/// Used with: Encoder_Rotary_Magnetic_AS5XXX
/*
#define ENCODER_QEP_TIMER ? //Number, 2, 3, 4, 5, 15 or 19
#define ENCODER_QEPA_PORT ? //Letter, ex: A
#define ENCODER_QEPA_PIN ? //Number, ex: 1
#define ENCODER_QEPA_AF ? //Number, ex: 1
#define ENCODER_QEPB_PORT ? //Letter, ex: A
#define ENCODER_QEPB_PIN ? //Number, ex: 1
#define ENCODER_QEPB_AF ? //Number, ex: 1
#define ENCODER_COUNTS ? //Number, ex: 4096
#define ENCODER_INDEX_PORT ? //Letter, ex: A
#define ENCODER_INDEX_PIN ? //Number, ex: 1
#define ENCODER_PWM_TIMER ? //Number, 2, 3, 4, 5, 12, 15 or 19
#define ENCODER_PWM_PORT ? //Letter, ex: A
#define ENCODER_PWM_PIN ? //Number, ex: 1
#define ENCODER_PWM_AF ? //Number, ex: 1
#define ENCODER_PWM_CHANNEL ? //Number, 1 or 2
*/

/// Three Phase Inverter Setup
/// Used with 3x: Inverter_Half_H-Bridge_40A_50khz, Inverter_Half_H-Bridge_20A_50khz
/*
#define SYNC_TIMER_HIGH ? //Timer used for high side (2,3,4,5,19)
#define SYNC_TIMER_LOW ? //Timer used for low side (2,3,4,5,19)
#define SYNC_TIMER_HIGH1_PORT ? //Letter, ex: A
#define SYNC_TIMER_HIGH1_PIN ? //Number, ex: 1
#define SYNC_TIMER_HIGH1_AF ? //Number, ex: 1
#define SYNC_TIMER_HIGH2_PORT ? //Letter, ex: A
#define SYNC_TIMER_HIGH2_PIN ? //Number, ex: 1
#define SYNC_TIMER_HIGH2_AF ? //Number, ex: 1
#define SYNC_TIMER_HIGH3_PORT ? //Letter, ex: A
#define SYNC_TIMER_HIGH3_PIN ? //Number, ex: 1
#define SYNC_TIMER_HIGH3_AF ? //Number, ex: 1
#define SYNC_TIMER_LOW1_PORT ? //Letter, ex: A
#define SYNC_TIMER_LOW1_PIN ? //Number, ex: 1
#define SYNC_TIMER_LOW1_AF ? //Number, ex: 1
#define SYNC_TIMER_LOW2_PORT ? //Letter, ex: A
#define SYNC_TIMER_LOW2_PIN ? //Number, ex: 1
#define SYNC_TIMER_LOW2_AF ? //Number, ex: 1
#define SYNC_TIMER_LOW3_PORT ? //Letter, ex: A
#define SYNC_TIMER_LOW3_PIN ? //Number, ex: 1
#define SYNC_TIMER_LOW3_AF ? //Number, ex: 1
*/

/// Time Base Setup
/// For each TIMEBASE_INTx_FREQ, create a timerCallbackx() function where the x's match numbers
/// The callbacks will be called at the desired TIMEBASE_INTx_FREQ 
/// Define the frequencies in descending frequency order wrt to INT#.
/// Do not skip INT#s.  First define INT1, then INT2, then INT3, then INT4.
/*
#define TIMEBASE_TIMER ?
#define TIMEBASE_INT1_FREQ ? //Number 
#define TIMEBASE_INT2_FREQ ? //Number
#define TIMEBASE_INT3_FREQ ? //Number
#define TIMEBASE_INT4_FREQ ? //Number
*/


/// System Clock Setup
/// If defined, the system_clock time functionality will be enabled.
/// Value must equal 2 or 5 (a 32 bit timer), or be left undefined to disable.
//#define SYSTEM_CLOCK_TIMER ? //Number, 2 or 5


/// USB Setup
#define USING_USB // define if using USB
/// Used with Communication_Plug_USB_Micro_B_FCI_10103594-0001LF, Communication_Plug_USB_Mini_B_Wurth_651305142821
/// Enable these only if a USB software disconnect pin is used
/*
#define USB_DISCONNECT_PORT ? //Letter, ex: A
#define USB_DISCONNECT_PIN ? //Number, ex: 1
*/

/// Zigbee Setup (for zigbee, uncomment and fill in ? below)
/// Used with: Radio_AT86RF
#define ZIGBEE_SPI 3 //Number, ex: 1
#define ZIGBEE_SCK_PORT A //Letter, ex: A
#define ZIGBEE_SCK_PIN 1 //Number, ex: 1
#define ZIGBEE_SCK_AF 6 //Number, ex: 1
#define ZIGBEE_MISO_PORT A //Letter, ex: A
#define ZIGBEE_MISO_PIN 2 //Number, ex: 1
#define ZIGBEE_MISO_AF 6 //Number, ex: 1
#define ZIGBEE_MOSI_PORT A //Letter, ex: A
#define ZIGBEE_MOSI_PIN 3 //Number, ex: 1
#define ZIGBEE_MOSI_AF 6 //Number, ex: 1
#define ZIGBEE_SS_PORT A //Letter, ex: A
#define ZIGBEE_SS_PIN 4 //Number, ex: 1

#endif // PROJECT_PERIPHERALS_STM32F37X_H
