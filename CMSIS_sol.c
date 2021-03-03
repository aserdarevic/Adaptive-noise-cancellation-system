/**
  * loop_intr.c
  * Copyright 2016 ARM Ltd.
  */

#include "audio.h"
#include "iir_elliptic_10.h"
float32_t pCoeffs[5*NUM_SECTIONS] = {0.0f};
float32_t state[4*NUM_SECTIONS] = {0.0f};


float32_t beta = 2e-11;
arm_biquad_casd_df1_inst_f32 IIRfilter;
arm_lms_instance_f32 AFIR;

#define BLOCK_SIZE 1
#define NUM_TAPS 128

float32_t FIRCoeffs [NUM_TAPS] = {0.0f};
float32_t FIRState [NUM_TAPS + BLOCK_SIZE -1];

float32_t xn, yn, signoise, error, yo;




/**
  * I2S Data Process
  */
void ProcessData(I2S_Data_TypeDef* I2S_Data)
{
    
    int i;
    float32_t refnoise, signal;
	
	

    if (IS_RIGHT_CH_SELECT(I2S_Data->mask))
    {
        /* TODO: Process data of the left channel. */
		TestPinOut(TEST_PIN, HIGH);
		refnoise = (float32_t)(I2S_Data->input_r);
		signal = (float32_t)(I2S_Data->input_l);
		
		xn = refnoise;
		arm_biquad_cascade_df1_f32(&IIRfilter, &refnoise, &yn,1);
		
		signoise = yn + signal;
		arm_lms_f32(&AFIR, &refnoise, &signoise, &yo, &error, BLOCK_SIZE);
		
	
		
		
        
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
	int i, j = 0;
	
	for( i = 0; i < NUM_SECTIONS; i++){
	pCoeffs[j++] = b[i][0];
	pCoeffs[j++] = b[i][1];
	pCoeffs[j++] = b[i][2];
	pCoeffs[j++] = -a[i][1];
	pCoeffs[j++] = -a[i][2];
	};
	
    TestPinInit(TEST_PIN);
	arm_biquad_cascade_df1_init_f32(&IIRfilter, NUM_SECTIONS, (float32_t *)&pCoeffs,(float32_t *)&state);
	arm_lms_init_f32(&AFIR, NUM_TAPS, (float32_t *)&FIRCoeffs[0], &FIRState[0], beta, 1);
    AudioInit(FS_8000_HZ, AUDIO_INPUT_LINE, IO_METHOD_INTR);
    while(1)
        __wfi();
}
