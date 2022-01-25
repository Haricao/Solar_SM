#include "tpm0.h"

void TPM0_IRQHandler(void);

static uint16_t tpm1Diff = 0;
static uint16_t tpm1New = 0;
static uint16_t tpm1Old = 0;
static uint8_t tpm0Enabled = 0;


void TPM0_Init_PWM(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		// ToDo 3.1.1: Enable TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // ToDo 3.1.1: Choose MCGFLLCLK clock source

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // ToDo 3.1.2: Connect port B to clock
	PORTB->PCR[11] = PORT_PCR_MUX(2);		// ToDo 3.1.2: Set multiplekser to TPM0 for PTB9, get channel number (page 148 of the Reference Manual)
	

	TPM0->SC |= TPM_SC_PS(5);  					// ToDo 3.1.3: Set prescaler to 128
	TPM0->SC |= TPM_SC_CMOD(1);					// ToDo 3.1.4: For TMP0, select the internal input clock source

	TPM0->MOD = 13334*2; 										
	
// ToDo 3.1.6: Connect correct channel from TPM0 for "Edge-aligned PWM Low-true pulses" mode
	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
	TPM0->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); /* set Output on match, clear Output on reload */ 

	TPM0->CONTROLS[0].CnV = 1000; 				// Starting value?? 
	/* Wartosc minimalna 1317
	Wartosc maksymalna 2634*/
	tpm0Enabled = 1;  /* set local flag */
}

	uint32_t TPM0_GetVal(void) {
	return tpm1Diff;
}

void TPM0_SetVal(uint32_t value) {
	value = value; 				/* because slider returns '1' as minimum */
	if (tpm0Enabled) TPM0->CONTROLS[0].CnV = value;    // ToDo 3.1.8: Choose correct channel for PWM update.
}
/**
 * @brief Interrupt handler for TPM1.
 */
