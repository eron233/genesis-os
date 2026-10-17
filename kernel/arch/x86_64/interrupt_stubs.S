/* Exceptions 0..31: vector, error code, RIP, CS, RFLAGS.
 * CPL0 only. IST, IRQ routing and userspace are NOT supported.
 */
.intel_syntax noprefix
.section .text, "ax"
.code64
.extern genesis_interrupt_dispatch

.macro GENESIS_NOERROR n
.global genesis_isr_\n
genesis_isr_\n:
    push 0
    push \n
    jmp genesis_isr_common
.endm
.macro GENESIS_ERROR n
.global genesis_isr_\n
genesis_isr_\n:
    push \n
    jmp genesis_isr_common
.endm

GENESIS_NOERROR 0
GENESIS_NOERROR 1
GENESIS_NOERROR 2
GENESIS_NOERROR 3
GENESIS_NOERROR 4
GENESIS_NOERROR 5
GENESIS_NOERROR 6
GENESIS_NOERROR 7
GENESIS_ERROR 8
GENESIS_NOERROR 9
GENESIS_ERROR 10
GENESIS_ERROR 11
GENESIS_ERROR 12
GENESIS_ERROR 13
GENESIS_ERROR 14
GENESIS_NOERROR 15
GENESIS_NOERROR 16
GENESIS_ERROR 17
GENESIS_NOERROR 18
GENESIS_NOERROR 19
GENESIS_NOERROR 20
GENESIS_ERROR 21
GENESIS_NOERROR 22
GENESIS_NOERROR 23
GENESIS_NOERROR 24
GENESIS_NOERROR 25
GENESIS_NOERROR 26
GENESIS_NOERROR 27
GENESIS_NOERROR 28
GENESIS_ERROR 29
GENESIS_ERROR 30
GENESIS_NOERROR 31

genesis_isr_common:
    cld
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    mov rdi, rsp
    mov r12, rsp
    and rsp, -16
    call genesis_interrupt_dispatch
    mov rsp, r12
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    add rsp, 16
    iretq

.section .rodata, "a"
.align 8
.global genesis_exception_stubs
genesis_exception_stubs:
.macro GENESIS_TABLE_ENTRY n
    .quad genesis_isr_\n
.endm
.irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
GENESIS_TABLE_ENTRY \index
.endr
.section .note.GNU-stack, "", @progbits
