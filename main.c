/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

int main(int argc, const char* argv[]) {
	char code[] = {
		I_SKIP,
		I_PUSH_INT, 0x5, 0x00, 0x0, 0x0,
		I_PUSH_INT, 0xa, 0x00, 0x0, 0x0,
		I_ADD,
		I_EXIT,
	};

	TyosVM_state* vm = vm_create();
	vm_exec(vm, code);
	vm_free(vm);
	return 0;
}