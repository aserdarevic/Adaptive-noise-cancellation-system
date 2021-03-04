/*!
 * \file      audio.h
 * \brief     Implements the audio configuration
 * \copyright ARM University Program &copy; ARM Ltd 2015.
 *
 */

#ifndef audio_H
#define audio_H

#define ARM_MATH_CM4

#include <s6e2cc.h>
#include "arm_math.h"
#include "delay.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "dstc.h"

typedef enum {AUDIO_CH_LEFT = 1, AUDIO_CH_RIGHT = 2, AUDIO_CH_BOTH = (AUDIO_CH_LEFT | AUDIO_CH_RIGHT)} DspAudioChannelSelect;
#define IS_LEFT_CH_SELECT(MASK)  (((MASK) & AUDIO_CH_LEFT) != 0)
#define IS_RIGHT_CH_SELECT(MASK) (((MASK) & AUDIO_CH_RIGHT) != 0)

typedef struct{
    int32_t mask;
    int32_t input_l;
    int32_t input_r;
    int32_t output_l;
    int32_t output_r;
} I2S_Data_TypeDef;

void I2S_Handler(I2S_Data_TypeDef* I2S_Data);

//Variables used to move the audio data.

static volatile int32_t audio_IN=0;     //audio data IN
static volatile int32_t audio_OUT=0;    //audio data OUT

#define NOISELEVEL 8000

//I2C write o read
#define MWSR 0x00 // Master write     
#define MRSW 0x01 // Master read 

/* The address of the codec is set by the state of the CSB pin */
#define CODEC_DEVICE_ADDRESS   0x1A    // 0011010

/* -----CODEC Registers ----- */

#define WM8731_LLINEIN              ((unsigned char)0x00)
#define WM8731_RLINEIN              ((unsigned char)0x01)
#define WM8731_LHEADOUT             ((unsigned char)0x02)
#define WM8731_RHEADOUT             ((unsigned char)0x03)
#define WM8731_ANALOG               ((unsigned char)0x04)
#define WM8731_DIGITAL              ((unsigned char)0x05)
#define WM8731_POWERDOWN            ((unsigned char)0x06)
#define WM8731_INTERFACE            ((unsigned char)0x07)
#define WM8731_SAMPLING             ((unsigned char)0x08)
#define WM8731_CONTROL              ((unsigned char)0x09)
#define WM8731_RESET                ((unsigned char)0x0F)

/*! This enum describes the different sampling frequency setup of the CODEC */
typedef enum {
    FS_8000_HZ = 0x0C,   // 8kHz from 12.288MHz MCLK
    FS_32000_HZ = 0x18,  // 32kHz from 12.288MHz MCLK
    FS_48000_HZ = 0x00,  // 48kHz from 12.288MHz MCLK
    FS_96000_HZ = 0x1C,  // 96kHz from 12.288MHz MCLK
} sampling_rate;

/*! This enum describes the different input setup of the CODEC */		
typedef enum {
    AUDIO_INPUT_LINE = 0x11,  // Select DAC input Line In and disable bypass
    AUDIO_INPUT_MIC = 0x15,   // Select DAC input Microphone In and disable bypass 
} audio_input;

/*! This enum describes the different input setup of the CODEC */		
typedef enum {
    IO_METHOD_INTR = 0x00,     // User I2S interruptions to controll the flow of the program (generate interruption when  irq_depth=<FIFO level)
    IO_METHOD_DMA = 0x01,      // Use DMA requests to controll the flow of the program (generat a request whem dma_depth=<FIFO level)
} mode;

/*! This enum describes two channels of DMA data */		
typedef enum {
    LEFT = 0,
    RIGHT = 1,
} audio_data_channel;

/*! \brief Initialize the 4 on-board LEDS */	
void init_LED(void);		
		
/*! \brief Creat a pseud-random value */			
short prbs(void);
		
/*! \brief Configures the audio part of the QSB Base Board
* Configure the freescale and the CODEC
*
* Requires: 
*
*   \param sampling_rate; hz8000, hz32000, hz48000, hz96000.
*   \param audio_input; line_in, mic_in.
		\param mode; intr, dma.
		\param handler; name of the handler.
* 
* Description: 
* Configure the I2C protocol
* Write via I2C into the CODEC registers to configure it 
* Configure the I2S protocol
* Configure the interruptions or the DSTC (DMA) to work with the I2S RX FIFO and TX FIFO
*/ 

void AudioInit(char sampling_rate, char audio_input, char mode);

	
#endif // audio_H
// *******************************ARM University Program Copyright © ARM Ltd 2015*************************************   
