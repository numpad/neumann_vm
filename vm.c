#include <stdio.h>
#include <stdlib.h>

typedef struct {
	short *memory;
	short accu;
	size_t pc;
} vm_t;

vm_t vm_new() {
	vm_t vm;
	
	/* Initialize a new virtual machine */
	vm.memory = calloc(0xFF, sizeof(short));
	vm.accu = 0;
	vm.pc = 0;

	return vm;
}

void wm_delete(vm_t *vm) {
	free(vm->memory);
}

int main(int argc, char *argv[]) {
	
	vm_t vm = vm_new();

	/* Programm */
	vm.memory[0] = 0x1805;
	vm.memory[1] = 0x30A0;
	vm.memory[2] = 0x28A0;
	/* Speicher */
	vm.memory[0xA0] = 20;
	
	printf("memory[0xA0] = 0x%X\n", memory[0xA0]);
	
	/* Programm ausführen */
	while (pc < 0xFF) {
		const unsigned char op = (memory[pc] & 0xFF00) >> 8;
		const unsigned char val = (memory[pc] & 0xFF);
		
		switch (op) {
		case 0x18:
			accu = val;
			break;
		case 0x30:
			accu += memory[val];
			break;
		case 0x28:
			memory[val] = accu;
			break;
		};

		++pc;	
		if (pc >= 3) break;
	}
	
	printf("memory[0xA0] = 0x%X\n", memory[0xA0]);

	return 0;
}
