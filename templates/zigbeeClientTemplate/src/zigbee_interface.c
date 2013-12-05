#include "zigbee_interface.h"
#include "zigbee.h"
#include "crc_helper.h"

/*******************************************************************************
 * Initialize
 ******************************************************************************/

void InitZigbeeInterface(struct ZigbeeInterface *z)
{
  InitBQ(&z->index_queue, z->pf_index_data, ZI_PF_INDEX_DATA_SIZE);
  InitPacketFinder(&z->pf, &z->index_queue);
  InitFQ(&z->out_queue, z->out_queue_data, ZI_OUT_QUEUE_DATA_SIZE, MAX_ZIGBEE_PACKET_BYTES);
  //SetSendInCallback(SendUsbNow); Missing zigbee version
}

/*******************************************************************************
 * Receive
 ******************************************************************************/
 
int8_t GetZigbeeBytes(struct ZigbeeInterface *z)
{
  uint8_t receive_buffer[130]; // Max buffer size of AT86RF should be 130
  uint8_t receive_length = 0;
  // Get the data
  uint8_t zb_irq_stat = ZigBee_ReadIrq();
  if((zb_irq_stat & 0x08) == 0x08)
  {
    ZigBee_ReadFrame(receive_buffer, &receive_length);
    if(receive_length > 3) // 3 status bytes from the radio + actual data
    {
      if(receive_buffer[receive_length-1] & 0x80) // Valid CRC
      {
        int8_t status = PutBytes(&z->pf, receive_buffer, receive_length-3); // -3 for status bytes
        return(status);
      }
      else
      {
        return(0);
      }
    }
    else
    {
      return(0);
    }
  }
  return(0);
}

int8_t PeekZigbeePacket(struct ZigbeeInterface *z, uint8_t **packet, uint8_t *length) 
{
  return(PeekPacket(&z->pf, packet, length));
}

int8_t DropZigbeePacket(struct ZigbeeInterface *z) 
{
  return(DropPacket(&z->pf));
}

/*******************************************************************************
 * Send
 ******************************************************************************/

int8_t SendZigbeePacket(struct ZigbeeInterface *z,uint8_t msg_type, uint8_t *data, uint16_t length) {
  // This function must not be interrupted by another call to SendUsbBytes or 
  // SendUsbPacket, or else the packet it builds will be spliced/corrupted.

  uint8_t header[3];
  header[0] = kStartByte;                   // const defined by packet_finder.c
  header[1] = length;
  header[2] = msg_type;
  SendZigbeeBytes(z,header, 3);
  
  SendZigbeeBytes(z,data, length);
  
  uint8_t footer[2];
  uint16_t crc;
  crc = MakeCrc(&(header[1]), 2);
  crc = ArrayUpdateCrc(crc, data, length);
  footer[0] = crc & 0x00FF;
  footer[1] = crc >> 8;
  SendZigbeeBytes(z,footer, 2);
  
  return(1);
}

int8_t SendZigbeeBytesNow(struct ZigbeeInterface *z, uint8_t *bytes, uint8_t length)
{
  int8_t ret;
  ret = SendZigbeeBytes(z,bytes,length);
  SendZigbeeNow(z);
  return ret;
}

int8_t SendZigbeeBytes(struct ZigbeeInterface *z, uint8_t *bytes, uint8_t length)
{
  PutBytesFQ(&z->out_queue, bytes, length);
  
  uint8_t* next_packet;
  uint16_t next_length;
  PeekFrameFQ(&z->out_queue, &next_packet, &next_length);
  
  if(next_length >= MAX_ZIGBEE_PACKET_BYTES-1) {
    SendZigbeeNow(z);
  }
  
  return(1);
}

void SendZigbeeNow(struct ZigbeeInterface *z)
{
  //TODO:: write non-blocking zigbee code
  if(!IsEmptyFQ(&z->out_queue)) {
    ZigBee_SetModeTx();
    uint8_t* packet;
    uint16_t length;
    PeekFrameFQ(&z->out_queue, &packet, &length);
    ZigBee_WriteFrame(packet, length);
    int ret = ZigBee_ReadIrq();
    while((ret & 0x08) == 0x00)
    {
      /*if(ret != 0x00)
      {
        if((ret & 0x40) != 0)
        {
          // Frame Buffer access violation
          //TODO:: Clear buffer, lose packet, start fresh
        }
        else if((ret & 0x02) != 0)
        {
          // PLL unlock
          //TODO:: LOCK BACK ON!!!!
        }
        else if((ret & 0x01) != 0)
        {
          // PLL lock
          // TODO:: Set mode to Rx?  Try to resend? Break? Return?
        }
      }
      else
      {*/
        ret = ZigBee_ReadIrq();
      //}
    }
    ZigBee_SetModeRx();
    DropFrameFQ(&z->out_queue); 
  }
}
