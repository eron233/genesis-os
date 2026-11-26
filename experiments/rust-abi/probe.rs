#![no_std]
#![forbid(unsafe_code)]

// Probe only. No allocation, OS calls, unwinding or kernel integration.
// Fixed-width C ABI; wrapping behavior is defined on both sides.
#[no_mangle]
pub extern "C" fn genesis_rust_abi_probe(value: u64) -> u64 {
    value.rotate_left(17) ^ 0x47454e4553495301
}

#[cfg(test)]
mod tests {
    use super::genesis_rust_abi_probe;

    #[test]
    fn fixed_vectors() {
        assert_eq!(genesis_rust_abi_probe(0), 0x47454e4553495301);
        assert_eq!(genesis_rust_abi_probe(1), 0x47454e45534b5301);
        assert_eq!(genesis_rust_abi_probe(u64::MAX), !0x47454e4553495301u64);
    }
}
