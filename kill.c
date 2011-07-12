/* See license.txt for license information. */

#include "kill.h"
#include "flash.h"

unsigned char kill (void) {
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
