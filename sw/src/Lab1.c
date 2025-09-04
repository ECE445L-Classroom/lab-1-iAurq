// ----------------------------------------------------------------------------
// Lab1.c
// Tyler Tekin
// Sept. 2, 2025
// Possible main program to test the lab
// EID % 7 = 6, Switch uses PB4, LED uses PC7
#include <stdint.h>
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"

void Clock_Delay1ms(uint32_t n);
void LED_Init(void);
void LED_Out(uint32_t data);
void Switch_Init(void);
uint32_t Switch_In(void);

int main(void){
  PLL_Init(Bus80MHz); 
  LED_Init();
  Switch_Init();
  // Write something to declare required variables
  // Write something to initalize the state of the FSM, LEDs, and variables as needed
	
	uint32_t state = 0, count = 0, led = 0;
	
	LED_Out(0);

  while(1){
      // Write something using Switch_In() and LED_Out() to implement the behavior in the lab doc
		
		uint32_t sw;
		uint32_t swFirst, swSecond;
		
		do {
			swFirst = Switch_In();
			Clock_Delay1ms(10);
			swSecond = Switch_In();
		} while (swFirst != swSecond);	// do at least once and continue until switch stablizes (both sw are equal)
		
		sw = swFirst;
		
		if (sw && !state) { // rising edge, starting with off state
			count++;
			if (count % 2 == 0) { // every second press (even)
				led ^= 0x01; // use XOR to toggle
				LED_Out(led);
			}
		}
		state = sw; // remember last state, so you know if switch was pressed-- prevents count from
								// incrementing multiple times in a press (or if holding button)-- edge trigger
  } 
} 


void LED_Init(void){
  
  // Write something to initalize the GPIOs that drive the LEDs based on your EID as defined in the lab doc.
		
	SYSCTL_RCGCGPIO_R |= 0x04; // 0b0100, port c
		while ((SYSCTL_PRGPIO_R & 0x04) == 0) {
			// do nothing and just wait here, wait until port ready
		}
	GPIO_PORTC_DIR_R |= 0x80; // 0b1000 0000, use 7th bit bc PC7, set bit = 1 to output to LED
	GPIO_PORTC_DEN_R |= 0x80; // enable the 7th bit
		
}
void LED_Out(uint32_t data){
  if (data) {
		GPIO_PORTC_DATA_R |= 0x80; // send high to PC7 (LED)
	} else {
		GPIO_PORTC_DATA_R &= ~0x80; // clear PC7 
	}
}
void Switch_Init(void){
  
  // write something to initalize the GPIO that take input from the switches based on your EID as defined in the lab doc
	
	SYSCTL_RCGCGPIO_R |= 0x02; // 0b0010, port bs
	while ((SYSCTL_PRGPIO_R & 0x02) == 0) {
		// do nothing and wait until port ready
	}
	
	GPIO_PORTB_DIR_R &= ~0x10; // 0b0001 0000, use 4th bit fro PB4-- clear bit for input
	GPIO_PORTB_DEN_R |= 0x10; // enable 4th bit
	
}
uint32_t Switch_In(void){
  // write something that reads the state of the GPIO pin as required
  
	uint32_t value = (GPIO_PORTB_DATA_R & 0x10) >> 4;
	return value;
}

void Clock_Delay(uint32_t ulCount){
  while(ulCount){
    ulCount--;
  }
}

// ------------Clock_Delay1ms------------
// Simple delay function which delays about n milliseconds.
// Inputs: n, number of msec to wait
// Outputs: none
void Clock_Delay1ms(uint32_t n){
  while(n){
    Clock_Delay(23746);  // 1 msec, tuned at 80 MHz, originally part of LCD module
    n--;
  }
}
