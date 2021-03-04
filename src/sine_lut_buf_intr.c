/**
  * sine_lut_buf_intr.c
  * Copyright 2016 ARM Ltd.
  */

#include "audio.h"

#define LOOP_SIZE 8
int16_t sine_table[LOOP_SIZE] = {0, 7071, 10000, 7071, 0, -7071, -10000, -7071};
unsigned int sine_ptr = 0;

#define BUFFER_SIZE 100
float32_t buffer[BUFFER_SIZE];
unsigned int buf_ptr = 0;

/**
  * I2S Data Process
  */
void ProcessData(I2S_Data_TypeDef* I2S_Data)
{
    TestPinToggle(TEST_PIN);

    if (IS_LEFT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the left channel. */
        
        I2S_Data->output_l = sine_table[sine_ptr];
        buffer[buf_ptr] = I2S_Data->output_l;
        sine_ptr = (sine_ptr + 1) % LOOP_SIZE;
        buf_ptr = (buf_ptr + 1) % BUFFER_SIZE;
    }

    if (IS_RIGHT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the right channel. */
        
        I2S_Data->output_r = 0;
    }
}

int main(void)
{
    TestPinInit(TEST_PIN);
    AudioInit(FS_8000_HZ, AUDIO_INPUT_MIC, IO_METHOD_INTR);
    while(1)
        __wfi();
}
