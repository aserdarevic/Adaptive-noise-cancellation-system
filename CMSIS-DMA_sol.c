/**
  * noise_cancellation_dma.c
  * Copyright 2016 ARM Ltd.
  */

#include "audio.h"
#include "iir_elliptic_10.h"

//parametri IIR filta
float32_t pCoeffs[5*NUM_SECTIONS] = {0.0f};
float32_t state[4*NUM_SECTIONS] = {0.0f};


float32_t beta = 2e-11;
arm_biquad_casd_df1_inst_f32 IIRfilter;
arm_lms_instance_f32 AFIR;

#define BLOCK_SIZE 1
#define NUM_TAPS 128

//parametri adaptivnog FIR filtera
float32_t FIRCoeffs [NUM_TAPS] = {0.0f};
float32_t FIRState [NUM_TAPS + BLOCK_SIZE -1];

//pomocne varijable
float32_t xn, yn, signoise, error, yo;

/* DMA buffer status */
extern int16_t (*tx_proc_buffer)[2];
extern int16_t (*rx_proc_buffer)[2];
extern uint32_t tx_buffer_empty,
                rx_buffer_full;

/**
  * Data process function.
  * Called by main() at every end of DMA transfers.
  */
void ProcessBuffer(void) 
{
    int ii;
    float32_t refnoise, signal;
	int section;

    for(ii = 0; ii < DMA_BUFFER_SIZE; ii++)
    {		
		refnoise = (float32_t)(rx_proc_buffer[ii][RIGHT]);
        signal = (float32_t)(rx_proc_buffer[ii][LEFT]);
		
        xn = refnoise;
       arm_biquad_cascade_df1_f32(&IIRfilter, &refnoise, &yn,1);
		
		signoise = yn + signal;
		arm_lms_f32(&AFIR, &refnoise, &signoise, &yo, &error, BLOCK_SIZE);
		tx_proc_buffer[ii][LEFT] = (int16_t)(signoise);
        tx_proc_buffer[ii][RIGHT] = (int16_t)(error);
 	} 
    
    tx_buffer_empty = 0;
    rx_buffer_full = 0;
}

/**
  * Main Function
  */
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
    AudioInit(FS_8000_HZ, AUDIO_INPUT_LINE, IO_METHOD_DMA);

    while (1)
    {
        while (!(rx_buffer_full && tx_buffer_empty)) {};
        TestPinOut(TEST_PIN, HIGH);
        ProcessBuffer();
        TestPinOut(TEST_PIN, LOW);
    }
}
