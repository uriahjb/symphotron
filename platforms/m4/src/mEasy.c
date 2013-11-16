//-----------------------------------------------------------------------------
// MAEVARM M4 STM32F373 mEasy
// version: 1.0
// date: Novermber 7, 2013
// author: Tyler Barkin (tbarkin@seas.upenn.edu)
//-----------------------------------------------------------------------------
#include "mEasy.h"

uint16_t PrescalerValue = 0;

void set_gpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,int a){
    static GPIO_InitTypeDef GPIO_InitStructure;
    
    if(GPIOx==GPIOA){
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    mGreenON;
    }
    else if(GPIOx==GPIOB)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    else if(GPIOx==GPIOC)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    else if(GPIOx==GPIOE)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
    else if(GPIOx==GPIOF)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
    else
        mYellowON;
    
    if(a==0)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //out
    else if(a==1)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //in
    else if(a==2)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //analog (for DAC or ADC)
    else{
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //Alt Function for PWM
        mBlueON;
    }
    
    
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//Temp
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //Temp
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}
void set_pwm (uint16_t Per,uint16_t Pul,uint16_t PreScale){
    NVIC_InitTypeDef NVIC_InitStructure;
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    
    
    
    
    /* TIM12 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
    
    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM12_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = Per;
    TIM_TimeBaseStructure.TIM_Prescaler = PreScale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
    
    /* Output Compare Toggle Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Pul;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM12, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Disable);
    
    /* Output Compare Toggle Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse = Pul;
    
    TIM_OC2Init(TIM12, &TIM_OCInitStructure);
    
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Disable);
    
    /* Output Compare Toggle Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Pul;
    
    TIM_OC3Init(TIM12, &TIM_OCInitStructure);
    
    TIM_OC3PreloadConfig(TIM12, TIM_OCPreload_Disable);
    
    /* Output Compare Toggle Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Pul;
    
    TIM_OC4Init(TIM12, &TIM_OCInitStructure);
    
    TIM_OC4PreloadConfig(TIM12, TIM_OCPreload_Disable);
    
    /* TIM enable counter */
    TIM_Cmd(TIM12, ENABLE);
    
    /* TIM IT enable */
    TIM_ITConfig(TIM12, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
    TIM_GenerateEvent(TIM12, TIM_EventSource_Update);
    
}/*Not even close to complete yet. Needs a lot a lot of work and decisions*/



void G_interrupt(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void pin_init(void){

    GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* GPIOA Configuration: PA0(TIM2 CH1 CH2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Connect TIM pins to AF1 */
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1);
    
}


void TIM_Config(void)
{
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_10);
    /*
     RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI); //8MHz Clock
     RCC_HCLKConfig(RCC_SYSCLK_Div1);       //Divide by 1
     */
    
    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
    //Timer 2 base Initialization
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    //Set Timer 2 to Output Compare Mode
    TIM_OCInitTypeDef        TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    
    
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((SystemCoreClock ) / 36000000) - 1;
    
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 10000;  //PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
    
    /* Output Compare Active Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 500;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1Init(TIM12, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM2, DISABLE);
    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM12, DISABLE);
    
    /* Output Compare Toggle Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;
    
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
    
    
    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM12, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
    
    /* TIM2 enable counter */
    TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
    TIM_Cmd(TIM2, ENABLE);
    TIM_GenerateEvent(TIM12, TIM_EventSource_Update);
    TIM_Cmd(TIM12, ENABLE);
}
void modulateArm(uint32_t i){
        TIM12->CCR1 = i;
//    static TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_OCInitStructure.TIM_Pulse = i;
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC1Init(TIM12, &TIM_OCInitStructure);

}
void modulatePick(uint32_t i){
    TIM12->CCR2 = i;
//    static TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_OCInitStructure.TIM_Pulse = i;
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC2Init(TIM12, &TIM_OCInitStructure);
    
}