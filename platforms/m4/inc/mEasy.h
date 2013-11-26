//-----------------------------------------------------------------------------
// MAEVARM M4 STM32F373 m_easy
// version: 1.0
// date: Nov 7, 2013
// author: Tyler Barkin (tbarkin@seas.upenn.edu)
//-----------------------------------------------------------------------------
//#ifndef __m_easy
#define __m_easy
#define MODE_OUT 0
#define MODE_IN 1
#define MODE_ANALOG 2
#define MODE_ALT_FUNCTION 3

#include "mGeneral.h"
#include "stdio.h"
#include "stm32f37x.h"

void set_gpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,int a);
void set_pwm_servo (uint16_t Per,uint16_t Pul,uint16_t PreScale);
void set_pwm_stepper (uint16_t Per,uint16_t Pul,uint16_t PreScale);
void modulatePick(uint32_t i);
void modulateArm(uint32_t i);

void G_interrupt(void);
void pin_init(void);
void TIM_Config(void);


//#endif
