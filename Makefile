CC ?= gcc
LD ?= ld
CFLAGS = -m64 -mcmodel=large -mno-red-zone -mgeneral-regs-only -fno-pie -fno-pic -fno-stack-protector -fno-builtin -fno-asynchronous-unwind-tables -ffreestanding -Wall -Wextra -Werror -O2 -Ikernel/include
BUILD = build
OBJS = $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/scheduler.o $(BUILD)/serial.o $(BUILD)/interrupts.o $(BUILD)/idt_gate.o $(BUILD)/interrupt_stubs.o

.PHONY: all check test run smoke clean
all: $(BUILD)/genesis.elf
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/boot.o: boot/x86_64/boot.S | $(BUILD)
	$(CC) -m64 -c -o $@ $<
$(BUILD)/kernel.o: kernel/src/kernel.c kernel/include/scheduler.h kernel/include/idt.h kernel/include/serial.h | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILD)/scheduler.o: kernel/src/scheduler.c kernel/include/scheduler.h | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILD)/serial.o: kernel/src/serial.c kernel/include/serial.h | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILD)/interrupt_stubs.o: kernel/arch/x86_64/interrupt_stubs.S | $(BUILD)
	$(CC) -m64 -c -o $@ $<
$(BUILD)/interrupts.o: kernel/src/interrupts.c kernel/include/idt.h kernel/include/serial.h | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILD)/idt_gate.o: kernel/src/idt_gate.c kernel/include/idt.h | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILD)/genesis.elf: $(OBJS) linker.ld
	$(LD) -m elf_x86_64 -z max-page-size=0x1000 -T linker.ld -o $@ $(OBJS)
check: all test
	python3 tools/check_multiboot.py $(BUILD)/genesis.elf
	python3 -m unittest discover -s tests -p "test_*.py" -v
test: | $(BUILD)
	$(CC) -Wall -Wextra -Werror -std=c11 -Ikernel/include tests/test_scheduler.c kernel/src/scheduler.c -o $(BUILD)/test_scheduler
	./$(BUILD)/test_scheduler
	$(CC) -Wall -Wextra -Werror -std=c11 -Ikernel/include tests/test_idt_gate.c kernel/src/idt_gate.c -o $(BUILD)/test_idt_gate
	./$(BUILD)/test_idt_gate
	$(CC) -Wall -Wextra -Werror -std=c11 -Ikernel/include tests/test_trap_frame.c -o $(BUILD)/test_trap_frame
	./$(BUILD)/test_trap_frame
smoke: all
	python3 tools/qemu_smoke.py $(BUILD)/genesis.elf
run: all
	qemu-system-x86_64 -kernel $(BUILD)/genesis.elf -m 256M -display none -serial stdio -monitor none -no-reboot
clean:
	rm -rf $(BUILD)
