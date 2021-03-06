//Copyright KMel Robotics, LLC, 2012

#include "zigbee.h"
#include "stm32f37x.h"
#include "stm32f37x_spi.h"
#include "stm32f37x_rcc.h"
#include "stm32f37x_gpio.h"
#include "stm32f37x_dma.h"
#include "delay.h"
#include "common_peripherals.h"

//#define DEBUG
#if defined(DEBUG)
#include <string.h>
#include <stdio.h>
#endif

#ifdef ZIGBEE_SPI

static inline void PullDown()
{
  GPIO_ResetBits(MAKENAME(GPIO,ZIGBEE_SS_PORT), MAKENAME(GPIO_Pin_,ZIGBEE_SS_PIN));
}

static inline void PullUp()
{
  GPIO_SetBits(MAKENAME(GPIO,ZIGBEE_SS_PORT), MAKENAME(GPIO_Pin_,ZIGBEE_SS_PIN));
}

void ZigBee_InitPins()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,ZIGBEE_SCK_PORT), ENABLE);
  RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,ZIGBEE_MISO_PORT), ENABLE);
  RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,ZIGBEE_MOSI_PORT), ENABLE);
  RCC_AHBPeriphClockCmd(MAKENAME(RCC_AHBPeriph_GPIO,ZIGBEE_SS_PORT), ENABLE);
  
  //SCK

  GPIO_InitStructure.GPIO_Pin   = MAKENAME(GPIO_Pin_,ZIGBEE_SCK_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(MAKENAME(GPIO,ZIGBEE_SCK_PORT), &GPIO_InitStructure);
  
  //MISO

  GPIO_InitStructure.GPIO_Pin   = MAKENAME(GPIO_Pin_,ZIGBEE_MISO_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(MAKENAME(GPIO,ZIGBEE_MISO_PORT), &GPIO_InitStructure);
  
  //MOSI

  GPIO_InitStructure.GPIO_Pin   = MAKENAME(GPIO_Pin_,ZIGBEE_MOSI_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(MAKENAME(GPIO,ZIGBEE_MOSI_PORT), &GPIO_InitStructure);

  // slave select
  
  GPIO_InitStructure.GPIO_Pin   = MAKENAME(GPIO_Pin_,ZIGBEE_SS_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(MAKENAME(GPIO,ZIGBEE_SS_PORT), &GPIO_InitStructure);

  // Configure SPI pins: SCK, MISO and MOSI
  GPIO_PinAFConfig(MAKENAME(GPIO,ZIGBEE_SCK_PORT), MAKENAME(GPIO_PinSource,ZIGBEE_SCK_PIN), MAKENAME(GPIO_AF_,ZIGBEE_SCK_AF));
  GPIO_PinAFConfig(MAKENAME(GPIO,ZIGBEE_MISO_PORT), MAKENAME(GPIO_PinSource,ZIGBEE_MISO_PIN), MAKENAME(GPIO_AF_,ZIGBEE_MISO_AF));
  GPIO_PinAFConfig(MAKENAME(GPIO,ZIGBEE_MOSI_PORT), MAKENAME(GPIO_PinSource,ZIGBEE_MOSI_PIN), MAKENAME(GPIO_AF_,ZIGBEE_MOSI_AF));
  
#ifdef ZIGBEE_RESET_PIN
  GPIO_InitStructure.GPIO_Pin   = MAKENAME(GPIO_Pin_,ZIGBEE_RESET_PIN);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(MAKENAME(GPIO,ZIGBEE_RESET_PORT), &GPIO_InitStructure);
#endif
}

void ZigBee_InitSpi()
{
  RCC_APB1PeriphClockCmd(MAKENAME(RCC_APB1Periph_SPI,ZIGBEE_SPI), ENABLE);
  // SPI initialization
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial     = 7;
  SPI_Init(MAKENAME(SPI,ZIGBEE_SPI), &SPI_InitStructure);

  SPI_SSOutputCmd(MAKENAME(SPI,ZIGBEE_SPI), ENABLE); // MOST IMPORTANT LINE FOR SPI !!!!
  SPI_RxFIFOThresholdConfig(MAKENAME(SPI,ZIGBEE_SPI), SPI_RxFIFOThreshold_QF);

  SPI_Cmd(MAKENAME(SPI,ZIGBEE_SPI), ENABLE);

  /*
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel                   = MAKENAME(SPI,ZIGBEE_SPI)_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  */
}

int ZigBee_InitCommon()
{
  //initialize pins
  ZigBee_InitPins();

  //initialize spi
  ZigBee_InitSpi();

  _delay_ms(1);

  return 0;
}

uint8_t ZigBee_GetPartNum()
{
  uint8_t resp;
  ZigBee_ReadReg(REG_PART_NUM,1,&resp);
  return resp;
}

uint8_t ZigBee_GetVersionNum()
{
  uint8_t resp;
  ZigBee_ReadReg(REG_VERS_NUM,1,&resp);
  return resp;
}

int ZigBee_CheckPartAndVers()
{
  uint8_t part, vers;
  int ret = -1;

  part = ZigBee_GetPartNum();

  _delay_us(1);

  vers = ZigBee_GetVersionNum();

#if defined(DEBUG)
  printf("got part num %d and version num %d\r\n",part,vers);
#endif

  if ( (part == 3) && (vers == 2) )
  {
    ret = 0;
#if defined(DEBUG)
    printf("found 2.4Ghz zigbee\r\n");
#endif
  }
  else if ( (part == 7) && ((vers == 1) || (vers == 3)))
  {
    ret = 0;
#if defined(DEBUG)
    printf("found 900Mhz zigbee\r\n");
#endif
  }

  return ret;
}

int ZigBee_ConfigClock()
{
  int ret = 0;

  //2mA all pins, CLKM disabled
  ZigBee_WriteReg(REG_TRX_CTRL_0,0x00);

  _delay_us(2);

  // Internal crystal disabled, use external freq input
  ZigBee_WriteReg(REG_XOSC_CTRL,0x40);

  return ret;
}

uint8_t ZigBee_ReadState()
{
  uint8_t state;
  ZigBee_ReadReg(REG_TRX_STATUS,1,&state);
  state &= 0x1F;

  return state;
}

int ZigBee_ForceTrxOff()
{
  uint8_t zigbee_state = ZigBee_ReadState();
  while(zigbee_state == STATUS_BUSY_RX || zigbee_state == STATUS_BUSY_TX || zigbee_state == STATUS_STATE_TRANSITION)
  {
    zigbee_state = ZigBee_ReadState();
  }

  //write FORCE_TRX_OFF to TRX_STATE
  ZigBee_WriteReg(REG_TRX_STATE, CMD_FORCE_TRX_OFF);

  uint8_t state = 0;
  for(int i = 0; i < 300; i++)
  {
    state = ZigBee_ReadState();
    if (state == STATUS_TRX_OFF)
      return(0);
  }

  return(-1);
}

int ZigBee_PllOn()
{
  uint8_t zigbee_state = ZigBee_ReadState();
  while(zigbee_state == STATUS_BUSY_RX || zigbee_state == STATUS_BUSY_TX || zigbee_state == STATUS_STATE_TRANSITION)
  {
    zigbee_state = ZigBee_ReadState();
  }
#if defined(DEBUG)
  printf("zigbee setting PLL ON\r\n");
#endif
  ZigBee_WriteReg(REG_TRX_STATE, CMD_PLL_ON);

  uint8_t state = 0;
  for(int i = 0; i < 100; i++)
  {
    state = ZigBee_ReadState();
    if (state == STATUS_PLL_ON)
      return(0);
  }

  return(-1);
}

int ZigBee_ReadIrq()
{
  uint8_t resp;
  ZigBee_ReadReg(REG_IRQ_STATUS,1,&resp);
  return resp;
}

int ZigBee_EnableTrxEndIrq()
{
  ZigBee_WriteReg(REG_IRQ_MASK,0b00001000);
  return 0;
}

int ZigBee_SetModeTx()
{
  return ZigBee_PllOn();
}

int ZigBee_SetModeRx()
{
  uint8_t zigbee_state = ZigBee_ReadState();
  while(zigbee_state == STATUS_BUSY_RX || zigbee_state == STATUS_BUSY_TX || zigbee_state == STATUS_STATE_TRANSITION)
  {
    zigbee_state = ZigBee_ReadState();
  }

  ZigBee_WriteReg(REG_TRX_STATE, CMD_RX_ON);

  uint8_t state = 0;
  for(int i = 0; i < 100; i++)
  {
    state = ZigBee_ReadState();
    if (state == STATUS_RX_ON)
      return(0);
  }

  return(-1);
}

int ZigBee_Init()
{

  if ( ZigBee_CheckPartAndVers() == 0 )
  {
#if defined(DEBUG)
    printf("zigbee found!\r\n");
#endif
  }
  else
  {
#if defined(DEBUG)
    printf("zigbee NOT found!!\r\n");
#endif
    return -1;
  }

  ZigBee_ConfigClock();

  _delay_us(10);

  if (ZigBee_ForceTrxOff() != 0)
    return -1;

  //enable Auto CRC, disable RX buffer empty IRQ
  ZigBee_WriteReg(REG_TRX_CTRL_1, 0x20);

  _delay_us(2);

  ZigBee_WriteReg(REG_TRX_CTRL_2, 0x2C); //modulation: OQPSK-SIN-250

  _delay_us(2);

  ZigBee_WriteReg(REG_PHY_TX_PWR, 0xE1);   //output power setting to 10dBm

  if (ZigBee_SetModeRx() == 0)
  {
#if defined(DEBUG)
    printf("zigbee initialization complete.\r\n");
#endif
  }
  else
  {
#if defined(DEBUG)
    printf("zigbee initialization FAILED.\r\n");
#endif
/*     while(1)
    {
      uint8_t zigbee_ret = ZigBee_ReadState();
      switch(zigbee_ret)
      {
      case STATUS_P_ON:
        mRedTOGGLE; break;
      case STATUS_BUSY_RX:
        mWhiteTOGGLE; break;
      case STATUS_BUSY_TX:
        mGreenTOGGLE; break;
      //case STATUS_RX_ON:
        //mAmberTOGGLE; break;
      case STATUS_TRX_OFF:
        mRedTOGGLE; mWhiteTOGGLE; break;
      case STATUS_PLL_ON:
        mRedTOGGLE; mGreenTOGGLE; break;
      case 0xF:
        mRedTOGGLE; mAmberTOGGLE; break;
      case 0x11:
        mRedTOGGLE; mWhiteTOGGLE; mGreenTOGGLE; break;
      case 0x12:
        mRedTOGGLE; mWhiteTOGGLE; mAmberTOGGLE; break;
      case 0x16:
        mRedTOGGLE; mWhiteTOGGLE; mAmberTOGGLE; mGreenTOGGLE; break;
      case 0x19:
        mWhiteTOGGLE; mGreenTOGGLE; break;
      case 0x1C:
        mWhiteTOGGLE; mAmberTOGGLE; break;
      case 0x1D:
        mWhiteTOGGLE; mGreenTOGGLE; mAmberTOGGLE; break;
      case 0x1E:
        mGreenTOGGLE; mAmberTOGGLE; break;
      case 0x1F:
        mAmberTOGGLE; break;
        int irq_ret = ZigBee_ReadIrq();
        if((irq_ret & 0x40) != 0)
        {
          // Frame Buffer access violation
          mRedON;
          mWhiteON;
        }
        else if((irq_ret & 0x02) != 0)
        {
          // PLL unlock
          mRedON;
          mGreenON;
        }
        else if((irq_ret & 0x01) != 0)
        {
          // PLL lock
          mGreenON;
          mWhiteON;
        }
      }
      _delay_ms(500);
    } */
    return -1;
  }

  return 0;
}

int ZigBee_GetEdMeasurement()
{
  uint8_t ed;

  ZigBee_ReadReg(REG_PHY_ED_LEVEL, 1, &ed);
  return ed;
}

void ZigBee_StartEdMeasurement()
{
  ZigBee_WriteReg(REG_PHY_ED_LEVEL, 0xFF);
}

void ZigBee_ReadReg(uint8_t addr, uint8_t len, uint8_t * dest)
{
  uint8_t resp;

  SPI_TypeDef*   SPI = MAKENAME(SPI,ZIGBEE_SPI);

  //read off any remaining bytes
  while(SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE))
    resp = SPI_ReceiveData8(SPI);

  PullDown();
  _delay_us(1);

  for(uint8_t ii=0; ii<len; ii++)
  {
    //wait until TX buffer is empty
    while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE));

    uint8_t cmd = addr;
    cmd |= 0x80; // read cmd

    SPI_SendData8(SPI, cmd);  //send the command
    while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
    resp = SPI_ReceiveData8(SPI);  //this is PHY_STATUS byte from ZigBee

    SPI_SendData8(SPI, 0); // send dummy
    while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
    resp = SPI_ReceiveData8(SPI);

    *dest++ = resp;
  }

  _delay_us(1);

  PullUp();
}

