/**
  * loop_intr.c
  * Copyright 2016 ARM Ltd.
  */

#include "audio.h"

/**
  * I2S Data Process
  */
void ProcessData(I2S_Data_TypeDef* I2S_Data)
{
    TestPinToggle(TEST_PIN);

    if (IS_LEFT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the left channel. */
        
        I2S_Data->output_l = I2S_Data->input_l;
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
