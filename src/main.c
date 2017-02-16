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
		  6, 0, 0, 0,	/* SIZE */
		  I_PUSH_INT, 9, 00, 00, 00,	/* push 9 */
		  I_RET,
		I_CALL, 0x1, 00, 00, 00,
		I_EXIT,
	};
	
	TyosVM_state* vm = vm_create();
	vm_exec(vm, code, array_size(code));
	vm_free(vm);
	return 0;
}