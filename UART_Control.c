#include "MKL25Z4.h"
#include "cmsis_os2.h"

#define UART_RX_PORTD4 4
#define UART_TX_PORTD5 5
#define UART2_INT_PRIO 128

void initUART2(uint32_t baud_rate) {
	
	uint32_t divisor, bus_clock;
	
	// Enable clock gating to UART2 and PORTE
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Put pins in UART TX and RX mode
	PORTD->PCR[UART_RX_PORTD4] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[UART_RX_PORTD4] |= PORT_PCR_MUX(3);
	
	PORTD->PCR[UART_TX_PORTD5] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[UART_TX_PORTD5] |= PORT_PCR_MUX(3);
	
	// Turn off TX and RX for UART2 first
	// To be re enabled later
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	// Set baud rate prescaler appropriately
	// Factor of 16 to account for oversampling
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	// Clear other registers
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	//Initialize interrupts
	NVIC_SetPriority(UART2_IRQn, 128);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	// Enable UART TX and RX
	UART2->C2 |= ((UART_C2_RE_MASK) | (UART_C2_RIE_MASK));

}
