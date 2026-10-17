#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "serial.h"
#include "idt.h"
int main(void) {
    assert(offsetof(genesis_trap_frame_t, r15) == 0u);
    assert(offsetof(genesis_trap_frame_t, rax) == 14u * 8u);
    assert(offsetof(genesis_trap_frame_t, vector) == 15u * 8u);
    assert(offsetof(genesis_trap_frame_t, error_code) == 16u * 8u);
    assert(offsetof(genesis_trap_frame_t, rip) == 17u * 8u);
    assert(sizeof(genesis_trap_frame_t) == 20u * 8u);
    puts("Trap frame: 64-bit layout OK");
}
