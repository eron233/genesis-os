/* Gênesis: estágio 0, Multiboot v1 -> modo longo x86_64.
 * Prova de conceito. Memória física baixa mapeada 1:1 (1 GiB).
 */
.intel_syntax noprefix
.set MB_MAGIC,    0x1BADB002
.set MB_FLAGS,    0x00000003
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS)

.section .multiboot, "a"
.align 4
.long MB_MAGIC
.long MB_FLAGS
.long MB_CHECKSUM

.section .text, "ax"
.code32
.global _start
.extern kernel_main
_start:
    cli
    cmp eax, 0x2BADB002
    jne boot_bad
    mov esp, offset stack_top

    /* Mapear 1 GiB de memória física com páginas de 2 MiB. */
    mov eax, offset pml4_table
    mov cr3, eax
    mov eax, offset pdpt_table
    or eax, 0x03
    mov dword ptr [pml4_table], eax
    mov eax, offset pd_table
    or eax, 0x03
    mov dword ptr [pdpt_table], eax
    xor ecx, ecx
map_2m:
    mov eax, ecx
    shl eax, 21
    or eax, 0x83
    mov dword ptr [pd_table + ecx*8], eax
    inc ecx
    cmp ecx, 512
    jne map_2m

    /* PAE e long mode. */
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax
    mov ecx, 0xC0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax
    lgdt [gdt_ptr]
    /* 32-bit far jump; works with GNU as and LLVM integrated assembler. */
    .byte 0xEA
    .long long_mode_start
    .word 0x08

boot_bad:
    cli
1:  hlt
    jmp 1b

.code64
long_mode_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov rsp, offset stack_top
    and rsp, -16
    call kernel_main
halt_forever:
    cli
    hlt
    jmp halt_forever

.section .rodata, "a"
.align 8
gdt:
    .quad 0
    .quad 0x00AF9A000000FFFF
    .quad 0x00CF92000000FFFF
gdt_end:
gdt_ptr:
    .word gdt_end - gdt - 1
    .long gdt

.section .bss, "aw", @nobits
.align 4096
pml4_table: .skip 4096
pdpt_table: .skip 4096
pd_table:   .skip 4096
.align 16
stack_bottom: .skip 16384
stack_top:

.section .note.GNU-stack, "", @progbits
