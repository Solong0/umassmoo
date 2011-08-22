/* See license.txt for license information. */

#include "kill.h"
#include "flash.h"
#include "moo.h"

unsigned char kill (void) {
    extflash_erase_segment((unsigned long)KILLSWITCH_ADDR);

    while (! is_killed())
        extflash_write_uchar((unsigned long)KILLSWITCH_ADDR, 1);
    return 1;
}

unsigned char unkill (void) {
    extflash_erase_segment((unsigned long)KILLSWITCH_ADDR);
    return 1;
}

unsigned char is_killed (void) {
  return (extflash_read_uchar(KILLSWITCH_ADDR) != 0);
}

void hard_reset (void) {
    // per http://tech.groups.yahoo.com/group/msp430/message/49331
    WDTCTL = 0;
}
