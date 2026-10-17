#ifndef GENESIS_SCHEDULER_H
#define GENESIS_SCHEDULER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* ID round-robin demonstration ONLY; does not switch CPU context. */
#define GENESIS_MAX_TASKS 64u
typedef struct {
    uint64_t tasks[GENESIS_MAX_TASKS];
    size_t count;
    size_t cursor;
} genesis_scheduler_t;
void genesis_scheduler_init(genesis_scheduler_t *s);
bool genesis_scheduler_add(genesis_scheduler_t *s, uint64_t task_id);
bool genesis_scheduler_remove(genesis_scheduler_t *s, uint64_t task_id);
uint64_t genesis_scheduler_next(genesis_scheduler_t *s);
#endif
