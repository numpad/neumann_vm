#include <stdio.h>
#include <stdlib.h>

/* Data types */
typedef unsigned char vm_byte;
typedef unsigned short vm_word;

/* Von Neumann Opcodes */
enum {
	/* data transfer */
	NOP  = 0x10, LDM  = 0x11,
	LDI  = 0x12, LDA  = 0x18,
	STI  = 0x21, STM  = 0x28,
	/* arithmetic, logic and shift */
	ADD  = 0x30, SUB  = 0x31,
	MUL  = 0x32, DIV  = 0x33,
	AND  = 0x34, OR   = 0x35,
	NOT  = 0x36, XOR  = 0x37,
	INC  = 0x38, DEC  = 0x39,
	LEFT = 0x3c, RIGT = 0x3d,
	/* flow control */
	JM   = 0x41, JA   = 0x48,
	JZM  = 0x51, JNM  = 0x52,
	JLM  = 0x53, JZA  = 0x58,
	JNA  = 0x59, JLA  = 0x5a,
	/* io */
	IN   = 0x61, OUT  = 0x71
} vm_opcode;

/* Structure */
typedef struct {
	vm_word *memory;
	vm_word accu;
	size_t pc;
} vm_t;

vm_t vm_new() {
	vm_t vm;
	
	/* Initialize a new virtual machine */
	vm.memory = calloc(0xFF, sizeof(vm_word));
	vm.accu = 0;
	vm.pc = 0;

	return vm;
}

/* Frees resources reserved by the vm */
void vm_delete(vm_t *vm) {
	free(vm->memory);
}

/* Fetches data from memory and increases pc by 1 */
short vm_fetch(vm_t *vm) {
	return vm->memory[vm->pc];
}

/* Splits instruction into op and param */
void vm_decode(vm_word instruction, vm_byte *op, vm_byte *param) {
	*op = (instruction & 0xFF00) >> 8;
	*param = instruction & 0x00FF;
}

void vm_execute(vm_t *vm, vm_byte op, vm_byte param) {
	
	switch (op) {
		case NOP: /* no operation */
			break;
		case LDM: /* accu = [param] */
			vm->accu = vm->memory[param];
			break;
		case LDI: /**/
			
			break:
	};
}

#define MEM(addr) printf("[%d]: 0x%X\n", addr, vm.memory[addr])
int main(int argc, char *argv[]) {
	
	vm_t vm = vm_new();

	/* Programm */
	vm.memory[0] = 0x1805;
	vm.memory[1] = 0x30A0;
	vm.memory[2] = 0x28A0;
	/* Speicher */
	vm.memory[0xA0] = 20;
	
	MEM(0xa0);

	/* Programm ausführen */
	while (vm.pc < 0xFF) {
		vm_word instruction = vm_fetch(&vm);
		vm_byte op, param;
		vm_decode(instruction, &op, &param);

		vm_execute(&vm, op, param);

		if (vm.pc >= 3) break;
		vm.pc++;
	}

	MEM(0xa0);
	
	return 0;
}
