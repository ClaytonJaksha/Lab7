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

	  ADC10CTL0 &=~(ENC|ADC10SC);
	  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	  ADC10CTL1 = INCH_4;
	  ADC10AE0 &= 0x0000;
	  ADC10AE0 |= BIT4;                         // PA.1 ADC option select
	  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;// Set P1.0 to output direction
	  ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	  __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	  left_reading=ADC10MEM;

	  ADC10CTL0 &=~(ENC|ADC10SC);
	  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	  ADC10CTL1 = INCH_3;                       // input A4
	  ADC10AE0 &= 0x0000;
	  ADC10AE0 |= BIT3;                         // PA.1 ADC option select
	  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;
	  ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	  __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	  right_reading=ADC10MEM;

	  ADC10CTL0 &=~(ENC|ADC10SC);
	  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	  ADC10CTL1 = INCH_5;                       // input A4
	  ADC10AE0 &= 0x0000;
	  ADC10AE0 |= BIT5;                         // PA.1 ADC option select
	  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;                // Select SMCLK
	  P1DIR |= 0x01;                            // Set P1.0 to output direction
	  ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	  __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	  cent_reading=ADC10MEM;

	  if ((left_reading<0x220)&&(right_reading>0x220)&&(cent_reading<0x220)){
		  P1OUT |= BIT0;
		  P1OUT &=~BIT6;
	  }else if ((right_reading<0x220)&&(left_reading>0x220)&&(cent_reading<0x220)){
	 	  P1OUT |= BIT6;
	 	  P1OUT &=~BIT0;// Clear P1.0 LED off
	  }else if ((right_reading<0x220)&&(left_reading<0x220)&&(cent_reading>0x220)){
		  P1OUT |= BIT0|BIT6;
  	  }else{
		  P1OUT &=~(BIT0|BIT6);
	  }
}
}
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
