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

#define type_check(obj0, obj1, t) (obj0.type == obj1.type && obj0.type == t)

#define op_arith(op) stack_get(-1).value.i op stack_top().value.i; stack_pop()

#define MAX_ITER 1024 << 14  /* ~16 million */

void vm_init(TyosVM_state* vm) {
	vm->stack_size = array_size(vm->stack);
	vm->top = 0;
	vm->ip = 0;
	vm->fn_size = array_size(vm->functions);
	vm->ret_addr = 0;

	for (unsigned int i = 0; i < vm->fn_size; i++) {
		vm->functions[i] = 0;
	}
}

void vm_print_top(TyosVM_state* vm) {
	const struct Object obj = vm->stack[vm->top];
	switch (obj.type) {
		case T_INT:
			printf("%i\n", obj.value.i);
			break;

		case T_STRING:
			printf("%s\n", obj.value.s);
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

int vm_exec(TyosVM_state* vm, char* code, unsigned int size) {
	if (!code || size < 1) {	/* Just in case */
		return 1;
	}

	unsigned int prog_size = size;

	for (;;) {
		switch (code[vm->ip++]) {
			case I_SKIP:
				break;

			case I_ALERT:
				printf("ALERT(%i)\n", vm->ip);
				break;

			case I_CALL: {
				unsigned int id = *(int*)&code[vm->ip];
				if (!vm->functions[id]) {
					printf("%s\n", "Failed to do call. Function does not exist.");
					return 0;
				}
				vm->ret_addr = vm->ip + sizeof(int);
				vm->ip = vm->functions[id];
			}
				break;

			case I_RET: {
				vm->ip = vm->ret_addr;
			}
				break;

			case I_DEF: {
				unsigned int id, size;
				id = *(int*)&code[vm->ip];
				vm->ip += (int)sizeof(int);
				size = *(int*)&code[vm->ip];
				vm->ip += (int)sizeof(int);

				if (id > vm->fn_size) {
					printf("Failed to allocate function. Index exceeded the limit of %i (Requested id was: %i).\n", vm->fn_size, id);
					return 0;
				}

				if (vm->functions[id]) {
					printf("Failed to allocate function. Address is already in use.\n");
					return 0;
				}

				if (vm->ip + size > prog_size) {
					printf("Function size is too big. Size exceeds the program total size. %i over %i.\n", vm->ip + size, prog_size);
					return 0;
				}
				vm->functions[id] = vm->ip;
				vm->ip += size;	/* So that the function will not get executed */
			}
				break;

			case I_ADD: {
				if (type_check(stack_get(-1), stack_top(), T_INT)) {
					op_arith(+=);
					break;
				}
				if (type_check(stack_get(-1), stack_top(), T_STRING)) {
					char* left = stack_get(-1).value.s;
					char* right = stack_top().value.s;
					unsigned long right_size = strlen(right);
					unsigned long left_size = strlen(left);
					left = (char*)realloc(left, left_size + right_size + 1);
					for (unsigned long i = left_size; i < left_size + right_size + 1; i++) {
						left[i] = right[i - left_size];
					}
					stack_pop();
				}
			}
				break;

			case I_SUB:
				if (type_check(stack_get(-1), stack_top(), T_INT))
					op_arith(-=);
				break;

			case I_MULT:
				if (type_check(stack_get(-1), stack_top(), T_INT))
					op_arith(*=);
				break;

			case I_DIV:
				if (type_check(stack_get(-1), stack_top(), T_INT))
					op_arith(/=);
				break;


			case I_JUMP: {
				vm->ip += *(int*)&code[vm->ip];
			}
				break;

			case I_PUSH_INT: {
				stack_push(((Object){
					{.i = *(int*)&code[vm->ip]},
					T_INT
				}));
				vm->ip += sizeof(int);
			}
				break;

			case I_PUSH_STR: {
				unsigned int size = 0;
				while (1) {
					if (code[vm->ip + size] == '\0') {
						break;
					}
					if (size > MAX_ITER || size > prog_size) {
						printf("(ip: %i) %s\n", vm->ip, "Iteration limit reached! Forgot to null terminate string?");
						return 0;
					}
					size++;
				};
				char* str = (char*)malloc(sizeof(char) * size);
				const unsigned int limit = size;
				while (size--) {
					str[size] = code[vm->ip + size];
				};
				vm->ip += limit + 1;
				stack_push(((Object){
				 	{.s = str},
				 	T_STRING
				}));
			}
				break;

			case I_POP: {
				vm->top > 0 ? stack_pop() : (void)0;
			}

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