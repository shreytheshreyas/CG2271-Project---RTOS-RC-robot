/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "Led_Control.h"  	      	/** Contains code to control LED */
#include "Uart_Control.h"						/** Contains code to control UART */
#include "Motor_Control.h"					/** Contains code to control motors */
#include "Music_Control.h"					/** Contains code to control music */

#define SPEED 100

/* Global Variables */

volatile unsigned int rx_data = 0x00U;

/* Thread flags, event flags and priority definitions */

osThreadId_t motorThreadID;
osThreadId_t brainThreadID;
osThreadId_t LEDThreadID;
osThreadId_t musicThreadID;

osEventFlagsId_t instructionFlag;
osEventFlagsId_t LEDControlFlag;
osEventFlagsId_t MusicControlFlag;

const osThreadAttr_t highThreadPriority = {
	.priority = osPriorityHigh
};

const osThreadAttr_t aboveNormalThreadPriority = {
	.priority = osPriorityAboveNormal
};

/* UART2 Interrupt Handler */
void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	// For receiving data
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;
		osEventFlagsSet(instructionFlag, 0x01U);
	}
	// For error handling
	if (UART2->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		uint8_t temp = UART2->D;
	}
}

/*----------------------------------------------------------------------------
 * Application threads
 *---------------------------------------------------------------------------*/

/** Main thread which controls the other threads */
void tBrain(void *argument) {
	for (;;) {
		osEventFlagsWait(instructionFlag, 0x01U, osFlagsWaitAll, osWaitForever);
		
		/** Read rx_data and set other thread flags */
		
		// Movement command
		if (rx_data & 0b10000000) {
			osThreadFlagsSet(motorThreadID, rx_data >> 3);
			osEventFlagsSet(MusicControlFlag, 0x2U);
			
			// Movement in any direction
			if (rx_data & 0b01111000) {
				osEventFlagsSet(LEDControlFlag, 0x4U);
				osEventFlagsClear(LEDControlFlag, 0x2U);
			}
			
			// Stop movement command
			else {
				osEventFlagsSet(LEDControlFlag, 0x2U);
				osEventFlagsClear(LEDControlFlag, 0x4U);
			}
		}
		
		// Connected
		else if (rx_data & 0b00000001) {
			osEventFlagsSet(LEDControlFlag, 0x1U);
			osEventFlagsSet(MusicControlFlag, 0x1U);
		}
		
		// Finished
		else if (rx_data & 0b00000100) {
			osEventFlagsClear(MusicControlFlag, 0x3U);
			osEventFlagsSet(MusicControlFlag, 0x4U);
		}
	}
}


/** Thread to control motors */
void tMotors(void *argument) {
	for (;;) {
		unsigned int instructions = osThreadFlagsWait(0x01FU, osFlagsWaitAny, osWaitForever);

		if (instructions == 0b11000) {
			moveForward(SPEED);
		}
		
		else if (instructions == 0b10100) {
			moveBackward(SPEED);
		}
		
		else if (instructions == 0b10010) {
			turnLeft(SPEED);
		}
		
		else if (instructions == 0b10001) {
			turnRight(SPEED);
		}
		
		else if (instructions == 0b11010) {
			moveForwardLeft(SPEED);
		}
		
		else if (instructions == 0b11001) {
			moveForwardRight(SPEED);
		}
		
		else if (instructions == 0b10110) {
			moveBackwardLeft(SPEED);
		}
		
		else if (instructions == 0b10101) {
			moveBackwardRight(SPEED);
		}
		
		else if (instructions == 0b10000) {
			stop();
		}
	}
}



/** Thread to control LEDs */
void tLED (void *argument) {
	for (;;) {
		unsigned int instructions = osEventFlagsWait(LEDControlFlag, 0x7U, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
		
		// Connected
		if (instructions == 0b001) {
			flashTwiceGreen();
			osDelay(500);
			osEventFlagsClear(LEDControlFlag, 0x1U);
			osEventFlagsSet(LEDControlFlag, 0x2U);
		}
		
		// Stationary
		else if  (instructions == 0b010) {
			led_stationary_display();
		}
		
		// Moving
		else if (instructions == 0b100) {
			led_moving_display();
		}
	}
}

/** Thread to play music */
void tMusic(void *argument) {		
	int i = 0;
	int j = 0;
	for (;;) {
		unsigned int instructions = osEventFlagsWait(MusicControlFlag, 0x7U, osFlagsWaitAny | osFlagsNoClear, osWaitForever);
		
		if (instructions == 0b001) {
			playStartUpMusic();
			osDelay(500);
			osEventFlagsClear(MusicControlFlag, 0x1U);
			osEventFlagsSet(MusicControlFlag, 0x2U);
		}
		else if (instructions == 0b010) {
			i = (i+1) % PIRATES_OF_THE_CARIBBEAN_NOTE_COUNT;
			playPiratesOfTheCaribbeanMusic(i);
		}
		else if (instructions == 0b100) {
			j = (j + 1) % MARIO_NOTE_COUNT;
			playMarioMusic(j);	
		}
	}
}


int main (void) {
	// System Initialization
	SystemCoreClockUpdate();
	
	// Initialize CMSIS-RTOS
	osKernelInitialize();
	
	// Initialize thread and event Flags
	instructionFlag = osEventFlagsNew(NULL);
	LEDControlFlag = osEventFlagsNew(NULL);
	MusicControlFlag = osEventFlagsNew(NULL);

	// Initialize registers
	initGPIO();
	initUART2(BAUD_RATE);
	initMotorPWM();
	initMusicPWM();

	// Create new threads
	LEDThreadID = osThreadNew(tLED, NULL, NULL);
  brainThreadID = osThreadNew(tBrain, NULL, &highThreadPriority);
	motorThreadID = osThreadNew(tMotors, NULL, &aboveNormalThreadPriority);
	musicThreadID = osThreadNew(tMusic, NULL,NULL);

	// Start thread execution
	osKernelStart();                     
	for (;;) {}
}
