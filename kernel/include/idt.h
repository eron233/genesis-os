#ifndef GENESIS_IDT_H
#define GENESIS_IDT_H
#include <stddef.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} genesis_idt_gate_t;

/* x86-64 exception frame saved by interrupt_stubs.S. CPL0 only. */
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t vector, error_code, rip, cs, rflags;
} genesis_trap_frame_t;

void genesis_idt_make_gate(genesis_idt_gate_t *gate, uintptr_t address,
                           uint16_t selector, uint8_t attributes);
void genesis_interrupts_init(void);
uint64_t genesis_breakpoint_count(void);
void genesis_interrupt_dispatch(const genesis_trap_frame_t *frame);

#endif
