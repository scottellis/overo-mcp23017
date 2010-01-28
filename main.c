/*
  Demo how to access the i2c-3 bus from a userland program on an Overo.
 
  Uses a Microchip MCP23017 16-Bit I/O Expander as the example slave
  device.

  Assumptions

  The IOCON register is in the default POR state.

  At least one of the MCP devices has address 0x20, i.e. A0-A2 grounded.

  The test part is more interesting if you jumper the PortA GPIO pins 0 
  and 1 to the PortB pins 0 and 1 respectivelly. See the included 
  mcp23017_simple_circuit.png diagram.


  On every invocation the program

  1. Dumps all the registers of device 0x20

  2. Reads the IO direction registers and verifies that GPIOA.GP0 and
     GPIOA.GP1 are configured as outputs and GPIOB.GP0 and GPIOB.GP1
     are configured as inputs. If not it sets them up this way.

  3. Reads the IO polarity register and verifies that the GPIOB.GP1 is
     configured to have the input polarity reversed and if not sets 
     that up. 

  4. Reads the current value of all the GPIO pins and displays it.

  5. Toggles the value of the GPIOA.GP0 and sets GPIOA.GP1 to be the 
     same. 

  6. Reads the new current value of the GPIO pins and displays it.    
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#include "mcp23017_regs.h"

#define MCP_DEVICE_1 0x20
#define MCP_DEVICE_2 0x21


int read_reg(int fh, uint8_t reg, int count);
int write_reg(int fh, uint8_t reg, uint8_t val);
int dump_all_regs(int fh, uint8_t addr);
 
int main(int argc, char **argv)
{
	int fh, result;
	uint8_t val;

	fh = open("/dev/i2c-3", O_RDWR);

	if (fh < 0) {
		perror("open");
		exit(1);
	}

	if (dump_all_regs(fh, MCP_DEVICE_1) < 0) 
		goto DONE;

	/* You can have eight of these expanders per I2C bus. */
	if (dump_all_regs(fh, MCP_DEVICE_2) < 0)
		goto DONE; 

	/* 
	  Call this whenever you want to start talking to a different slave 
	  device. If you only have one device, only have to call this once. 
	*/ 
	if (ioctl(fh, I2C_SLAVE, MCP_DEVICE_1) < 0) {
		perror("ioctl(I2C_SLAVE)");
		goto DONE;
	}

	printf("\n\nRunning some tests on device 0x20...\n");
 
	result = read_reg(fh, IODIRA, 2);

	if (result == -1) 
		goto DONE;

	printf("IODIRA 0x%02X  IODIRB 0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	/* 
	  Make GPIOA.GP0 and GPIOA.GP1 outputs. The default state is set or configured 
	  as inputs. We want them unset.
	*/
	val = (result & 0xff);

	if ((val & (GP0 | GP1)) != 0) {
		val &= ~(GP0 | GP1);

		if (write_reg(fh, IODIRA, val) == -1) 
			goto DONE;
	}

	/* Make sure GPIOB.GP0 and GPIOB.GP1 are inputs */
	val = (result >> 8) & 0xff;
 
	if (((val & (GP0 | GP1)) != (GP0 | GP1))) {
		val |= GP0 | GP1;

		if (write_reg(fh, IODIRB, val) == -1) 
			goto DONE;
	}


	/* Check the input polarity configuration. */
	result = read_reg(fh, IPOLA, 2);

	if (result == -1)
		goto DONE;

	printf("IPOLA 0x%02X  IPOLB 0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	/* make sure IPOLB.IP0 is 0 and IPOLB.IP1 is 1 */ 
	val = (result >> 8) & 0xff;

	if (val & IP0 || (val & IP1) != IP1) {
		val &= ~IP0;
		val |= IP1;

		if (write_reg(fh, IPOLB, val) == -1)
			goto DONE;
	}


	result = read_reg(fh, GPIOA, 2);

	if (result == -1)
		goto DONE;

	printf("GPIOA 0x%02X  GPIOB 0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	/* toggle the values of GPIOA.GP0 and GPIOA.GP1, just using GP0's value to test */ 
	val = (result & 0xFF); 

	if (val & GP0) 
		val &= ~(GP0 | GP1);
	else
		val |= GP0 | GP1;	

	if (write_reg(fh, GPIOA, val) == -1)
		goto DONE;
	
	/* see if it worked */ 
	result = read_reg(fh, GPIOA, 2);

	if (result == -1)
		goto DONE;

	printf("GPIOA 0x%02X  GPIOB 0x%02X\n\n", result & 0xff, (result >> 8) & 0xff);

DONE:

	close(fh);	

	return 0;
}

/*
 To read from the MCP device, you first write the register you are interested
 in, and then do a read of x bytes. The first byte read will be the register
 you specified. Subsequent bytes read will depend on the state of the IOOCON 
 register. I am assuming the default POR state where a two byte read of a register 
 will give me both ports. 
 Whenever I call this with count = 2, I am providing the PortA register. There
 is a lot of flexibility in the device. This is just one way to read from it.
 The result is packed in the lower 16 bits of the return, PortA low, PortB high
*/ 
int read_reg(int fh, uint8_t reg, int count)
{
	uint8_t data[2];

	if (count < 0 || count > 2)
		return -1;

	data[0] = reg;

	if (write(fh, &data, 1) != 1) { 		 
		perror("write before read");
		return -1;
	}

	data[1] = 0;

	if (read(fh, &data, count) != count) {
		perror("read");
		return -1;	
	}

	return (data[1] << 8) + data[0];
}

/*
 Only doing 8 bit writes here. The device allows multi-byte writes. See the manual.
*/
int write_reg(int fh, uint8_t reg, uint8_t val)
{
	uint8_t data[2];

	data[0] = reg;
	data[1] = val;

	if (write(fh, &data, 2) != 2) { 		 
		perror("write");
		return -1;
	}

	return 1;
}

int dump_all_regs(int fh, uint8_t addr)
{
	int result;

	if (ioctl(fh, I2C_SLAVE, addr) < 0) {
		perror("ioctl(I2C_SLAVE)");
		return -1;
	}

	printf("\n=== Device 0x%02X Register Dump ===\n", addr);
 
	if ((result = read_reg(fh, IOCON, 1)) < 0) 
		return -1; 

	printf("IOCON: 0x%02X\n", result & 0xff);
	printf("  INTPOL %d\n", (result & IOCON_INTPOL) ? 1 : 0);
	printf("  ODR    %d\n", (result & IOCON_ODR) ? 1 : 0);
	printf("  HAEN   %d\n", (result & IOCON_HAEN) ? 1 : 0);
	printf("  DISSLW %d\n", (result & IOCON_DISSLW) ? 1 : 0);
	printf("  SEQOP  %d\n", (result & IOCON_SEQOP) ? 1 : 0);
	printf("  MIRROR %d\n", (result & IOCON_MIRROR) ? 1 : 0);
	printf("  BANK   %d\n", (result & IOCON_BANK) ? 1 : 0);
  

	printf("\n         PortA  PortB\n");
 
	if ((result = read_reg(fh, IODIRA, 2)) < 0) 
		return -1; 

	printf("IODIR:   0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, IPOLA, 2)) < 0) 
		return -1; 

	printf("IPOL:    0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, GPINTENA, 2)) < 0) 
		return -1; 

	printf("GPINTEN: 0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);
 
	if ((result = read_reg(fh, DEFVALA, 2)) < 0) 
		return -1; 

	printf("DEFVAL:  0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, INTCONA, 2)) < 0) 
		return -1; 

	printf("INTCON:  0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, GPPUA, 2)) < 0) 
		return -1; 

	printf("GPPU:    0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, INTFA, 2)) < 0) 
		return -1; 

	printf("INTF:    0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, INTCAPA, 2)) < 0) 
		return -1; 

	printf("INTCAP:  0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, GPIOA, 2)) < 0) 
		return -1; 

	printf("GPIO:    0x%02X   0x%02X\n", result & 0xff, (result >> 8) & 0xff);

	if ((result = read_reg(fh, OLATA, 2)) < 0) 
		return -1; 

	printf("OLAT:    0x%02X   0x%02X\n=====\n", result & 0xff, (result >> 8) & 0xff);

	return 1;
} 
