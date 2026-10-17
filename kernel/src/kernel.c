/* Experimental stage-0 kernel: VGA + COM1 only. */
#include <stddef.h>
#include <stdint.h>
#include "scheduler.h"
#include "serial.h"
#include "idt.h"

static volatile uint16_t *const vga = (volatile uint16_t *)(uintptr_t)0xB8000;
static size_t cursor;
static void print(const char *s, uint8_t color) {
    genesis_serial_write(s);
    while (*s) {
        if (*s == '\n') { cursor += 80 - cursor % 80; ++s; continue; }
        if (cursor >= 80u * 25u) cursor = 0;
        vga[cursor++] = ((uint16_t)color << 8) | (uint8_t)*s++;
    }
}
void kernel_main(void) {
    genesis_serial_init();
    for (size_t i = 0; i < 80u * 25u; ++i) vga[i] = 0x0720;
    print("GENESIS_BOOT_STAGE0: x86_64 long mode\n", 0x0F);
    genesis_interrupts_init();
    __asm__ volatile ("int3");
    if (genesis_breakpoint_count() != 1u) {
        print("GENESIS_IDT_CHECK: FAIL\n", 0x0C);
        for (;;) __asm__ volatile ("cli; hlt");
    }
    print("GENESIS_IDT_CHECK: OK\n", 0x0A);
    print("Kernel independente carregado (modo longo, 64 bits).\n", 0x0A);
    print("Memoria temporaria: identidade 1 GiB. Sem usuario, drivers ou disco.\n", 0x07);
    print("Proximo: IDT, memoria fisica, heap, processos e syscalls.\n", 0x07);
    genesis_scheduler_t s;
    genesis_scheduler_init(&s);
    genesis_scheduler_add(&s, 100);
    genesis_scheduler_add(&s, 200);
    if (genesis_scheduler_next(&s) == 100 && genesis_scheduler_next(&s) == 200)
        print("GENESIS_SCHEDULER_CHECK: OK\n", 0x0A);
    else print("GENESIS_SCHEDULER_CHECK: FAIL\n", 0x0C);
    print("GENESIS_KERNEL_READY: stage0 exception bootstrap\n", 0x0A);
    print("Sistema parado com seguranca (hlt).\n", 0x07);
    for (;;) __asm__ volatile ("cli; hlt");
}
