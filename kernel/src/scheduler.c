#include "scheduler.h"
void genesis_scheduler_init(genesis_scheduler_t *s) {
    s->count = 0;
    s->cursor = 0;
}
bool genesis_scheduler_add(genesis_scheduler_t *s, uint64_t id) {
    if (!id || s->count >= GENESIS_MAX_TASKS) return false;
    for (size_t i = 0; i < s->count; i++)
        if (s->tasks[i] == id) return false;
    s->tasks[s->count++] = id;
    return true;
}
bool genesis_scheduler_remove(genesis_scheduler_t *s, uint64_t id) {
    for (size_t i = 0; i < s->count; i++) {
        if (s->tasks[i] != id) continue;
        for (size_t j = i + 1; j < s->count; ++j)
            s->tasks[j - 1] = s->tasks[j];
        --s->count;
        if (s->count == 0) s->cursor = 0;
        else if (i < s->cursor) --s->cursor;
        else if (s->cursor >= s->count) s->cursor = 0;
        return true;
    }
    return false;
}
uint64_t genesis_scheduler_next(genesis_scheduler_t *s) {
    if (!s->count) return 0;
    const uint64_t id = s->tasks[s->cursor];
    s->cursor = (s->cursor + 1) % s->count;
    return id;
}
