
/*
 * MCP23S17.h
 *
 *  Created on: 2017Äê12ÔÂ23ÈÕ
 *      Author: leo
 */

#ifndef MCP23S17_H_
#define MCP23S17_H_

#define IODIRA      0x00
#define IODIRB      0x01
#define IPOLA       0x02
#define IPOLB       0x03
#define GPINTENA    0x04
#define GPINTENB    0x05
#define DEFVALA     0x06
#define DEFVALB     0x07
#define INTCONA     0x08
#define INTCONB     0x09
#define IOCONA      0x0A
#define IOCONB      0x0B
#define GPPUA       0x0C
#define GPPUB       0x0D
#define INTFA       0x0E
#define INTFB       0x0F
#define INTCAPA     0x010
#define INTCAPB     0x011
#define GPIOA       0x012
#define GPIOB       0x013
#define OLATA       0x014
#define OLATB       0x015

void InitMCP_Output( void );
void InitMCP_Input( void );
void Write23X17_Output(unsigned char reg, unsigned char data);
unsigned char Read23X17_Output(unsigned char reg);
void Write23X17_Input(unsigned char reg, unsigned char data);
unsigned char Read23X17_Input(unsigned char reg);

#endif /* MCP23S17_H_ */
