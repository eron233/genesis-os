#include <assert.h>
#include <stdio.h>
#include "scheduler.h"
int main(void) {
    genesis_scheduler_t s;
    genesis_scheduler_init(&s);
    assert(genesis_scheduler_next(&s) == 0);
    assert(!genesis_scheduler_add(&s, 0));
    assert(genesis_scheduler_add(&s, 1));
    assert(genesis_scheduler_add(&s, 2));
    assert(genesis_scheduler_add(&s, 3));
    assert(!genesis_scheduler_add(&s, 2));
    assert(genesis_scheduler_next(&s) == 1);
    assert(genesis_scheduler_next(&s) == 2);
    assert(genesis_scheduler_next(&s) == 3);
    assert(genesis_scheduler_next(&s) == 1);
    assert(genesis_scheduler_remove(&s, 2));
    assert(genesis_scheduler_next(&s) == 3);
    assert(genesis_scheduler_next(&s) == 1);
    assert(!genesis_scheduler_remove(&s, 2));
    assert(genesis_scheduler_remove(&s, 1));
    assert(genesis_scheduler_remove(&s, 3));
    assert(genesis_scheduler_next(&s) == 0);
    for (unsigned i = 1; i <= GENESIS_MAX_TASKS; ++i) assert(genesis_scheduler_add(&s, i));
    assert(!genesis_scheduler_add(&s, 1000));
    puts("Scheduler: testes unitarios OK");
}
