#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "idt.h"
int main(void) {
    genesis_idt_gate_t gate = {0};
    const uintptr_t addr = (uintptr_t)UINT64_C(0xFEDCBA9876543210);
    genesis_idt_make_gate(&gate, addr, 0x08u, 0x8Eu);
    assert(sizeof(gate) == 16u);
    assert(gate.offset_low == 0x3210u);
    assert(gate.offset_middle == 0x7654u);
    assert(gate.offset_high == 0xFEDCBA98u);
    assert(gate.selector == 8u && gate.attributes == 0x8Eu);
    assert(gate.ist == 0u && gate.reserved == 0u);
    puts("IDT: gate encoding OK");
}
