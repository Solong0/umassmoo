/* See license.txt for license information. */

#ifndef RPC_H
#define RPC_H

#define RPC_DISPATCH_TABLE_SIZE 3

/* Function identifiers for RPC commands.  Each one-byte RPC_*_ID constant
 * corresponds to a byte value the reader sends via the cmd[] array.
 * XXX HACK: at the moment, Andres Molina provides these byte values based on
 * his experiments with receiving cmd[] bytes over a lossy channel. */
#define RPC_BEEP_ID 0xD0
#define RPC_KILL_ID 0xC1
#define RPC_TEMPERATURE_ID 0xDD

extern unsigned char rpc_cmd;
extern unsigned char rpc_param;
extern unsigned char rpc_in_progress;
extern unsigned int rpc_retval;

typedef struct {
    unsigned char id;
    unsigned int (*fn)(void);
} rpc_dispatch_table_entry;

extern rpc_dispatch_table_entry rpc_dispatch_table[RPC_DISPATCH_TABLE_SIZE];

void rpc_init (void);
void rpc_dispatch (void);

unsigned int rpc_beep (void);
unsigned int rpc_kill (void);
unsigned int rpc_get_temperature (void);

#endif // RPC_H
