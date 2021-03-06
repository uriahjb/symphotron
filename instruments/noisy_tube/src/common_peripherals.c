/**
** This file provides an interface to each peripheral.
** Edit the #define statements at the top of the file to
** enable and specifiy the location of the peripherals.
** The remainder of the file does the setting up of the peripherals.
**/

#include <stdint.h>
#include "common_peripherals.h"
#include "systick.h" // for systick_init()
#include "delay.h" // for _delay_ms()
#include "stm32f37x_misc.h" // for interrupt config

void InitPeripherals() {

/// ======================  Don't touch past here  ============================

// LED Initialization
#ifdef USING_GPIO
  GPIO_InitTypeDef GPIO_InitStructure;
#endif

// #ifdef USING_AMBER_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_AMBER_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_AMBER_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_AMBER_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_AMBER_PORT_FULL, 1<<LED_AMBER_PIN);
// 	mAmberOFF;
// #endif


// #ifdef USING_BLUE_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_BLUE_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_BLUE_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_BLUE_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_BLUE_PORT_FULL, 1<<LED_BLUE_PIN);
// 	mBlueOFF;
// #endif


// #ifdef USING_CYAN_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_CYAN_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_CYAN_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_CYAN_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_CYAN_PORT_FULL, 1<<LED_CYAN_PIN);
// 	mCyanOFF;
// #endif

// #ifdef USING_GREEN_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_GREEN_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_GREEN_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_GREEN_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_GREEN_PORT_FULL, 1<<LED_GREEN_PIN);
// 	mGreenOFF;
// #endif

// #ifdef USING_ORANGE_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_ORANGE_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_ORANGE_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_ORANGE_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_ORANGE_PORT_FULL, 1<<LED_ORANGE_PIN);
// 	mOrangeOFF;
// #endif

// #ifdef USING_PINK_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_PINK_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_PINK_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_PINK_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_PINK_PORT_FULL, 1<<LED_PINK_PIN);
// 	mPinkOFF;
// #endif

// #ifdef USING_PURPLE_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_PURPLE_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_PURPLE_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_PURPLE_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_PURPLE_PORT_FULL, 1<<LED_PURPLE_PIN);
// 	mPurpleOFF;
// #endif

// #ifdef USING_RED_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_RED_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_RED_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_RED_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_RED_PORT_FULL, 1<<LED_RED_PIN);
// 	mRedOFF;
// #endif

// #ifdef USING_TURQUOISE_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_TURQUOISE_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_TURQUOISE_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_TURQUOISE_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_TURQUOISE_PORT_FULL, 1<<LED_TURQUOISE_PIN);
// 	mTurquoiseOFF;
// #endif

// #ifdef USING_WHITE_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_WHITE_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_WHITE_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_WHITE_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_WHITE_PORT_FULL, 1<<LED_WHITE_PIN);
// 	mWhiteOFF;
// #endif


// #ifdef USING_YELLOW_LED
//   RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,LED_YELLOW_PORT), ENABLE);
// 	GPIO_InitStructure.GPIO_Pin =  1<<LED_YELLOW_PIN;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(LED_YELLOW_PORT_FULL, &GPIO_InitStructure);
// 	GPIO_PinLockConfig(LED_YELLOW_PORT_FULL, 1<<LED_YELLOW_PIN);
// 	mYellowOFF;
// #endif


/* 2 bit for pre-emption priority, 2 bits for subpriority */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

// mBus Initialization
// #ifdef USING_MBUS1
//   mBusInit(1);
// #endif
// #ifdef USING_MBUS2
//   mBusInit(2);
// #endif

// Setup system timer for delay
systick_init();

// Zigbee Initialization
#ifdef ZIGBEE_SPI
  ZigBee_InitCommon();
  ZigBee_Reset();
  if(ZigBee_Init() != 0)
  {
    //My radio is messed up
  }
  _delay_ms(10);
  ZigBee_EnableTrxEndIrq();
  _delay_ms(1);
 /*  uint8_t zigbee_ret = ZigBee_ReadState();
  while(zigbee_ret != STATUS_PLL_ON)
  {
    zigbee_ret = ZigBee_ReadState();
    switch(zigbee_ret)
    {
    case 0:
      mRedTOGGLE; break;
    case 1:
      mWhiteTOGGLE; break;
    case 2:
      mGreenTOGGLE; break;
    case 6:
      mBlueTOGGLE; break;
    case 8:
      mRedTOGGLE; mWhiteTOGGLE; break;
    case 9:
      mRedTOGGLE; mGreenTOGGLE; break;
    case 0xF:
      mRedTOGGLE; mBlueTOGGLE; break;
    case 0x11:
      mRedTOGGLE; mWhiteTOGGLE; mGreenTOGGLE; break;
    case 0x12:
      mRedTOGGLE; mWhiteTOGGLE; mBlueTOGGLE; break;
    case 0x16:
      mRedTOGGLE; mWhiteTOGGLE; mBlueTOGGLE; mGreenTOGGLE; break;
    case 0x19:
      mWhiteTOGGLE; mGreenTOGGLE; break;
    case 0x1C:
      mWhiteTOGGLE; mBlueTOGGLE; break;
    case 0x1D:
      mWhiteTOGGLE; mGreenTOGGLE; mBlueTOGGLE; break;
    case 0x1E:
      mGreenTOGGLE; mBlueTOGGLE; break;
    case 0x1F:
      mRedTOGGLE; mBlueTOGGLE; break;
  }
  _delay_ms(50);
  } */
#endif

// USB Initialization
// #ifdef USING_USB
//   _delay_ms(200); // Don't know why it needs this, but it does
//   mUSBInit();
// #endif

// // EEPROM Initialization
// #if defined(EEPROM_NUMBER_VALUES) && EEPROM_NUMBER_VALUES > 0
//   EE_Init();
// #endif

#ifdef TIMEBASE_TIMER
  initTimeBase();
#endif

#ifdef SYSTEM_CLOCK_TIMER
  InitSystemClock();
#endif

#if defined(ENCODER_QEP_TIMER) || defined(ENCODER_PWM_TIMER)
  initEncoder();
  enable_PWM_timer();
  enable_QEP_timer();
  start_encoder(0);
#endif

#if defined(SYNC_TIMER_HIGH) && defined(SYNC_TIMER_LOW)
  SetSyncTimers();
#endif

}
