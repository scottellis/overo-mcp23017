/* 
 Registers for the MCP23017 16-Bit I/O Expander

 BANK 0 configuration, the A/B registers are paired
 BANK 1 configuration, the A/B registers are independent
*/

#ifndef MCP23017_REGS_H
#define MCP23017_REGS_H

/*
 IO Direction Register - controls the direction of the data I/O 
 R/W 
 0 = output 
 1 = input (default)
*/
#define IODIRA		0x00
#define IODIRB		0x01

#define IODIRA_BK1	0x00
#define IODIRB_BK1	0x10

#define IO0		0x01
#define IO1		0x02
#define IO2		0x04
#define IO3		0x08
#define IO4		0x10
#define IO5		0x20
#define IO6		0x40
#define IO7		0x80


/*
 IO Polarity Register - configure registers to indicate inverse logic state of input pin
 R/W 
 0 = no inversion (default) 
 1 = register is inverse polarity of input pin
*/

#define IPOLA		0x02
#define IPOLB		0x03

#define IPOLA_BK1	0x01
#define IPOLB_BK1	0x11

#define IP0		0x01
#define IP1		0x02
#define IP2		0x04
#define IP3		0x08
#define IP4		0x10
#define IP5		0x20
#define IP6		0x40
#define IP7		0x80


/*
 Interrupt on Change Control Register
 R/W
 0 = disable (default)
 1 = enable
*/

#define GPINTENA	0x04
#define GPINTENB	0x05

#define GPINTENA_BK1	0x02
#define GPINTENB_BK1	0x12

#define GPINT0		0x01
#define GPINT1		0x02
#define GPINT2		0x04
#define GPINT3		0x08
#define GPINT4		0x10
#define GPINT5		0x20
#define GPINT6		0x40
#define GPINT7		0x80


/*
 Default Compare Register for Interrupt On Change - used for the default comparison value 
 R/W
 0 = default
*/

#define DEFVALA		0x06
#define DEFVALB		0x07

#define DEFVALA_BK1	0x03
#define DEFVALB_BK1	0x13

#define DEF0		0x01
#define DEF1		0x02
#define DEF2		0x04
#define DEF3		0x08
#define DEF4		0x10
#define DEF5		0x20
#define DEF6		0x40
#define DEF7		0x80


/*
 Interrupt Control Register
 R/W
 0 = the IOx pin value is compared against previous value
 1 = the IOx pin value is compared the DEFx pin 
*/

#define INTCONA		0x08
#define INTCONB		0x09

#define INTCONA_BK1	0x04
#define INTCONB_BK1	0x14

#define IOC0		0x01
#define IOC1		0x02
#define IOC2		0x04
#define IOC3		0x08
#define IOC4		0x10
#define IOC5		0x20
#define IOC6		0x40
#define IOC7		0x80


/*
 I/O Expander Configuration Register
*/

#define IOCON		0x0A
#define IOCONA		0x0A
#define IOCONB		0x0B

#define IOCON_BK1	0x05
#define IOCONA_BK1	0x05
#define IOCONB_BK1	0x15

/* 
 Bit 0 not used, read as 0 

 Polarity of the INT output pin
 0 = low (default)
 1 = high
*/
#define IOCON_INTPOL	0x02

/*
 INT pin as an open-drain output
 0 = Active driver output (INTPOL bit sets the polarity) (default)
 1 = Open-drain output (overrides the INTPOL bit)
*/
#define IOCON_ODR	0x04

/*
 Hardware Address Enable bit, MCP23S17 only, MCP23017 HAEN always enabled
 0 = disabled (default)
 1 = enabled
*/
#define IOCON_HAEN	0x08

/*
 Slew Rate control for SDA output
 0 = enabled (default)
 1 = disabled
*/
#define IOCON_DISSLW	0x10

/*
 Sequential Operation mode bit
 0 = Sequential operation enabled, address pointer increments (default)
 1 = Sequential operation disabled, address pointer does not increment
*/
#define IOCON_SEQOP	0x20

/*
 INT Pins Mirror bit
 0 = The INT pins are not connected, INTA is associated with PortA and INTB with PortB (default)
 1 = The INT pins are internally connected
*/
#define IOCON_MIRROR	0x40

/*
 Controls how the registers are addressed
 0 = Registers are in the same bank, addresses are sequential (default)
 1 = Registers associated with each port are separated into different banks 
*/
#define IOCON_BANK	0x80


/*
 Pull-Up Resistor Configuration Register
 If a bit is set and the corresponding pin is configured as input, the corresponding
 port pin is internally pulled up with a 100K resistor
 R/W
 0 = pull-up disabled (default)
 1 = pull-up enabled
*/
#define GPPUA		0x0C
#define GPPUB		0x0D

#define GPPUA_BK1	0x06
#define GPPUB_BK1	0x16

#define PU0		0x01
#define PU1		0x02
#define PU2		0x04
#define PU3		0x08
#define PU4		0x10
#define PU5		0x20
#define PU6		0x40
#define PU7		0x80


/*
 Interrupt Flag Register - reflects the interrupt condition on the port pins
 Read-only
 0 = Interrupt not pending
 1 = Pin caused interrupt
*/

#define INTFA		0x0E
#define INTFB		0x0F

#define INTFA_BK1	0x07
#define INTFB_BK1	0x17

#define INT0		0x01
#define INT1		0x02
#define INT2		0x04
#define INT3		0x08
#define INT4		0x10
#define INT5		0x20
#define INT6		0x40
#define INT7		0x80


/*
 Interrupt Capture Register
 Capture the GPIO port value at the time the interrupt occurred. The value remains unchanged
 until the interrupt is cleared via a read of INTCAP or GPIO
 Read-only
 0 = Logic-low
 1 = Logic-high
*/

#define INTCAPA		0x10
#define INTCAPB		0x11

#define INTCAPA_BK1	0x08
#define INTCAPB_BK1	0x18

#define ICP0		0x01
#define ICP1		0x02
#define ICP2		0x04
#define ICP3		0x08
#define ICP4		0x10
#define ICP5		0x20
#define ICP6		0x40
#define ICP7		0x80


/*
 Port Register - reflects the value on the port.
 Reading from this register reads the port. Writing to this register modifies 
 the Output Latch (OLAT) register.
 R/W
 0 = Logic-low (default)
 1 = Logic-high
*/

#define GPIOA		0x12
#define GPIOB		0x13

#define GPIOA_BK1	0x09
#define GPIOB_BK1	0x19

#define GP0		0x01
#define GP1		0x02
#define GP2		0x04
#define GP3		0x08
#define GP4		0x10
#define GP5		0x20
#define GP6		0x40
#define GP7		0x80


/*
 Output Latch Register (OLAT)
 The OLAT register provides access to the output latches.
 A read from this register results in a read of the OLAT and not the port itself.
 A write to this register modifies the output latches that modifies the pins
 configured as outputs.
 R/W
 0 = Logic-low (default)
 1 = Logic-high 
*/

#define OLATA		0x14
#define OLATB		0x15

#define OLATA_BK1	0x0A
#define OLATB_BK1	0x1A

#define OL0		0x01
#define OL1		0x02
#define OL2		0x04
#define OL3		0x08
#define OL4		0x10
#define OL5		0x20
#define OL6		0x40
#define OL7		0x80


#endif /* ifndef MCP23017_REGS_H */

