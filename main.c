/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 * + MKL25Z4
 * + Component libraries
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "MKL25Z4.h"
#include "motor.h"
#include "myLED.h"
#include "serialComm.h"

#include "onboardLED.h"	// FOR TESTING RTOS SYNC

/*----------------------------------------------------------------------------
 * Global sync
 *---------------------------------------------------------------------------*/

static volatile int isMoving = 0;

osMessageQueueId_t rxDataQ, motorMsg, ledMsg, audioMsg;
 
 /*----------------------------------------------------------------------------
 * Serial communications
 * 1. UART2 IRQ handler
 * 2. Application brain thread
 * 		Decode the data from the Serial Port and perform the necessary action
 *---------------------------------------------------------------------------*/
  
 void UART2_IRQHandler(void) {
	// Clear pending IRQs
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	// RX ready
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		uint8_t rx_data = UART2->D;
		osMessageQueuePut(rxDataQ, &rx_data, NULL, 0);
	}
	
	// Error
	if (UART2->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		// Clear error flag by reading UART_D
		uint8_t error_data = UART2->D;
	}
	
}
 
void brain_thread (void *argument) {

  for (;;) {
		// Get data from rxDataQ
		uint8_t rx_data;
		osMessageQueueGet(rxDataQ, &rx_data, NULL, osWaitForever);
	
		// Process data
		uint8_t motorData, ledData, audioData;
		switch (rx_data) {
			case 0x00:	// STOP - white
				osMessageQueuePut(motorMs
				break;
			case 0x10:	// FRONT - red

				break;
			case 0x20:	// BACK - green

				break;
			case 0x30:	// LEFT - blue

				break;
			case 0x40:	// RIGHT - yellow

				break;
			default:
				off_led();
		}
		
		// Send messages
		osMessageQueuePut(motorMsg, &motorData, NULL, 0);
//		osMessageQueuePut(ledMsg, &ledData, NULL, 0);
//		osMessageQueuePut(audioMsg, &audioData, NULL, 0);
	}
}
 
/*----------------------------------------------------------------------------
 * Application motor_control thread
 * Control the action of the motors
 *---------------------------------------------------------------------------*/
void motor_control_thread (void *argument) {
	
	// Read data, then execute appropriate command
 

	
	}
}
 
 /*----------------------------------------------------------------------------
 * Application led thread
 * Control the LEDs
 *---------------------------------------------------------------------------*/
void led_front_thread(void *argument) {

	
	uint8_t ledIndex = 0;

}

void led_rear_thread(void *argument) {

	uint8_t rearled_data = 0;
	
}
 
/*----------------------------------------------------------------------------
 * Application audio thread
 * Provide audio output
 *---------------------------------------------------------------------------*/
void audio_thread (void *argument) {
 
  // ...
  for (;;) {}
}

/*----------------------------------------------------------------------------
 * MAIN
 *---------------------------------------------------------------------------*/

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	
	// Initialise components
	initMotor();
	initLedPins();
	initUART2();
	initGPIO(); // FOR TESTING RTOS SYNC
	
	// Start multi-threaded environment 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
		
	// Message queues
	rxDataQ = osMessageQueueNew(10, sizeof(uint8_t), NULL);
	motorMsg = osMessageQueueNew(10, sizeof(uint8_t), NULL);
	ledMsg = osMessageQueueNew(10, sizeof(uint8_t), NULL);
	audioMsg = osMessageQueueNew(10, sizeof(uint8_t), NULL);
	
	osThreadNew(brain_thread, NULL, NULL);			// Brain
  osThreadNew(motor_control_thread, NULL, NULL);    // Motors
//  osThreadNew(led_front_thread, NULL, NULL); // Create led_front_thread
//  osThreadNew(led_rear_thread, NULL, NULL);  // Create led_rear_thread
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
