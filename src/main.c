/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

/*

 push_int 24
 store 0    # store top to register 0

 push_r 0   # (int) 24

*/

int main(int argc, const char* argv[]) {
	char code[] = {
		I_PUSH_INT, 18, 00, 00, 00,
		I_STORE, 0,
		I_EXIT
	};

	TyosVM_state* vm = vm_create();
	vm_exec(vm, code, array_size(code));
	vm_free(vm);
	return 0;
}