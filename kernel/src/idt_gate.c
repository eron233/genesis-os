#include "idt.h"

_Static_assert(sizeof(genesis_idt_gate_t) == 16u, "x86_64 IDT gate must be 16 bytes");
_Static_assert(offsetof(genesis_idt_gate_t, reserved) == 12u, "IDT layout mismatch");

void genesis_idt_make_gate(genesis_idt_gate_t *gate, uintptr_t address,
                           uint16_t selector, uint8_t attributes) {
    gate->offset_low = (uint16_t)address;
    gate->selector = selector;
    gate->ist = 0; /* A separate IST and TSS are required for production. */
    gate->attributes = attributes;
    gate->offset_middle = (uint16_t)(address >> 16);
    gate->offset_high = (uint32_t)(address >> 32);
    gate->reserved = 0;
}
