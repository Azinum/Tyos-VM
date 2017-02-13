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
	I_PUSH_INT,
	I_PUSH_STR,

	I_ADD,
	I_SUB,
	I_MULT,
	I_DIV,

	T_INT,
	T_STRING,
};

typedef struct Object {
	union {
		int i;
		char* s;
		void* p;
	} value;
	char type;
} Object;

typedef struct TyosVM_state {
	struct Object stack[64];
	unsigned long stack_size;
	unsigned char top;	/* stack will not exceed the 255 mark */
	unsigned int ip;	/* instruction pointer */
} TyosVM_state;

void vm_init(TyosVM_state* vm);

void vm_print_top(TyosVM_state* vm);

TyosVM_state* vm_create();

int vm_exec(TyosVM_state* vm, char* code);

void vm_free(TyosVM_state* vm);

#endif