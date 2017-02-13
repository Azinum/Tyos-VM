/*
** vm.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"
#include "table.h"


#define stack_push(obj) (vm->stack[++vm->top] = obj)

#define stack_pop() (--vm->top)

#define stack_top() (vm->stack[vm->top])

#define stack_get(delta) (vm->stack[vm->top + delta])

#define op_arith(op) stack_get(-1).value.i op stack_top().value.i; stack_pop()

void vm_init(TyosVM_state* vm) {
	vm->stack_size = array_size(vm->stack);
	vm->top = 0;
	vm->ip = 0;
}

void vm_print_top(TyosVM_state* vm) {
	const struct Object obj = vm->stack[vm->top];
	switch (obj.type) {
		case T_INT:
			printf("%i\n", obj.value.i);
			break;

		default:
			break;
	}
}

TyosVM_state* vm_create() {
	TyosVM_state* vm = (TyosVM_state*)malloc(sizeof(TyosVM_state));
	if (!vm) {
		printf("%s\n", "Failed to allocate VM state");
		return NULL;
	}
	vm_init(vm);
	return vm;
}

int vm_exec(TyosVM_state* vm, char* code) {
	if (!code) {	/* Just in case */
		return 1;
	}

	for (;;) {
		switch (code[vm->ip++]) {
			case I_SKIP:
				break;

			case I_ADD:
				op_arith(+=);
				break;

			case I_SUB:
				op_arith(-=);
				break;

			case I_MULT:
				op_arith(*=);
				break;

			case I_DIV:
				op_arith(/=);
				break;


			case I_PUSH_INT: {
				stack_push(((Object){
					{.i = *(int*)&code[vm->ip]},
					T_INT
				}));
				vm->ip += sizeof(int);
			}
				break;

			case I_EXIT:
				vm_print_top(vm);
				return 0;

			default:
				break;
		}
	}

	return 1;
}

void vm_free(TyosVM_state* vm) {

	free(vm);
}