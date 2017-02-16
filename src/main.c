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
	      1, 0, 0, 0,	/* ID */
		  12, 0, 0, 0,	/* SIZE */
		  I_PUSH_INT, 9, 00, 00, 00,
		  I_PUSH_INT, 8, 00, 00, 00,
		  I_ADD,
		  I_RET,
		I_DEF,
	      2, 0, 0, 0,
		  12, 0, 0, 0,
		  I_PUSH_INT, 3, 00, 00, 00,
		  I_PUSH_INT, 4, 00, 00, 00,
		  I_MULT,
		  I_RET,
		I_CALL, 0x1, 00, 00, 00,
		I_CALL, 0x2, 00, 00, 00,
		I_EXIT,
	};
	
	TyosVM_state* vm = vm_create();
	vm_exec(vm, code, array_size(code));
	vm_free(vm);
	return 0;
}