#include "../../moo.h"
#include "../../kill.h"

void main (void) {
    // Stop watchdog timer
    WDTCTL = WDTPW + WDTHOLD;

    unkill();
}
