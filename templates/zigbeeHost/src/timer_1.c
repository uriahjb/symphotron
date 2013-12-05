/*
AUTORELOAD IS THE LOWER VALUE THE TIMER COUNTER TAKES AFTER EACH OVERFLOW (NOT USED HERE)
MAX FREQUENCY OF 80KHZ WITH THIS CONFIGURATION (IT SEEMS THAT TIM_PERIOD DIVIDES THE FREQUENCY BY
	ITS VALUE)
MAXIMUM ALLOWED FREQUENCY FOR APB1 PERIPH (MOST TIMERS, WWDG) IS 36MHZ
	FOR APB2 PERIPH ITS UPTO 72HMZ
THE ACTIVE MODE JUST GENERATES A SIGNAL WHEN CNT MATCHES THE CCRx VALUE
	AND THE COMES BACK LOW
THE INTERRUPT IS GENERATED AT A HIGHER FREQUENCY COMPARED 
	TO THE TIMER FREQUENCY(ATLEAST THATS WHAT THE LED SAYS)
*/
#include "stm32f37x.h"
#include "mGeneral.h"

//prototype functions
void TIM_Config(uint16_t pulse_1,uint16_t pulse_2)
{
	//Configure the GPIO piins for TIMER
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Pins PB0 and PB1 for Timer 3 Channel 3 and 4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	//Configure the pins to TIMER 3 Alternate Function (AF2 for both)
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2); 
	
	//Enable the TIMER2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//TIMER 2 base Initialization
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	//TIMER base configuration
	uint16_t PrescalerValue = 0;
	PrescalerValue = (uint16_t)(SystemCoreClock/10000) - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 1000;//PrescalerValue;
	TIM_TimeBaseStructure.TIM_Period = 255;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//Set the TIMER to Output Compare Mode
	TIM_OCInitTypeDef TIM_OCInitStructure ,TIM_OCInitStructure_1;
	TIM_OCStructInit(&TIM_OCInitStructure); 
	
	/* Output Compare Active Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pulse_1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Not really required. can always change CCRx value or mode to PWM2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	
	//TIM_ARRPreloadConfig(TIM2, DISABLE); 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //Required for PWM mode
	
	// Channel 3
	TIM_OCInitStructure_1.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure_1.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure_1.TIM_Pulse = pulse_2;
	TIM_OCInitStructure_1.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure_1);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //Required for PWM mode
	
	//TIMER2 INTERRUPT
	
	//Setl the interrupts global structure
	// NVIC_InitTypeDef NVIC_InitStructure;
	// /* Enable the TIM2 global Interrupt */
	// NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
	
	/* TIM IT enable */
	// TIM_ITConfig(TIM2, TIM_IT_CC2,ENABLE);// | TIM_IT_CC3, ENABLE);
	TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
	//TIMER 2 Counter Enable
	TIM_Cmd(TIM3,ENABLE);
	
}