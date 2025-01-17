#ifndef STACK_H
#define STACK_H

#include "stdafx.h"
#include <string.h>

typedef struct StackItem StackItem;
typedef struct Stack Stack;

struct StackItem {
    void* data;
    StackItem* next;
};

struct Stack {
    StackItem* top;
    int size;
    void (*free_data)(void*);
    void (*print_data)(const void*);
    int (*compare_data)(const void*, const void*);
    int (*save_data)(FILE*, const void*);
    void* (*read_data)(FILE*);
};

// Funkcje zewnêtrzne
Stack* initialize_stack(
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
);
void free_stack(Stack* stack);
int push(Stack* stack, void* data);
void* pop(Stack* stack);
void* peek(const Stack* stack);
int stack_is_empty(const Stack* stack);
void peek_all(const Stack* stack);
void* stack_find_by_criteria(const Stack* stack, const void* data);
int stack_save_to_file(const Stack* stack, const char* filename);
Stack* stack_load_from_file(const char* filename,
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
);

#endif