/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

/*
 
 push_int 1
 if A, B

 if true
   goto addr A
 if false
   goto addr B

*/

int main(int argc, const char* argv[]) {
	char code[] = {
		I_PUSH_INT,
			0x1, 0x0, 0x0, 0x0,
		I_STORE,
			0x0,
		I_POP,
		I_PUSH_R,
			0x0,
		I_PUSH_INT,
			0x5, 0x0, 0x0, 0x0,
		I_LT,
		I_IF,
			26, 0x0, 0x0, 0x0,
			0x0, 0x0, 0x0, 0x0,
		I_EXIT
	};

	TyosVM_state* vm = vm_create();
	vm_exec(vm, code, array_size(code));
	vm_free(vm);
	return 0;
}