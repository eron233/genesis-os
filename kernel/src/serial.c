#include <stdint.h>
#include "serial.h"
#define COM1 0x3F8u
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
void genesis_serial_init(void) {
    outb(COM1 + 1u, 0u);
    outb(COM1 + 3u, 0x80u);
    outb(COM1 + 0u, 3u);
    outb(COM1 + 1u, 0u);
    outb(COM1 + 3u, 0x03u);
    outb(COM1 + 2u, 0xC7u);
    outb(COM1 + 4u, 0x0Bu);
}
static void genesis_serial_putc(char c) {
    unsigned int spins = 100000u;
    while ((inb(COM1 + 5u) & 0x20u) == 0u && --spins != 0u) {}
    if (spins != 0u) outb(COM1, (uint8_t)c);
}
void genesis_serial_write(const char *text) {
    for (; *text != '\0'; ++text) {
        if (*text == '\n') genesis_serial_putc('\r');
        genesis_serial_putc(*text);
    }
}
void genesis_serial_write_hex64(uint64_t value) {
    static const char digits[] = "0123456789abcdef";
    genesis_serial_write("0x");
    for (int nibble = 15; nibble >= 0; --nibble) {
        const uint8_t digit = (uint8_t)((value >> (unsigned)(nibble * 4)) & 15u);
        genesis_serial_putc(digits[digit]);
    }
}
