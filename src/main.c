/*
** main.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

/*
def, (int)id, (int)size,
 data0,
 data1, ...
 ret
*/

int main(int argc, const char* argv[]) {
	char code[] = {
		I_DEF,
	      0x1, 00, 00, 00,	/* ID */
		  0x1, 0x0, 00, 00,	/* SIZE */
		I_ALERT,
		I_CALL,
		  00, 00, 00, 00,
		I_EXIT,
	};

	TyosVM_state* vm = vm_create();
	vm_exec(vm, code, array_size(code));
	vm_free(vm);
	return 0;
}