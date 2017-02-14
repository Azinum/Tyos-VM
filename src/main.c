/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"


int main(int argc, const char* argv[]) {
	char code[] = {
		I_SKIP,
		I_PUSH_STR, 'H', 'e', 'y', ' ', '\0',
		I_PUSH_STR, 'Y', 'o', 'u', '\0',
		I_ADD,
		I_EXIT,
	};

	TyosVM_state* vm = vm_create();
	vm_exec(vm, code);
	vm_free(vm);
	return 0;
}