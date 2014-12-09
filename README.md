Lab 7/8
=====

ECE 382 Lab7/8: Robot Maze

## Prelab

**Include whatever information from this lab you think will be useful in creating your program.**

We must read the IR sensors through `P1.2`, `P1.3`, `P1.4`, and `P1.5`. The basic structure of the code will be polling each of the three sensors and then acting off of the values received by the sensors.

**Test your sensors with a DMM. Ensure they are functional. What would good reference values be?**

They are functional. Good reference values would be 0V and 5V since I don't really need much fidelity in my readings. I will be acting off of rough values rather than anything too precise.

**Consider how you'll setup the ADC10 subsystem. What are the registers you'll need to use? Which bits in those registers are important? What's the initialization sequence you'll need?**

I will initialize the subsystem as follows:

```
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_2|INCH_3|INCH_4|INCH_5;    // input A4
  ADC10AE0 |= BIT4;                           // PA.1 ADC option select
  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;         // Select SMCLK
  P1DIR |= BIT2|BIT3|BIT4|BIT5;               // Set P1.0 to output direction
```

**Consider the hardware interface. Which ADC10 channels will you use? Which pins correspond to those channels?**

I will use `A2`, `A3`, `A4`, and `A5` which correspond to `P1.2`, `P1.3`, `P1.4`, and `P1.5`, respectively.

**Consider the interface you'll create to your sensors. Will you block or use interrupts? Will you convert one sensor at a time or multiple?**

I will convert once sensor at a time. I will block one at a time and then interrupt from there. Essentially, I will be polling each sensor.


## Testing

### B Functionality

Please check repository for `sensors.h`, `sensors.c`, and `readme_sensors.md`. This consitutes a separate, usable library for the sensor functions.

### A Functionality

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
