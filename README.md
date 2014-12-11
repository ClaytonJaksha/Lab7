Lab 7
====
#### Clayton Jaksha | ECE 382 | Dr. York | M2A
## Objective and Purpose
### Objective

The objective of this lab is to interpret the analog readings from our infrared sensors by making lights turn on and off when in varying proximity to the robot.

### Purpose

To practice analog-to-digital implementation in a real-world application.

## Preliminary Design

**Include whatever information from this lab you think will be useful in creating your program.**

We must read the IR sensors through `P1.2`, `P1.3`, and `P1.4`. The basic structure of the code will be polling each of the three sensors and then acting off of the values received by the sensors.

**Test your sensors with a DMM. Ensure they are functional. What would good reference values be?**

They are functional. Good reference values would be 0V and 5V since I don't really need much fidelity in my readings. I will be acting off of rough values rather than anything too precise.

**Consider how you'll setup the ADC10 subsystem. What are the registers you'll need to use? Which bits in those registers are important? What's the initialization sequence you'll need?**

I will initialize the subsystem as follows:

```
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_3			      // input A3
  ADC10AE0 |= BIT3;                           // PA.1 ADC option select
  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;         // Select SMCLK
  P1DIR |= BIT3|BIT4|BIT;       	      // Set P1.0 to output direction
```
I will repeat this process for `A4` and `A5`.

**Consider the hardware interface. Which ADC10 channels will you use? Which pins correspond to those channels?**

I will use `A3`, `A4`, and `A5` which correspond to `P1.2`, `P1.3`, and `P1.4`, respectively.

**Consider the interface you'll create to your sensors. Will you block or use interrupts? Will you convert one sensor at a time or multiple?**

I will convert once sensor at a time. I will block one at a time and then interrupt from there. Essentially, I will be polling each sensor.

#### Hardware Schematic
This is how the sensors hook up to the MSP430
![alt text](http://i.imgur.com/77EEUgY.jpg "dat hardware")
## Code Walkthrough
### Basic Functionality
###### Taken from `main_basic.c`
```
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

	  left_reading=poll_left();
	  right_reading=poll_right();
	  cent_reading=poll_cent();
	  if (DETECT_RIGHT&&CLEAR_CENT&&CLEAR_LEFT){
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
```
###### Taken from `sensors.c`
```
unsigned int poll_right(void){
	unsigned int right_val=0;
	ADC10CTL0 &=~(ENC|ADC10SC);
    ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	ADC10CTL1 = INCH_3;                       // input A4
	ADC10AE0 &= 0x0000;
	ADC10AE0 |= BIT3;                         // PA.1 ADC option select
	ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;
	ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	right_val=ADC10MEM;
	if (right_val>RIGHT_THRESH){
		P1OUT|=BIT0;
	} else{
		P1OUT &=~BIT0;
	}
	return right_val;
}
```
```
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
```

## Debugging

Debugging was primarily done by looking at the Nokia 1202 display and the stored register and expression values within CCS-6.0's debugging feature. By pausing the program during its runtime, I could peer into the values that the MSP430 was seeing from my inputs. The primary issue I faced was that the values would change constantly coming out of the IR sensors. To solve this I used a low-pass filter on my analog outputs from the sensors. This could also be accomplished by a moving-average filter in software, but I preferred the analog solution.

## Testing
#### Testing Methodology
To test the funcionality of my program, I attached the hardware IAW the diagram in my deisgn section, loaded the program, ran the program, and pressed buttons on my remote (or let the robot run). If it did what I wanted, then it passed the test.
#### Results
The code works!
##### Basic Functionality
Robert turned on the green LED (`P1.6`) when the right sensor reached its defined threshold voltage and turn it off when it was below the threshold voltage. Similar functionality occurred for the left sensor (red LED `P1.0`) and the center sensor (both LEDs).
##### B Functionality

Please check repository for `sensors.h`, `sensors.c`, and `readme_sensors.md`. This consitutes a separate, usable library for the sensor functions.

##### A Functionality

![alt text](http://i.imgur.com/YPeizUW.png "GO ARMY, BEAT NAVY")

|   Center Sensor   |                                  |                                  |    Right Sensor   |                                  |                                  | Left Sensor |                                  |                                  |
|:-----------------:|:--------------------------------:|:--------------------------------:|:-----------------:|:--------------------------------:|:--------------------------------:|:----------------------:|:--------------------------------:|:--------------------------------:|
| Displacement [in] | Register <br></br> Reading [hex] | Register <br></br> Reading [dec] | Displacement [in] | Register <br></br> Reading [hex] | Register <br></br> Reading [dec] |    Displacement [in]   | Register <br></br> Reading [hex] | Register <br></br> Reading [dec] |
|        inf        |                14D               |                333               |        inf        |                1AD               |                429               |           inf          |                181               |                385               |
|         12        |                17A               |                378               |         12        |                251               |                593               |           12           |                22F               |                559               |
|         11        |                180               |                384               |         11        |                26D               |                621               |           11           |                226               |                550               |
|         10        |                195               |                405               |         10        |                284               |                644               |           10           |                26A               |                618               |
|         9         |                190               |                400               |         9         |                286               |                646               |            9           |                2A9               |                681               |
|         8         |                1A2               |                418               |         8         |                295               |                661               |            8           |                2B6               |                694               |
|         7         |                1C0               |                448               |         7         |                2A6               |                678               |            7           |                308               |                776               |
|         6         |                1CC               |                460               |         6         |                2CF               |                719               |            6           |                334               |                820               |
|         5         |                20F               |                527               |         5         |                2E4               |                740               |            5           |                36D               |                877               |
|         4         |                29A               |                666               |         4         |                31A               |                794               |            4           |                38F               |                911               |
|         3         |                33A               |                826               |         3         |                336               |                822               |            3           |                39E               |                926               |
|         2         |                3C4               |                964               |         2         |                367               |                871               |            2           |                3B0               |                944               |
|         1         |                3E0               |                992               |         1         |                39E               |                926               |            1           |                3B5               |                949               |
|         0         |                3FF               |               1023               |         0         |                38F               |                911               |            0           |                397               |                919               |







## Observations and Conclusion
#### Observations

* Reusing previous .c and .h files can make coding faster and modular.
* Hardware can be tricky, read datasheets!

#### Conclusion

Robert the robot can move in a controlled, exact manner by himself or with commands from a user. Be afraid, be very afraid.

## Documentation

None


# SONS OF SLUM AND GRAVY TAKE NO QUARTER
![alt text](http://farm7.static.flickr.com/6120/6300471005_c44f5ef019.jpg "GO ARMY, BEAT NAVY")
