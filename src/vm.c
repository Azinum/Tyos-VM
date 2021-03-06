/*
** vm.c - tyosvm
** Azinum 2017.02.13
*/

#include "vm.h"

#define stack_push(obj) (vm->stack[++vm->top] = obj)

#define stack_pop() (--vm->top)

#define stack_top() (vm->stack[vm->top])

#define stack_get(delta) (vm->stack[vm->top + delta])

#define type_check(obj0, obj1, t) (obj0.type == obj1.type && obj0.type == t)

#define op_arith(op) { \
	if (type_check(stack_get(-1), stack_top(), T_INT)) { \
		stack_get(-1).value.n op stack_top().value.n; stack_pop(); \
		break; \
	} \
} 

#define op_arith2(op) { \
	if (type_check(stack_get(-1), stack_top(), T_INT)) { \
		stack_get(-1).value.n = stack_get(-1).value.n op stack_top().value.n; \
		stack_pop(); \
		break; \
	} \
}

#define MAX_ITER 1024 << 14  /* ~16 million */

#define puterr(err, message) if (!vm_puterr(err, message)) return 0

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
			printf("%.6g\n", obj.value.n);
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
				puterr(ERR_WARNING, "Alert!");
				break;

			case I_IF: {
				if (vm_check_top(vm)) {
					vm->ip = *(int*)&code[vm->ip];
					break;
				}
				vm->ip = *(int*)&code[vm->ip + sizeof(int)];
			}
				break;

			case I_STORE: {
				struct Object obj;
				if (vm->top > 0 && (int)code[vm->ip] <= array_size(vm->registers)) {
					obj = stack_top();
					vm->registers[(int)code[vm->ip]] = obj;
					vm->ip++;
					break;
				}
				puterr(ERR_FATAL, "Failed to store data to register.");
			}
				break;

			case I_PUSH_R: {
				char index = code[vm->ip];
				if (index <= array_size(vm->registers)) {
					if (!vm->registers[(int)index].type) {
						puterr(ERR_FATAL, "Failed to push register to stack. Register is null.");
					}

					stack_push(vm->registers[(int)index]);
					vm->ip++;
					break;
				}
				puterr(ERR_FATAL, "Invalid register index (index too big)");
			}
				break;

			case I_CALL: {
				unsigned int id = *(int*)&code[vm->ip];
				if (!vm->functions[id]) {
					puterr(ERR_FATAL, "Failed to do call. Function does not exist.");
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
					puterr(ERR_FATAL, "Failed to allocate function. Address is already in use.");
				}

				if (vm->ip + size > prog_size) {
					printf("Function size is too big. Size exceeds the program total size. %i over %i.\n", vm->ip + size, prog_size);
					return 0;
				}
				vm->functions[id] = vm->ip;
				vm->ip += size;	/* So that the function will not get executed */
			}
				break;

			case I_JUMP: {
				vm->ip += *(int*)&code[vm->ip];
			}
				break;

			case I_PUSH_INT: {
				stack_push(((Object){
					{.n = *(int*)&code[vm->ip]},
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
						puterr(ERR_FATAL, "Iteration limit reached! Forgot to null terminate string?");
					}
					size++;
				};
				char* str = (char*)malloc(sizeof(char) * size);
				const unsigned int limit = size;
				while (size--) {
					str[size] = code[vm->ip + size];
				};
				vm->ip += limit + 1;
				stack_push(((Object) {
				 	{.s = str},
				 	T_STRING
				}));
			}
				break;

			case I_POP: {
				if (vm->top > 0) {
					stack_pop();
					break;
				}
				puterr(ERR_WARNING, "Failed to pop stack top (stack top is at 0).");
			}
				break;

			case I_ADD: {
				op_arith(+=);

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
				op_arith(-=);
				break;

			case I_MULT:
				op_arith(*=);
				break;

			case I_DIV:
				op_arith(/=);
				break;

			case I_LT:
				op_arith2(<);
				break;

			case I_GT:
				op_arith2(>);
				break;

			case I_LEQ:
				op_arith2(<=);
				break;

			case I_GEQ:
				op_arith2(>=);
				break;

			case I_EQ:
				op_arith2(==);
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

int vm_puterr(int err, const char* message) {
	if (err == ERR_FATAL) {
		printf("[FATAL ERROR] %s\n", message);
		return 0;
	}
	printf("[WARNING] %s\n", message);
	return 1;
}

int vm_check_top(TyosVM_state* vm) {
	if (vm->top > 0) {
		const struct Object obj = stack_top();
		switch (obj.type) {
			case T_INT:
				if (obj.value.n != 0)	/* returns true even if number is negative, just return 0 on 0 */
					return 1;
				break;

			case T_STRING:
				if (strlen(obj.value.s) >= 1)
					return 1;
				break;
			default:
				return 0;
		}
	}
	return 0;
}

void vm_free(TyosVM_state* vm) {

	free(vm);
}