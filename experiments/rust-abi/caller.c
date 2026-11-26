#include <stdint.h>

/* Only an object-link probe. Not called from the current kernel. */
extern uint64_t genesis_rust_abi_probe(uint64_t value);

uint64_t genesis_c_abi_probe(uint64_t value) {
    return genesis_rust_abi_probe(value);
}
