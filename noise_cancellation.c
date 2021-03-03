/**
  * loop_intr.c
  * Copyright 2016 ARM Ltd.
  */

#include "audio.h"
#include "iir_elliptic_10.h"

float32_t beta = 2e-11;
arm_lms_instance_f32 S;

#define BLOCK_SIZE 1
#define NUM_TAPS 128

float32_t FIRCoeffs [NUM_TAPS] = {0.0f};
float32_t FIRState [NUM_TAPS + BLOCK_SIZE -1];

float32_t xn, yn, signoise, error, wn, yo;

float w[NUM_SECTIONS][2] = {0.0f, 0.0f};


/**
  * I2S Data Process
  */
void ProcessData(I2S_Data_TypeDef* I2S_Data)
{
    
    int i;
    float32_t refnoise, signal;
	int section;
	

    if (IS_RIGHT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the left channel. */
		TestPinOut(TEST_PIN, HIGH);
		refnoise = (float32_t)(I2S_Data->input_r);
		signal = (float32_t)(I2S_Data->input_l);
		
		xn = refnoise;
		for(section = 0; section < NUM_SECTIONS; section++){
		wn = xn - a[section][1]*w[section][0] -a[section][2]*w[section][1];
		yn = b[section][0]*wn + b[section][1]*w[section][0]+b[section][2]*w[section][1];
		
		w[section][1] = w[section][0];
		w[section][0]=wn;
		xn = yn;
		}
		
		signoise = yn + signal;
		arm_lms_f32(&S, &refnoise, &signoise, &yo, &error, BLOCK_SIZE);
		
	
		
		
        
        I2S_Data->output_r = (int16_t)(error);
		TestPinOut(TEST_PIN, LOW);
    }

    if (IS_LEFT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the right channel. */
        
        I2S_Data->output_l = (int16_t)(signoise);
    }
}

int main(void)
{
    TestPinInit(TEST_PIN);
	arm_lms_init_f32(&S, NUM_TAPS, (float32_t *)&FIRCoeffs[0], &FIRState[0], beta, 1);
    AudioInit(FS_8000_HZ, AUDIO_INPUT_LINE, IO_METHOD_INTR);
    while(1)
        __wfi();
}
