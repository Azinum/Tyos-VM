/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

int main(int argc, const char* argv[]) {
	char code[] = {
		I_SKIP,
		I_PUSH_INT, 0x0f, 00, 00, 00,
		I_PUSH_INT, 0x0e, 00, 00, 00,
		I_ADD,
		I_EXIT,
	};

	TyosVM_state* vm = vm_create();
	vm_exec(vm, code);
	vm_free(vm);
	return 0;
}