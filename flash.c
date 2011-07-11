/* See license.txt for license information. */

#include "moo.h"
#include "flash.h"

void extflash_init (void) {
    // set up EXTFLASH_PORT for use with external flash
    EXTFLASH_PORT_OUT |= EXTFLASH_CHIP_ENABLE_PIN;
    EXTFLASH_PORT_DIR |= EXTFLASH_CHIP_ENABLE_PIN;
    EXTFLASH_PORT_SEL |= EXTFLASH_SPI_DATA_OUT_PIN
        | EXTFLASH_SPI_DATA_IN_PIN
        | EXTFLASH_SPI_SERIAL_CLOCK_OUT_PIN;

    // generic SPI initialization; refer to TI MSP430F2618 sample code
    UCB1CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    //3-pin, 8-bit SPI master
    UCB1CTL1 |= UCSSEL_2;                     // SMCLK
    UCB1BR0 = 0x02;                           // /2
    UCB1BR1 = 0;                              //
    UCB1CTL1 &= ~UCSWRST;                     // Initialize USCI state machine
    UC1IE |= UCB1RXIE;                        // Enable USCI_B1 RX interrupt

    _BIS_SR(GIE); // enable interrupts because SPI uses interrupts for RX notify
}

void extflash_enable_chip (void) {
    // lower the CHIP ENABLE pin to enable extflash chip
    EXTFLASH_PORT_OUT &= ~EXTFLASH_CHIP_ENABLE_PIN;
}

void extflash_disable_chip (void) {
    // raise the CHIP ENABLE pin to disable extflash chip
    EXTFLASH_PORT_OUT |= EXTFLASH_CHIP_ENABLE_PIN;
}

void extflash_send_byte (unsigned char c) {
    UCB1TXBUF = c; // place c into transmit buffer
    while (!(UC1IFG & UCB1TXIFG)); // busy-wait until flag indicates TX is done
}

unsigned char extflash_read_uchar (unsigned long addr) {
    extflash_enable_chip();
    extflash_send_byte(EXTFLASH_BYTE_READ_COMMAND);

    // send three bytes of address, from upper to lower
    extflash_send_byte((addr & 0xFF0000) >> 16);
    extflash_send_byte((addr & 0xFF00) >> 8);
    extflash_send_byte(addr & 0xFF);

    // now wait for RX via interrupt XXX

    extflash_disable_chip();

    return 0; // XXX
}

unsigned char extflash_write_uchar (unsigned long addr, unsigned char c) {
    extflash_enable_chip();
    extflash_send_byte(EXTFLASH_WRITE_ENABLE_COMMAND);

    extflash_send_byte(EXTFLASH_BYTE_PROGRAM_COMMAND);

    // send three bytes of address, from upper to lower
    extflash_send_byte((addr & 0xFF0000) >> 16);
    extflash_send_byte((addr & 0xFF00) >> 8);
    extflash_send_byte(addr & 0xFF);

    // send the character to write
    extflash_send_byte(c);

    extflash_disable_chip();
    return 1; // XXX
}