void ZigBee_WriteReg(uint8_t addr, uint8_t val)
{
  SPI_TypeDef*  SPI = MAKENAME(SPI,ZIGBEE_SPI);

  //read off any remaining bytes
  while(SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE))
    SPI_ReceiveData8(SPI);

  //enable NSS pin
  PullDown();
  _delay_us(1);

  //wait until TX buffer is empty
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE));

  uint8_t cmd = addr;
  cmd |= 0xC0; // write cmd

  SPI_SendData8(SPI, cmd);
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  SPI_ReceiveData8(SPI);

  SPI_SendData8(SPI, val);
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  SPI_ReceiveData8(SPI);

  _delay_us(1);
  PullUp();
}

void ZigBee_ReadFrame(uint8_t * data, uint8_t * len)
{
  uint8_t resp;

  SPI_TypeDef*   SPI = MAKENAME(SPI,ZIGBEE_SPI);

  //read off any bytes in the buffer
  while(SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  resp = SPI_ReceiveData8(SPI);

  //enable select pin
  PullDown();
  _delay_us(1);

  //wait until TX buffer is empty
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE));

  //send the read command
  SPI_SendData8(SPI, 0x20);
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  resp = SPI_ReceiveData8(SPI);

  //receive the buffer length
  SPI_SendData8(SPI, 0);
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  resp = SPI_ReceiveData8(SPI);

  uint8_t nbytes = resp+3; //packet + lqi + ed + status (link quality indicatior) (energy detection)
  *len = nbytes;           //set the return value
  
  // Datasheet says max buffer access is 5 + frame length = 132
  // 2 reads come before frame, 3 after
  // First 2 reads not included in nbytes
  // Therefore, max nbytes size is 132 - 2 = 130
  if (nbytes > 130) 
  {
    *len = 0;
    _delay_us(1);
    PullUp();
    return;
  }
  for (uint8_t ii=0; ii<nbytes; ii++)
  {
    SPI_SendData8(SPI, 0);
    while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
    resp = SPI_ReceiveData8(SPI); //receive data
    *data++ = resp;
  }

  _delay_us(1);
  PullUp();
}

