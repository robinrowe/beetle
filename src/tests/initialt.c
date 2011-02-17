/* INITIALT.C

    (c) Reuben Thomas 1994-1995

    Test that the Beetle headers beetle.h and bportab.h compile properly, and
    that the storage allocation and register initialisation in storage.c
    works.

*/


#include <stdio.h>
#include <stdlib.h>
#include "beetle.h"     /* main header */
#include "btests.h"	/* Beetle tests header */


int main(void)
{
    init_beetle((BYTE *)malloc(1024), 256, 16);

    printf("InitialT ran OK\n");
    return 0;
}
