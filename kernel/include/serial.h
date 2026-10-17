#ifndef GENESIS_SERIAL_H
#define GENESIS_SERIAL_H
#include <stdint.h>
/* COM1 diagnostic console. */
void genesis_serial_init(void);
void genesis_serial_write(const char *text);
void genesis_serial_write_hex64(uint64_t value);
#endif
