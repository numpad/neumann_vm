#ifndef VM_H
#define VM_H

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
	enum {ALU_EQUAL, ALU_NOTEQUAL, ALU_LESSEQUAL, ALU_UNDEFINED, ALU_UNINITIALIZED = 0} alu;
} vm_t;

vm_t vm_new() {
	vm_t vm;
	
	/* Initialize a new virtual machine */
	vm.memory = calloc(0xFF, sizeof(vm_word));
	vm.accu = 0;
	vm.pc = 0;
	vm.alu = ALU_UNINITIALIZED;

	return vm;
}

/* Frees resources reserved by the vm */
void vm_delete(vm_t *vm) {
	free(vm->memory);
}

/* Updates and sets the alu flag */
void vm_alu_update_flag(vm_t *vm) {
	if      (vm->accu == 0) vm->alu = ALU_EQUAL;
	else if (vm->accu != 0) vm->alu = ALU_NOTEQUAL;
	else if (vm->accu <= 0) vm->alu = ALU_LESSEQUAL;
	else                    vm->alu = ALU_UNDEFINED;
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
		/* transfer */
		case NOP: /* no operation */
			break;
		case LDM: /* accu = [param] */
			vm->accu = vm->memory[param];
			break;
		case LDI: /* accu = [[param]] */
			vm->accu = vm->memory[vm->memory[param]];
			break;
		case LDA: /* accu = param */
			vm->accu = param;
			break;
		case STI: /* [[param]] = accu */
			vm->memory[vm->memory[param]] = vm->accu;
			break;
		case STM: /* [param] = accu */
			vm->memory[param] = vm->accu;
			break;
		/* arithmetic, logic and shift */
		case ADD: /* accu = accu + [param] */
			vm->accu += vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case SUB: /* accu = accu - [param] */
			vm->accu -= vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case MUL: /* accu = accu * [param] */
			vm->accu *= vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case DIV: /* accu = accu / [param] */
			vm->accu /= vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case AND: /* accu = accu & [param] */
			vm->accu &= vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case OR:  /* accu = accu | [param] */
			vm->accu |= vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case NOT: /* accu = ~accu */
			vm->accu = ~vm->accu;
			vm_alu_update_flag(vm);
			break;
		case XOR: /* accu = accu ^ [param] */
			vm->accu = vm->accu ^ vm->memory[param];
			vm_alu_update_flag(vm);
			break;
		case INC: /* accu = accu + 1 */
			++vm->accu;
			vm_alu_update_flag(vm);
			break;
		case DEC: /* accu = accu - 1 */
			--vm->accu;
			vm_alu_update_flag(vm);
			break;
		case LEFT: /* accu = accu << param */
			vm->accu <<= param;
			vm_alu_update_flag(vm);
			break;
		case RIGT: /* accu = accu >> param */
			vm->accu >>= param;
			vm_alu_update_flag(vm);
			break;
		/* flow control */
		case JM: /* jump [param] */
			vm->pc = vm->memory[param];
			break;
		case JA: /* jump param */
			vm->pc = param;
			break;
		case JZM: /* if alu == 0: jump [param] */
			if (vm->alu == ALU_EQUAL)
				vm->pc = vm->memory[param];
			break;
		case JNM: /* if alu != 0: jump [param] */
			if (vm->alu == ALU_NOTEQUAL)
				vm->pc = vm->memory[param];
			break;
		case JLM: /* if alu <= 0: jump [param] */
			if (vm->alu == ALU_LESSEQUAL)
				vm->pc = vm->memory[param];
			break;
		case JZA: /* if alu == 0: jump param */
			if (vm->alu == ALU_EQUAL)
				vm->pc = param;
			break;
		case JNA: /* if alu != 0: jump param */
			if (vm->alu == ALU_NOTEQUAL)
				vm->pc = param;
			break;
		case JLA: /* if alu <= 0: jump param */
			if (vm->alu == ALU_LESSEQUAL)
				vm->pc = param;
			break;
		/* io */
		case IN: /* [param] = read */
			printf("<< [%X] = ", param);
			scanf("%hX", &vm->memory[param]);
			break;
		case OUT: /* print [param] */
			printf(">> [%X] = 0x%X\n", param, vm->memory[param]);
			break;
	};
	
	/* increase program counter */
	++vm->pc;
}


#endif
