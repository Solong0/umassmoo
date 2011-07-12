/* See license.txt for license information. */

#include "kill.h"
#include "flash.h"

unsigned char kill (void) {
<<<<<<< HEAD
    while (!is_killed())
    {
      extflash_write_uchar((unsigned long)KILLSWITCH_ADDR, 1);
    }
    return 0;
=======
    while (! is_killed())
        extflash_write_uchar((unsigned long)KILLSWITCH_ADDR, 1);
    return 1;
}

unsigned char unkill (void) {
    extflash_erase_segment((unsigned long)KILLSWITCH_ADDR);
    return 1;
>>>>>>> f8698b121946ff0ca299fea3b342bb7df606531f
}

unsigned char is_killed (void) {
  return (extflash_read_uchar(KILLSWITCH_ADDR) != 0);
}
