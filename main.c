#include <msp430.h>
#include "MCP23S17.h"
// *** Global variables *** //
#define _CS BIT0    //Set P4.0 as Chip Select
#define WrtCmd  0
#define RdCmd   1
unsigned char gControlByte = 0x40;
unsigned char gAddrPins = 0;

void initClockTo1MHz()
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    __bis_SR_register(SCG0);                           // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_2;                               // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 29;                             // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL

}


void initSPI()
{   //SYSCFG3|=USCIB0RMP;
    P4SEL0 |= BIT5 | BIT6;
    P5SEL0 |= BIT5;

    UCB0CTL0 |= UCMST+UCSYNC+UCMSB;    //3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;                     // SMCLK
    UCB0BR0 = 0x02;                           // /2
    UCB0BR1 = 0;                              //
    UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    /*
    UCB0CTLW0 = UCSWRST;                       // **Put state machine in reset**
    UCB0CTLW0 |= UCCKPL | UCMSB | UCSYNC
                | UCMST | UCSSEL__SMCLK;      // 3-pin, 8-bit SPI Slave
    UCB0BRW = 0x20;
    //UCA0MCTLW = 0;
    UCB0CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE |= UCRXIE;
*/
}

void initSPI2()
{
    //Clock Polarity: The inactive state is high
    //MSB First, 8-bit, Master, 3-pin mode, Synchronous
    UCB0CTLW0 = UCSWRST;                       // **Put state machine in reset**
    UCB0CTLW0 |= UCCKPL | UCMSB | UCSYNC
                | UCMST | UCSSEL__SMCLK;      // 3-pin, 8-bit SPI Slave
    UCB0BRW = 0x20;
    UCA0MCTLW = 0;
    UCB0CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE |= UCRXIE;                          // Enable USCI0 RX interrupt
}

void initGPIO(){
    // *** Set up GPIO *** //
     P6DIR |= BIT0;
     P6OUT |= BIT0;

     P4DIR |= _CS;
     P4OUT |= _CS;
}


int main(void)
{
    WDTCTL = WDTPW|WDTHOLD;                   // Stop watchdog timer
    //initClockTo1MHz();

    initGPIO();
    initSPI2();

    Write23X17_Output(IODIRA, 0x00);
    Write23X17_Output(GPIOA, 0xFF);
    __delay_cycles(1000000000000);
}

void Write23X17_Output(unsigned char reg, unsigned char data)
{
    P4OUT &= ~_CS;
    char temp[2];
    temp[0] = gControlByte | WrtCmd | gAddrPins;
    spi_tx_frame(temp,1);
    temp[0] = reg;
    temp[1] = data;
    spi_tx_frame(temp, 2);
    P4OUT |=  _CS;
}

void spi_tx_frame(const unsigned char* src, unsigned char size){
    while(size){
        while (!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = *src;
        src++;
        size--;
    }
   // while ((SPI_UCSTAT & UCBUSY) == 1); // wait for transfer to complete
    UCB0IFG &= ~UCRXIFG; // clear the rx flag
}

