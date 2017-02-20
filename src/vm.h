/*
** vm.h - tyosvm
** Azinum 2017.02.13
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef VM_H
#define VM_H

#define array_size(array) (sizeof(array) / sizeof(array[0]))

enum Instructions {
	I_EXIT = 0,
	I_SKIP,
	I_ALERT,
	I_STORE,
	I_PUSH_R,
	I_CALL,	/* call (int)id */
	I_RET,
	I_DEF,	/* define (int)id, (int)size */
	I_JUMP,	/* jump n (bytes) */
	I_PUSH_INT,
	I_PUSH_STR,
	I_POP,

	I_ADD,
	I_SUB,
	I_MULT,
	I_DIV,
};


enum Types {
	T_NULL = 0,
	T_INT,
	T_STRING
};

enum {
	ERR_FATAL,
	ERR_WARNING
};

typedef struct Object {
	union {
		double n;
		char* s;
		void* p;
	} value;
	char type;
} Object;

typedef struct TyosVM_state {
	struct Object stack[64];
	struct Object registers[255];	/* store variables here */
	unsigned long stack_size;
	unsigned char top;	/* stack will not exceed the 255 mark */
	unsigned int ip;	/* instruction pointer */
	unsigned int functions[255];	/* internal functions */
	unsigned int fn_size;
	unsigned int ret_addr;
} TyosVM_state;

void vm_init(TyosVM_state* vm);

void vm_print_top(TyosVM_state* vm);

TyosVM_state* vm_create();

int vm_exec(TyosVM_state* vm, char* code, unsigned int size);

int vm_puterr(int err, const char* message);

void vm_free(TyosVM_state* vm);

#endif