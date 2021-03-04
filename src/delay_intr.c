/**
  * delay_intr.c
  * Copyright 2016 ARM Ltd.
  */

#include "audio.h"

#define DELAY_BUF_SIZE 24000

int16_t buffer[DELAY_BUF_SIZE];
int16_t i = 0;

/**
  * I2S Data Process
  */
void ProcessData(I2S_Data_TypeDef* I2S_Data)
{
    int16_t delayed_sample;

    TestPinToggle(TEST_PIN);

    if (IS_LEFT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the left channel. */

        delayed_sample = buffer[i];
        I2S_Data->output_l = delayed_sample + I2S_Data->input_l;
        buffer[i] = I2S_Data->input_l;
        i = (i + 1) % DELAY_BUF_SIZE;
    }

    if (IS_RIGHT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the right channel. */

        I2S_Data->output_r = I2S_Data->input_r;
    }
}

int main(void)
{
    TestPinInit(TEST_PIN);
    AudioInit(FS_48000_HZ, AUDIO_INPUT_MIC, IO_METHOD_INTR);
    while(1)
        __wfi();
}
