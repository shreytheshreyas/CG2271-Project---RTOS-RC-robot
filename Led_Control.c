#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#define RED_LED_ALL 11 // PortC Pin 11
#define GREEN_LED1 5 // PortE Pin 5
#define GREEN_LED2 4 // PortE Pin 4
#define GREEN_LED3 3 // PortE Pin 3
#define GREEN_LED4 2 // PortE Pin 2
#define GREEN_LED5 11 // PortB Pin 11
#define GREEN_LED6 10 // PortB Pin 10
#define GREEN_LED7 9 // PortB Pin 9
#define GREEN_LED8 8 // PortB Pin 8
#define MASK(x) (1 << (x))

/** Turn on Green LED strip */
void turnOnGreen(void) {
	PTE->PDOR = (MASK(GREEN_LED1) | MASK(GREEN_LED2) | MASK(GREEN_LED3) |	MASK(GREEN_LED4));
	PTB->PDOR = (MASK(GREEN_LED5) | MASK(GREEN_LED6) | MASK(GREEN_LED7) |	MASK(GREEN_LED8));
}

/** Turns off the Green LED strip */
void turnOffGreen() {
	PTE->PCOR = (MASK(GREEN_LED1) | MASK(GREEN_LED2) | MASK(GREEN_LED3) |	MASK(GREEN_LED4));
	PTB->PCOR = (MASK(GREEN_LED5) | MASK(GREEN_LED6) | MASK(GREEN_LED7) |	MASK(GREEN_LED8));
}

/**	When BT connection established, flashes Green LED twice*/
void flashTwiceGreen(void){
	turnOnGreen();
	osDelay(500);	
	turnOffGreen();
	osDelay(500);	
	turnOnGreen();
	osDelay(500);	
	turnOffGreen();
	osDelay(500);				
}


/** Turn on Red LED strip */
void turnOnRed(void) {
	PTC->PDOR = MASK(RED_LED_ALL);
}

/** Turn off RED LED strip */
void turnOffRed() {
	PTC->PCOR |= MASK(RED_LED_ALL);
}

/** Toggle the Red LED strip on or off*/
void toggleRed() {
	PTC->PTOR |= MASK(RED_LED_ALL);
}

/** Turns on Green LED in running sequence based on count value*/
void runGreen(int count) {
	switch (count) {
		case 0: 
			PTE->PDOR = MASK(GREEN_LED1);
			PTB->PCOR = MASK(GREEN_LED8);
			break;
		case 1: 
			PTE->PDOR = MASK(GREEN_LED2);
			break;
		case 2: 
			PTE->PDOR = MASK(GREEN_LED3);
			break;
		case 3: 
			PTE->PDOR = MASK(GREEN_LED4);
			break;
		case 4: 
			PTE->PCOR = MASK(GREEN_LED4);
			PTB->PDOR = MASK(GREEN_LED5);
			break;
		case 5: 
			PTB->PDOR = MASK(GREEN_LED6);
			break;
		case 6: 
			PTB->PDOR = MASK(GREEN_LED7);
			break;
		case 7: 
			PTB->PDOR = MASK(GREEN_LED8);
			break;
	}
}

/** When robot moving, Green LED in a running mode 
	and Red LED flashing at 500ms on 500ms off.*/
void led_moving_display(void) {
	turnOffGreen();
	static int count = 0;
	toggleRed();
	runGreen(count);
	osDelay(500);
	count++;
	count %= 8;
}

/**	When robot is stationary, Green LEDs all light up
	and Red LED flashing at 250ms on 250ms off.*/
void led_stationary_display(void){
	toggleRed();
	turnOnGreen();
	osDelay(250);				
}


/** Configures the gpio ports used for the LED's */
void initGPIO(void) {
		// Enable Clock to PORTC and PORTE and PORTB 
		SIM->SCGC5 |= ((SIM_SCGC5_PORTC_MASK) | (SIM_SCGC5_PORTE_MASK) | (SIM_SCGC5_PORTB_MASK));
	
		// Configure MUX settings to make all 3 pins GPIO
		PORTC->PCR[RED_LED_ALL] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[RED_LED_ALL] |= PORT_PCR_MUX(1);
		
		PORTE->PCR[GREEN_LED1] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[GREEN_LED1] |= PORT_PCR_MUX(1);
		PORTE->PCR[GREEN_LED2] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[GREEN_LED2] |= PORT_PCR_MUX(1);
		PORTE->PCR[GREEN_LED3] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[GREEN_LED3] |= PORT_PCR_MUX(1);
		PORTE->PCR[GREEN_LED4] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[GREEN_LED4] |= PORT_PCR_MUX(1);
		PORTB->PCR[GREEN_LED5] &= ~PORT_PCR_MUX_MASK;
		PORTB->PCR[GREEN_LED5] |= PORT_PCR_MUX(1);
		PORTB->PCR[GREEN_LED6] &= ~PORT_PCR_MUX_MASK;
		PORTB->PCR[GREEN_LED6] |= PORT_PCR_MUX(1);
		PORTB->PCR[GREEN_LED7] &= ~PORT_PCR_MUX_MASK;
		PORTB->PCR[GREEN_LED7] |= PORT_PCR_MUX(1);
		PORTB->PCR[GREEN_LED8] &= ~PORT_PCR_MUX_MASK;
		PORTB->PCR[GREEN_LED8] |= PORT_PCR_MUX(1);

		
		// Set Data Direction Registers for PortB and PortD and PortC
		PTE->PDDR |= (MASK(GREEN_LED1) | MASK(GREEN_LED2) | MASK(GREEN_LED3) | MASK(GREEN_LED4) );
		PTB->PDDR |= (MASK(GREEN_LED5) | MASK(GREEN_LED6) | MASK(GREEN_LED7) | MASK(GREEN_LED8) );
		PTC->PDDR |= MASK(RED_LED_ALL);
		turnOffGreen();
		turnOffRed();
}
