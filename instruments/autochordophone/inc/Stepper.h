/*
 Stepper Interfacing Code using DRV8824/DRV8825
 Provides a speed setter/getter interface, keeps track of the current stepper counts.
 */
//#include "mbed.h"
//
#ifndef STEPPER_H
#define STEPPER_H

// Modes
#include "mEasy.h"
#include <stdlib.h>
#include "stdint.h"
#include "mGeneral.h"
#include "stm32f37x.h"
#include <math.h>
#include "arm_math.h"

#define Clock_Divide 30
#define STEP_FULL 0
#define STEP_HALF 1
#define STEP_4TH 2
#define STEP_8TH 3
#define STEP_16TH 4
#define STEP_32ND 5

void stepper_setup(void);
void set_speed(uint16_t speed);

void Stepper( void );
void Start( void );
void stop( void );
void accStart(void);
void accStop(void);
void updateCount(void);
uint32_t getCount( void );

int getModeDivisor( void );
void setMode( int mode );

float getAccelerationLimit( void );
void setAccelerationLimit( float acceleration );

float getSpeed( void );
void setSpeedLimits( float speed_min, float speed_max );

int getPosition( void );
void setPosition( int32_t desired_position );

int update( int16_t Note);
void Arrived(void);

#endif
