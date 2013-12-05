#ifndef ZIGBEE_INTERFACE_H
#define ZIGBEE_INTERFACE_H

#include "packet_finder.h"
#include "frame_queue.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ZI_PF_INDEX_DATA_SIZE 20
#define ZI_OUT_QUEUE_DATA_SIZE 252 // size of out buffer in usb_interface

/* 
 * ZigbeeInterface instance state struct, voluntarily opaque.  
 * The user must create a ZigbeeInterface struct for each zigbee device.
 */
struct ZigbeeInterface {
  struct PacketFinder pf;         // packet_finder instance
  struct ByteQueue index_queue;   // needed by pf for storing indices
  struct FrameQueue out_queue;    // frame_queue instance for outgoing bytes
  uint8_t pf_index_data[ZI_PF_INDEX_DATA_SIZE]; // data for index_queue used by pf
  uint8_t out_queue_data[ZI_OUT_QUEUE_DATA_SIZE];  // data for out_queue
};

/* Initialize the zigbee packetizing interface struct
 */
void InitZigbeeInterface(struct ZigbeeInterface *z);

/* Poll the Zigbee hardware for new byte data.
 *   Returns: 1 packet ready
 *            0 normal operation
 *           -1 failure
 */
int8_t GetZigbeeBytes(struct ZigbeeInterface *z);

/* Peek at the next available incoming packet. If a packet is ready, pointer 
 * 'packet' will point to the first byte of type+data and 'length' will give the length of 
 * packet type+data. Arguments 'packet' and 'length' are ignored if no packet is 
 * ready.  Repeated calls to Peek will return pointers to the same packet 
 * data until Get is used.
 *   Returns: 1 packet peek available
 *            0 no packet peek available
 *           -1 failure
 */
int8_t PeekZigbeePacket(struct ZigbeeInterface *z, uint8_t **packet, uint8_t *length);

/* Drop the next available packet from queue. Usually called after Peek.
 *   Returns: 1 packet removed
 *            0 no packet ready to remove
 *           -1 failure
 */
int8_t DropZigbeePacket(struct ZigbeeInterface *z);

/*
 * Add a packet to the outgoing Zigbee queue with automatically generated header 
 * and CRC. A hardware transmission is not immediatly initiated unless the 
 * endpoint is filled. To force a transmission, follow with SendZigbeeNow(). This 
 * operation is nonblocking. If the buffer fills, the most recent data is lost.
 */
int8_t SendZigbeePacket(struct ZigbeeInterface *z, uint8_t msg_type, uint8_t *data, uint16_t length);

/* 
 * Add bytes to the outgoing Zigbee queue. Initiates a hardware transmission, 
 * which will chain transmissions until the buffer empties. This operation is 
 * nonblocking. If the buffer fills, the most recent data is lost.
*/
int8_t SendZigbeeBytesNow(struct ZigbeeInterface *z, uint8_t *bytes, uint8_t length);

/*  
 Add bytes to the outgoing Zigbee queue. A hardware transmission is not 
 immediatly initiated unless the endpoint is filled. To force a 
 transmission, follow with SendZigbeeNow(). This operation is 
 nonblocking. If the buffer fills, the most recent data is lost. 
 */
int8_t SendZigbeeBytes(struct ZigbeeInterface *z, uint8_t *bytes, uint8_t length);

/* 
 Initiate a hardware transmission, which will chain transmissions through 
 the endpoint IN interrupt until the buffer empties completely. 
*/
void SendZigbeeNow(struct ZigbeeInterface *z);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ZIGBEE_INTERFACE_H
