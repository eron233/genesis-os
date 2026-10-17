#include <stdint.h>
#include "idt.h"
#include "serial.h"

#define GENESIS_EXCEPTION_COUNT 32u
#define GENESIS_KERNEL_CS 0x08u
#define GENESIS_INTERRUPT_GATE 0x8Eu

struct __attribute__((packed)) descriptor_table_ptr {
    uint16_t limit;
    uint64_t base;
};
extern void (*const genesis_exception_stubs[GENESIS_EXCEPTION_COUNT])(void);
static genesis_idt_gate_t idt[GENESIS_EXCEPTION_COUNT] __attribute__((aligned(16)));
static volatile uint64_t breakpoints;

void genesis_interrupts_init(void) {
    for (unsigned i = 0; i < GENESIS_EXCEPTION_COUNT; ++i)
        genesis_idt_make_gate(&idt[i], (uintptr_t)genesis_exception_stubs[i],
                              GENESIS_KERNEL_CS, GENESIS_INTERRUPT_GATE);
    const struct descriptor_table_ptr ptr = {
        .limit = (uint16_t)(sizeof(idt) - 1u), .base = (uint64_t)(uintptr_t)idt
    };
    __asm__ volatile ("lidt %0" : : "m"(ptr) : "memory");
}
uint64_t genesis_breakpoint_count(void) { return breakpoints; }
void genesis_interrupt_dispatch(const genesis_trap_frame_t *frame) {
    if (frame->vector == 3u) {
        ++breakpoints;
        return;
    }
    genesis_serial_write("GENESIS_EXCEPTION: vector=");
    genesis_serial_write_hex64(frame->vector);
    genesis_serial_write(" error=");
    genesis_serial_write_hex64(frame->error_code);
    genesis_serial_write(" rip=");
    genesis_serial_write_hex64(frame->rip);
    genesis_serial_write("\nGENESIS_PANIC: halted\n");
    for (;;) __asm__ volatile ("cli; hlt");
}