uint8_t ZigBee_WriteFrame(const void* data, uint8_t len)
{
  if (len > MAX_ZIGBEE_PACKET_BYTES)
    return -1;

  SPI_TypeDef*   SPI = MAKENAME(SPI,ZIGBEE_SPI);
  const uint8_t *tx_ptr = data;

  //clear any remaining bytes
  while(SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE))
    SPI_ReceiveData8(SPI);

  PullDown();
  _delay_us(1);

  //wait until TX buffer is empty
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE));

  SPI_SendData8(SPI, 0x60);
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  SPI_ReceiveData8(SPI);

  SPI_SendData8(SPI, len+2); //two extra bytes for the 16-bit CRC
  while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
  SPI_ReceiveData8(SPI);

  for(int ii = 0; ii < len; ii++)
  {
    SPI_SendData8(SPI, tx_ptr[ii]);
    while(!SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE));
    SPI_ReceiveData8(SPI);
  }

  _delay_us(1);
  PullUp();

  ZigBee_WriteReg(REG_TRX_STATE, 0x02);  //alternate method for signaling to send

  return 0;
}

int ZigBee_SetChannel(uint8_t ch)
{
  if (ch > 102)
    return -1;

  ZigBee_WriteReg(REG_CC_CTRL_1, 5);  //833Mhz + channel number
  ZigBee_WriteReg(REG_CC_CTRL_0, ch);

  return 0;
}

void ZigBee_Reset(void)
{
#ifdef ZIGBEE_RESET_PIN
GPIO_ResetBits(MAKENAME(GPIO,ZIGBEE_RESET_PORT), MAKENAME(GPIO_Pin_,ZIGBEE_RESET_PIN)); // Toggle ATRF reset pin
  _delay_ms(10);
GPIO_SetBits(MAKENAME(GPIO,ZIGBEE_RESET_PORT), MAKENAME(GPIO_Pin_,ZIGBEE_RESET_PIN)); // Toggle ATRF reset pin
#endif
}

#endif //ZIGBEE_SPI