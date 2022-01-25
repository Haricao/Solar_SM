#include "tpm.h"

void TPM1_IRQHandler(void);

static uint16_t tpm1Diff = 0;
static uint16_t tpm1New = 0;
static uint16_t tpm1Old = 0;
static uint8_t tpm0Enabled = 0;


void TPM1_Init_PWM(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;		// ToDo 3.1.1: Enable TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // ToDo 3.1.1: Choose MCGFLLCLK clock source

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // ToDo 3.1.2: Connect port B to clock
	//PORTB->PCR[9] = PORT_PCR_MUX(2);		// ToDo 3.1.2: Set multiplekser to TPM0 for PTB9, get channel number (page 148 of the Reference Manual)
	PORTB->PCR[13] = PORT_PCR_MUX(2);		// ToDo 3.1.2: Set multiplekser to TPM0 for PTB13, get channel number (page 148 of the Reference Manual)

	TPM1->SC |= TPM_SC_PS(5);  					// ToDo 3.1.3: Set prescaler to 128
	TPM1->SC |= TPM_SC_CMOD(1);					// ToDo 3.1.4: For TMP0, select the internal input clock source

	TPM1->MOD = 13334*2; 										// ToDo 3.1.5: Set modulo value to maximum value from slider
	
// ToDo 3.1.6: Connect correct channel from TPM0 for "Edge-aligned PWM Low-true pulses" mode
	TPM1->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); /* set Output on match, clear Output on reload */ 

	TPM1->CONTROLS[1].CnV = 1500; 				// Starting value?? 
	/* Wartosc minimalna 1317
	Wartosc maksymalna 2634*/
	tpm0Enabled = 1;  /* set local flag */
}

	uint32_t TPM1_GetVal(void) {
	return tpm1Diff;
}

void TPM1_SetVal(uint32_t value) { 				/* because slider returns '1' as minimum */
	if (tpm0Enabled) TPM1->CONTROLS[1].CnV = value;    // ToDo 3.1.8: Choose correct channel for PWM update.
}
/**
 * @brief Interrupt handler for TPM1.
 */
