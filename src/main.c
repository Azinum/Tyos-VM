/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

int main(int argc, const char* argv[]) {
	char code[] = {
		I_PUSH_INT, 0x5, 0, 0, 0,
		I_PUSH_INT, 0x7, 0, 0, 0,
		I_DIV,
		I_EXIT,
		/* I_DEF,
	      1, 0, 0, 0,
		  12, 0, 0, 0,
		  I_PUSH_INT, 9, 00, 00, 00,
		  I_PUSH_INT, 8, 00, 00, 00,
		  I_ADD,
		  I_RET,
		I_CALL, 0x1, 00, 00, 00,
		I_EXIT, */
	};
	TyosVM_state* vm = vm_create();
	vm_exec(vm, code, array_size(code));
	vm_free(vm);
	return 0;
}