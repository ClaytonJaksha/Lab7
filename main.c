//-----------------------------------------------------------------
// Name:	Jaksha
// File:	main.c
// Date:	Fall 2014
// Purp:	Read from IR sensors and then activate LEDs accordingly
//-----------------------------------------------------------------

#include <msp430.h> 

unsigned int left_reading=0;
unsigned int right_reading=0;
unsigned int cent_reading=0;

void main(void)
{

  WDTCTL = WDTPW + WDTHOLD;
  // Select SMCLK
  P1DIR |= BIT0|BIT6;

  while(1){

	  left_reading=poll_left();		//read IR sensors
	  right_reading=poll_right();
	  cent_reading=poll_cent();
	  if (DETECT_RIGHT&&CLEAR_CENT&&CLEAR_LEFT){		//activate appropriate LEDs
		  P1OUT |= BIT0;
		  P1OUT &=~BIT6;
	  }else if (DETECT_LEFT&&CLEAR_RIGHT&&CLEAR_CENT){
	 	  P1OUT |= BIT6;
	 	  P1OUT &=~BIT0;// Clear P1.0 LED off
	  }else if (DETECT_CENT){
		  P1OUT |= BIT0|BIT6;
  	  }else{
		  P1OUT &=~(BIT0|BIT6);
	  }
}
}
