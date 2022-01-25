
					
#include "MKL05Z4.h"
#include "ADC.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "tpm.h"
#include "tpm0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float volt_coeff = ((float)(((float)2.91) / 4095) );			// Współczynnik korekcji wyniku, w stosunku do napięcia referencyjnego przetwornika
uint8_t wynik_ok=0;
uint8_t zmienne [4] = {0, 2, 3 ,7};
uint8_t n = 0;
uint16_t temp;
float wynik [4];
static uint8_t msTicks = 0;
long int  fluent_tpm0 = 1000;
long int  fluent_tpm1 = 1500;



void SysTick_Handler(void);
void lcd_static(void);
void lcd_update(void);

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];		// Odczyt danej i skasowanie flagi COCO
	if(!wynik_ok)					// Sprawdź, czy wynik skonsumowany przez petlę główną
	{
		wynik[n] = temp;				// Wyślij wynik do pętli głównej
		
		wynik_ok=1;
		if (n<3)
	{
		n = n + 1;
	}
	else { n = 0; }
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(zmienne[n]);
	}
	NVIC_ClearPendingIRQ(ADC0_IRQn);
}
int main (void)
{
	uint8_t	kal_error;
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

	LCD1602_Init();		 // Inicjalizacja wyświetlacza LCD
	LCD1602_Backlight(TRUE);
	LCD1602_SetCursor(0,1);
	
	SysTick_Config(1000000); 				
	/* initialize system timer */
	TPM1_Init_PWM();
	TPM0_Init_PWM();
	
	
	kal_error=ADC_Init();		// Inicjalizacja przetwornika A/C

	if(kal_error)
	{
		while(1);							// Klaibracja się nie powiodła
	}
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(zmienne[n]); 		// Pierwsze wyzwolenie przetwornika ADC0 i odblokowanie przerwania
	
	while(1)
	{			
		if(wynik_ok)
		{
			sprintf(display,"%.3fV",wynik[3]*volt_coeff);
			LCD1602_SetCursor(0,0);
			LCD1602_Print(display);
			sprintf(display,"%.3fV",wynik[0]*volt_coeff);
			LCD1602_SetCursor(10,0);
			LCD1602_Print(display);
			sprintf(display,"%.3fV",wynik[2]*volt_coeff);
			LCD1602_SetCursor(0,1);
			LCD1602_Print(display);
			sprintf(display,"%.3fV",wynik[1]*volt_coeff);
			LCD1602_SetCursor(10,1);
			LCD1602_Print(display);
			 
			/*if(wynik[3]*volt_coeff&&wynik[2]*volt_coeff>1.9)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 < 2200; fluent_tpm0++)
				{
					DELAY(0.1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(wynik[0]*volt_coeff>0.5 && wynik[1]*volt_coeff>0.9)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 > 1150; fluent_tpm1--)
						{
							DELAY(0.1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}
			
			else
				{
					TPM0_SetVal(1000);
					TPM1_SetVal(1500);
				}*/
			if(wynik[3]*volt_coeff>2.2&&wynik[2]*volt_coeff>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 < 2000; fluent_tpm0++)
				{
					DELAY(0.1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(wynik[0]*volt_coeff<1||wynik[1]*volt_coeff<1)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 > 1300; fluent_tpm1--)
						{
							DELAY(0.1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}
			else if(wynik[0]*volt_coeff>2.2&&wynik[1]*volt_coeff>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 < 2000; fluent_tpm0++)
				{
					DELAY(0.1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(wynik[3]*volt_coeff<1||wynik[2]*volt_coeff<1)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 < 1800; fluent_tpm1++)
						{
							DELAY(0.1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}
			else if(wynik[1]*volt_coeff>2.2&&wynik[2]*volt_coeff>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 > 999; fluent_tpm0--)
				{
					DELAY(0.1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(wynik[3]*volt_coeff<1||wynik[0]*volt_coeff<1)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 < 1800; fluent_tpm1++)
						{
							DELAY(0.1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}
			else if(wynik[3]*volt_coeff>2.2&&wynik[0]*volt_coeff>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 > 999; --fluent_tpm0)
				{
					DELAY(0.1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(wynik[1]*volt_coeff<1||wynik[2]*volt_coeff<1)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 > 1300; fluent_tpm1--)
						{
							DELAY(0.1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}
			
			else
				{
					TPM0_SetVal(fluent_tpm0);
					TPM1_SetVal(fluent_tpm1);
				}
			
			
			wynik_ok=0;
		}
	}
}

void SysTick_Handler(void) {
	msTicks++;
	//newTick = 1;
}
