/* See license.txt for license information. */

#include "moo.h"
#include "rpc.h"
#include "kill.h"

unsigned char rpc_cmd = 0;
unsigned char rpc_param = 0;
unsigned char rpc_in_progress = 0;
unsigned int rpc_retval = 0;
rpc_dispatch_table_entry rpc_dispatch_table[RPC_DISPATCH_TABLE_SIZE];

void rpc_init (void) {
    rpc_dispatch_table[0].id = RPC_BEEP_ID;
    rpc_dispatch_table[0].fn = &rpc_beep;

    rpc_dispatch_table[1].id = RPC_KILL_ID;
    rpc_dispatch_table[1].fn = &rpc_kill;

    rpc_dispatch_table[2].id = RPC_UNKILL_ID;
    rpc_dispatch_table[2].fn = &rpc_unkill;

    rpc_dispatch_table[3].id = RPC_TEMPERATURE_ID;
    rpc_dispatch_table[3].fn = &rpc_get_temperature;
}

void rpc_dispatch (void) {
    // look up rpc_cmd in the dispatch table
    unsigned i;

    for (i = 0; i < RPC_DISPATCH_TABLE_SIZE; ++i) {
        if (rpc_dispatch_table[i].id == rpc_cmd) {
            break;
        }
    }
    // if not in dispatch table, reset RPC globals to 0; do nothing
    if (i == RPC_DISPATCH_TABLE_SIZE) {
        rpc_cmd = rpc_param = rpc_in_progress = rpc_retval = 0;
        return;
    }

    // execute the function named by rpc_cmd; stuff its return value into
    // rpc_retval
    rpc_retval = (*(rpc_dispatch_table[i].fn))();
}

unsigned int rpc_beep (void) {
#define COUNTERLEN 500 // ~440 Hz
#define NUMCYCLES 250
#define P36 0x40 // 0b01000000, pin 3.6
    unsigned i, cyc;

    P3DIR |= P36; // P3.6 is an output pin; all others are inputs

    for (cyc = 0; cyc != NUMCYCLES; ++cyc) {
        P3OUT ^= P36; // toggle P3.6
        i = COUNTERLEN;
        do { --i; } while (i != 0);
    }

    return 1;
}

unsigned int rpc_kill (void) {
    return kill();
};

unsigned int rpc_unkill (void) {
    return unkill();
}

unsigned int rpc_get_temperature (void) {
    // XXX copied from (sufficiently tested?) tests/sensorTest.c

    // setup ADC for external temperature sensor
    ADC12CTL0 &= ~ENC; // make sure this is off otherwise settings are locked.
    P6SEL |= INCH_TEMP_EXT_IN;                      // Enable A/D channel A4
    ADC12CTL0 = ADC12ON + SHT0_2 + REFON + REF2_5V; // Turn on and set up ADC12
    ADC12CTL1 = SHP;                                // Use sampling timer
    ADC12MCTL0 = INCH_TEMP_EXT_IN + SREF_1;         // Vr+=Vref+

    ADC12CTL0 |= ENC;     // Enable conversion
    ADC12CTL0 |= ADC12SC; // Start conversion
    while (ADC12CTL1 & ADC12BUSY); // busy-wait for ADC to sample

    return ADC12MEM0;
}
