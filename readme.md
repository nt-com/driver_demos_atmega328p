# ATmega328p Examples

#### This project aims to provide examples for using the peripherals of the ATmega328p
#### Everything is thorougly commented to provide traceability for all users 

#### You can use the examples as a foundation for your own projects

## Included Peripherals:

 * UART
 * Twin Wire Interface (TWI / I2C)
 * GPIO + external interrupts
 * 8 and 16 bit timers, with interrupts
 * Pulse-Width Modulation (PWM)

## Future Additions:

* ADC 
* SPI 

Once I get around to it...

## References

Refer to the header files of the source code for documentation and use the ATmega328p datasheet to follow along.
You can find all the code in **demo/**.


## Usage

---
#### Uses the Arduino Uno as a hardware platform, without the Arduino IDE. Coded and tested everything under Linux.

#### NOTE:
Depending on which demos you want to try, you may need some additional hardware (for example a MPU6050 sensor for the TWI demo).

---
To use the examples in this project under Linux install the arduino package, it provides all necessary tools.
You also might also need to install the C Development Tools for your distro.
You can compile the code using the provided Makefiles and use the **flash_script.sh** to flash the code to the arduino.

Example:

    * cd demo/external_interrupt
    * make -> yields ext_int_test
    * ./flash_script
    * enter "ext_int_test"
    * code should now run on the hardware

## License 

Do whatever you want with this code. Have fun with it!

## nmt @ NT-COM 
