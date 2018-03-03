/* STACKT.C

    (c) Reuben Thomas 1994-2011

    Test the stack operators. Also uses the 0 and NEXT instructions.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "beetle.h"     /* main header */
#include "btests.h"	/* Beetle tests header */
#include "opcodes.h"	/* opcode enumeration */
#include "debug.h"      /* debugging functions */


const char *correct[] = {
    "1 2 3", "1 2 3 3", "1 2 3", "1 3 2", "1 3 2 3", "1 2 3 3",
    "1 3 2 3", "1 3 3 2 3", "1 3 3 3", "2 1 1", "2 1 2", "2 1 2 2", "1 2 2",
    "1 2 2 2", "2 2 1", "2 2", "2 2 1", "2 2 1 1", "2 2 1 1 1", "2 2 1 1 1 0",
    "2 2 1 1 1 0"};


int main(void)
{
    int exception = 0;

    init_beetle((CELL *)malloc(1024), 256);
    here = EP;
    S0 = SP;	/* save base of stack */

    PUSH(1); PUSH(2); PUSH(3);	/* initialise the stack */

    start_ass();
    ass(O_DUP); ass(O_DROP); ass(O_SWAP); ass(O_OVER);
    ass(O_ROT); ass(O_NROT); ass(O_TUCK); ass(O_NIP);
    ass(O_PICK); ass(O_PICK); ass(O_DUP); ass(O_ROLL);
    ass(O_DUP); ass(O_ROLL); ass(O_TOR); ass(O_RFETCH);
    ass(O_RFROM); ass(O_QDUP); ass(O_ZERO); ass(O_QDUP);
    end_ass();

    NEXT;   /* load first instruction word */

    int i;
    for (i = 0; i < 10; i++) {
        show_data_stack();
        printf("Correct stack: %s\n\n", correct[i - i / 5]);
        if (strcmp(correct[i - i / 5], val_data_stack())) {
            printf("Error in StackT: EP = %"PRIu32"\n", EP);
            exit(1);
        }
        single_step();
        printf("I = %s\n", disass(I));
    }

    SP = S0;	/* reset stack */
    PUSH(2); PUSH(1); PUSH(0);	/* initialise the stack */
    printf("Next stack is wrong!\n");

    int first = i;
    for (; i <= instrs; i++) {
        show_data_stack();
        printf("Correct stack: %s\n\n", correct[i - i / 5]);
        if (strcmp(correct[i - i / 5], val_data_stack()) && i != first) {
            printf("Error in StackT: EP = %"PRIu32"\n", EP);
            exit(1);
        }
        single_step();
        printf("I = %s\n", disass(I));
    }

    printf("StackT ran OK\n");
    return 0;
}
