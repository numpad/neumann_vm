#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

/* Prints usage information */
void usage(char *arg0) {
	puts  ("vm - a simple von neumann virtual machine");
	puts  ("usage:");
	printf(" %s -h, help\n", arg0);
	printf(" %s -c, assemble <in> [out]\n", arg0);
	printf(" %s -r, run <in>\n", arg0);
}

#define IS_OP(a) (!strcmp(opname, a))
vm_word vm_parseop(const char *opname) {
	if      (IS_OP("NOP"))	return NOP;
	else if (IS_OP("LDM"))	return LDM;
	else if (IS_OP("LDI"))	return LDI;
	else if (IS_OP("LDA"))	return LDA;
	else if (IS_OP("STI"))	return STI;
	else if (IS_OP("STM"))	return STM;
	else if (IS_OP("ADD"))	return ADD;
	else if (IS_OP("SUB"))	return SUB;
	else if (IS_OP("MUL"))	return MUL;
	else if (IS_OP("DIV"))	return DIV;
	else if (IS_OP("AND"))	return AND;
	else if (IS_OP("OR"))	return OR;
	else if (IS_OP("NOT"))	return NOT;
	else if (IS_OP("XOR"))	return XOR;
	else if (IS_OP("INC"))	return INC;
	else if (IS_OP("DEC"))	return DEC;
	else if (IS_OP("LEFT"))	return LEFT;
	else if (IS_OP("RIGT"))	return RIGT;
	else if (IS_OP("JM"))	return JM;
	else if (IS_OP("JA"))	return JA;
	else if (IS_OP("JZM"))	return JZM;
	else if (IS_OP("JNM"))	return JNM;
	else if (IS_OP("JLM"))	return JLM;
	else if (IS_OP("JZA"))	return JZA;
	else if (IS_OP("JNA"))	return JNA;
	else if (IS_OP("JLA"))	return JLA;
	else if (IS_OP("IN"))	return IN;
	else if (IS_OP("OUT"))	return OUT;
	return 0;
}
#undef STREQ

/* Assemble a file to bytecode */
vm_word *vm_assemble(const char *infn, const char *outfn) {
	FILE *fp = fopen(infn, "r+");
	FILE *fpout;
	if (outfn != NULL) {
		fpout = fopen(outfn, "w+");
	}
	
	if (!fp) {
		printf("Could not find \"%s\"\n", infn);
		return NULL;
	}	
	if (outfn != NULL && !fpout) {
		fclose(fp);
		printf("Coult not open \"%s\"\n", outfn);
		return NULL;
	}
	
	vm_word *program = calloc(0xFF, sizeof(vm_word));
	unsigned char location, param;
	char op[8];
	while (fscanf(fp, "%hhX:%s %hhX\n", &location, op, &param) != EOF) {
		program[location] = (vm_parseop(op) << 8) + param;
	}
	
	if (outfn != NULL) {
		fwrite(program, sizeof(vm_word), 0xFF, fpout);
		fclose(fpout);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return program;
}

/* Loads a binary file to memory */
void vm_loadmemory(vm_t *vm, const char *fnbin) {
	FILE *fp = fopen(fnbin, "r+");
	if (!fp) {
		printf("Could not open \"%s\"\n", fnbin);
		return;
	}
	
	fread(vm->memory, sizeof(vm_word), 0xFF, fp);

	fclose(fp);
}

#define VM_INSTRUCTION(op,param) (((op) << 8) + (param))
int main(int argc, char *argv[]) {
	
	vm_t vm = vm_new();
	
	if (argc > 1) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "help")) {
			usage(argv[0]);
			return 0;
		} else if (!strcmp(argv[1], "-c") || !strcmp(argv[1], "assemble")) {
			if (argc > 2) {
				vm_assemble(argv[2], (argc > 3 ? argv[3] : argv[2]));
				return 0;
			}
		} else if (!strcmp(argv[1], "-r") || !strcmp(argv[1], "run")) {
			if (argc > 2) {
				vm_loadmemory(&vm, argv[2]);
			}
		} else {
			free(vm.memory);
			vm.memory = vm_assemble(argv[1], NULL);
		}
	} else {
		usage(argv[0]);
		return 0;
	}

	
	/* Programm ausführen */
	while (vm.pc < 0xFF) {
		vm_word instruction = vm_fetch(&vm);
		vm_byte op, param;
		vm_decode(instruction, &op, &param);

		vm_execute(&vm, op, param);
	}

	return 0;
}
