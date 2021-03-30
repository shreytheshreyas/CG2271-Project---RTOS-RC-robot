#include "MKL25Z4.h"                    // Device header

#define PTD0_W1_F_Pin              0 // Back right
#define PTD1_W1_R_Pin              1

#define PTD2_W2_F_Pin							 3 //Back Left
#define PTD3_W2_R_Pin							 2

#define PTC8_W3_F_Pin							 8 // Front Right
#define PTC9_W3_R_Pin							 9

#define PTA1_W4_F_Pin							 1 // Front Left
#define PTA2_W4_R_Pin							 2 

#define MOD_VALUE									 1000

void initMotorPWM(void) {
	
	// Enable clock to PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTD_MASK) | (SIM_SCGC5_PORTA_MASK) | (SIM_SCGC5_PORTC_MASK));
	
	// Clear MUX settings for wheel 1 pins, and set them to PWM (alt 4)
	PORTD->PCR[PTD0_W1_F_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD0_W1_F_Pin] |= PORT_PCR_MUX(4);
	PORTD->PCR[PTD1_W1_R_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD1_W1_R_Pin] |= PORT_PCR_MUX(4);
	
	// Clear MUX settings for wheel 2 pins, and set them to PWM (alt 4)
	PORTD->PCR[PTD2_W2_F_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD2_W2_F_Pin] |= PORT_PCR_MUX(4);
	PORTD->PCR[PTD3_W2_R_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD3_W2_R_Pin] |= PORT_PCR_MUX(4);
	
	// Clear MUX settings for wheel 3 pins, and set them to PWM (alt 3)
	PORTC->PCR[PTC8_W3_F_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[PTC8_W3_F_Pin] |= PORT_PCR_MUX(3);
	PORTC->PCR[PTC9_W3_R_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[PTC9_W3_R_Pin] |= PORT_PCR_MUX(3);
	
	// Clear MUX settings for wheel 4 pins, and set them to PWM (alt 3)
	PORTA->PCR[PTA1_W4_F_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA1_W4_F_Pin] |= PORT_PCR_MUX(3);
	PORTA->PCR[PTA2_W4_R_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA2_W4_R_Pin] |= PORT_PCR_MUX(3);
	
	
	// Enable clock to TPM0 and TPM2 Module
	SIM->SCGC6 |= ((SIM_SCGC6_TPM0_MASK) | (SIM_SCGC6_TPM2_MASK));
	
	// Clear TPM clock source, and select MCGFLLCLK clock
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// After prescaling: Clock frequency is 375000 Hz
	// Corresponds to 375Hz frequency
	TPM0->MOD = MOD_VALUE;
	TPM0_C0V = 0;
	TPM0_C1V = 0;
	TPM0_C2V = 0;
	TPM0_C3V = 0;
	TPM0_C4V = 0;
	TPM0_C5V = 0;
	
	TPM2->MOD = MOD_VALUE;
	TPM2_C0V = 0;
	TPM2_C1V = 0;
	
	// Clear clock mode selection and prescale factor selection
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	
	// Select LPTPM counter to increment on every LPTPM counter clock
	// Set prescale factor to divide by 128
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	
	// LPTPM counter operates in up counting mode
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	// Clear MSB, MSA, ELSB, ELSA in TPM0 C0SC register 
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Clear MSB, MSA, ELSB, ELSA in TPM0 C1SC register
	TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Clear MSB, MSA, ELSB, ELSA in TPM0 C2SC register
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Clear MSB, MSA, ELSB, ELSA in TPM0 C3SC register
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

	// Clear MSB, MSA, ELSB, ELSA in TPM0 C4SC register
	TPM0_C4SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Clear MSB, MSA, ELSB, ELSA in TPM0 C5SC register
	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Clear MSB, MSA, ELSB, ELSA in TPM2 C0SC register
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Clear MSB, MSA, ELSB, ELSA in TPM2 C1SC register
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//Set to edge-aligned PWM with High-true pulses
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}


void moveBackward(uint32_t percent) {
	TPM0_C0V = percent * (MOD_VALUE / 100); // Back right
	TPM0_C1V = 0;	
	TPM0_C2V = percent * (MOD_VALUE / 100); // Back left
	TPM0_C3V = 0;
	TPM0_C4V = percent * (MOD_VALUE / 100); // Front right
	TPM0_C5V = 0;
	TPM2_C0V = percent * (MOD_VALUE / 100); // Front left
	TPM2_C1V = 0;
}

void moveForward(uint32_t percent) {
	TPM0_C0V = 0;                           // Back right
	TPM0_C1V = percent * (MOD_VALUE / 100);	
	TPM0_C2V = 0;                           // Back left
	TPM0_C3V = percent * (MOD_VALUE / 100);	
	TPM0_C4V = 0;                           // Front right
	TPM0_C5V = percent * (MOD_VALUE / 100);
	TPM2_C0V = 0;                           // Front left
	TPM2_C1V = percent * (MOD_VALUE / 100);
}

void turnRight(uint32_t percent) {
	TPM0_C0V = percent * (MOD_VALUE / 100); // Back right
	TPM0_C1V = 0;	
	TPM0_C2V = 0;                           // Back left
	TPM0_C3V = percent * (MOD_VALUE / 100);
	TPM0_C4V = percent * (MOD_VALUE / 100); // Front right
	TPM0_C5V = 0;                          
	TPM2_C0V = 0;                           // Front left
	TPM2_C1V = percent * (MOD_VALUE / 100);
}

void turnLeft(uint32_t percent) {
	TPM0_C0V = 0;                           // Back right
	TPM0_C1V = percent * (MOD_VALUE / 100);
	TPM0_C2V = percent * (MOD_VALUE / 100); // Back left
	TPM0_C3V = 0;	
	TPM0_C4V = 0;                           // Front right
	TPM0_C5V = percent * (MOD_VALUE / 100);
	TPM2_C0V = percent * (MOD_VALUE / 100); // Front left
	TPM2_C1V = 0;
}

void moveForwardLeft(uint32_t percent) {
	TPM0_C0V = 0;                           // Back right
	TPM0_C1V = percent * (MOD_VALUE / 100);	
	TPM0_C2V = 0;                           // Back left
	TPM0_C3V = 0;
	TPM0_C4V = 0;                           // Front right
	TPM0_C5V = percent  * (MOD_VALUE / 100);
	TPM2_C0V = 0;                           // Front left
	TPM2_C1V = 0;
}

void moveForwardRight(uint32_t percent) {
	TPM0_C0V = 0;                           // Back right
	TPM0_C1V = 0;
	TPM0_C2V = 0;                           // Back left
	TPM0_C3V = percent * (MOD_VALUE / 100);	
	TPM0_C4V = 0;                           // Front right
	TPM0_C5V = 0;
	TPM2_C0V = 0;                           // Front left
	TPM2_C1V = percent * (MOD_VALUE / 100);
}

void moveBackwardLeft(uint32_t percent) {
	TPM0_C0V = percent * (MOD_VALUE / 100); // Back right
	TPM0_C1V = 0;	
	TPM0_C2V = 0;                           // Back left
	TPM0_C3V = 0;
	TPM0_C4V = percent * (MOD_VALUE / 100); // Front right
	TPM0_C5V = 0;
	TPM2_C0V = 0;                           // Front left
	TPM2_C1V = 0;
}

void moveBackwardRight(uint32_t percent) {
	TPM0_C0V = 0;                           // Back right
	TPM0_C1V = 0;	
	TPM0_C2V = percent * (MOD_VALUE / 100); // Back left
	TPM0_C3V = 0;
	TPM0_C4V = 0;                           // Front right
	TPM0_C5V = 0;
	TPM2_C0V = percent * (MOD_VALUE / 100); // Front left
	TPM2_C1V = 0;
}

void stop() {
  TPM0_C0V = 0;
	TPM0_C1V = 0;	
	TPM0_C2V = 0;
	TPM0_C3V = 0;
	TPM0_C4V = 0;
	TPM0_C5V = 0;
	TPM2_C0V = 0;
	TPM2_C1V = 0;
}
	



