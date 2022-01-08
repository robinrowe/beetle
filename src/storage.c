// Allocate storage for the registers and memory.
//
// (c) Reuben Thomas 1994-2020
//
// The package is distributed under the GNU Public License version 3, or,
// at your option, any later version.
//
// THIS PROGRAM IS PROVIDED AS IS, WITH NO WARRANTY. USE IS AT THE USER’S
// RISK.

#include "config.h"

#include "external_syms.h"

#include <stdlib.h>
#include <string.h>

#include "beetle.h"
#include "beetle_aux.h"
#include "private.h"


// VM registers

beetle_Registers beetle_registers;
beetle_CELL *M0;

// General memory access

// Return native address of a range of VM memory, or NULL if invalid
_GL_ATTRIBUTE_PURE uint8_t *native_address_of_range(UCELL start, UCELL length)
{
    if (start > R(MEMORY) || R(MEMORY) - start < length)
        return NULL;

    return ((uint8_t *)M0) + start;
}

// Macro for byte addressing
#ifdef WORDS_BIGENDIAN
#define FLIP(addr) ((addr) ^ (CELL_W - 1))
#else
#define FLIP(addr) (addr)
#endif

int load_cell(UCELL addr, CELL *value)
{
    if (!IS_ALIGNED(addr)) {
        R(NOT_ADDRESS) = addr;
        return -23;
    }

    uint8_t *ptr = native_address_of_range(addr, CELL_W);
    if (ptr == NULL) {
        R(NOT_ADDRESS) = addr;
        return -9;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    *value = *(CELL *)ptr;
#pragma GCC diagnostic pop
    return 0;
}

int load_byte(UCELL addr, BYTE *value)
{
    uint8_t *ptr = native_address_of_range(FLIP(addr), 1);
    if (ptr == NULL) {
        R(NOT_ADDRESS) = addr;
        return -9;
    }
    *value = *ptr;
    return 0;
}

int store_cell(UCELL addr, CELL value)
{
    if (!IS_ALIGNED(addr)) {
        R(NOT_ADDRESS) = addr;
        return -23;
    }

    uint8_t *ptr = native_address_of_range(addr, CELL_W);
    if (ptr == NULL) {
        R(NOT_ADDRESS) = addr;
        return -9;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    *(CELL *)ptr = value;
#pragma GCC diagnostic pop
    return 0;
}

int store_byte(UCELL addr, BYTE value)
{
    uint8_t *ptr = native_address_of_range(FLIP(addr), 1);
    if (ptr == NULL) {
        R(NOT_ADDRESS) = addr;
        return -9;
    }
    *ptr = value;
    return 0;
}

_GL_ATTRIBUTE_CONST CELL reverse_cell(CELL value)
{
    CELL res = 0;
    for (unsigned i = 0; i < CELL_W / 2; i++)
    {
        unsigned lopos = CHAR_BIT * i;
        unsigned hipos = CHAR_BIT * (CELL_W - 1 - i);
        unsigned move = hipos - lopos;
        res |= ((((UCELL)value) & ((UCELL)CHAR_MASK << hipos)) >> move) | ((((UCELL)value) & ((UCELL)CHAR_MASK << lopos)) << move);
    }
    return res;
}

int reverse(UCELL start, UCELL length)
{
    int ret = 0;
    for (UCELL i = 0; ret == 0 && i < length; i++)
    {
        CELL c;
        ret = load_cell(start + i * CELL_W, &c) || store_cell(start + i, reverse_cell(c));
    }
    return ret;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=pure"
int pre_dma(UCELL from, UCELL to)
{
    int exception = 0;

    // Expand range to words
    from &= -CELL_W;
    to = ALIGN(to);

    if (to < from || native_address_of_range(from, to - from) == NULL)
        exception = -1;
    if (exception == 0 && ENDISM)
        exception = reverse(from, to - from);
    return exception;
}
#pragma GCC diagnostic pop

int post_dma(UCELL from, UCELL to)
{
    return pre_dma(from, to);
}

// Initialise registers that are not fixed

int init(size_t size)
{
    R(CHECKED) = 1; // address checking is mandatory in this implementation
    R(MEMORY) = size * CELL_W;
    M0 = (beetle_CELL *)calloc(size, CELL_W);
    if (M0 == NULL)
        return -1;

    R(EP) = 0;
    R(A) = 0;
    R(S0) = R(SP) = R(MEMORY) - 0x100;
    R(R0) = R(RP) = R(MEMORY);
    R(THROW) = 0;
    R(BAD) = CELL_MAX;
    R(NOT_ADDRESS) = CELL_MAX;

    return 0;
}

void destroy(void)
{
    free(M0);
#ifdef HAVE_MIJIT
    mijit_beetle_drop(beetle_jit);
#endif
}
