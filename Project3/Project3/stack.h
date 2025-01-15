#ifndef STACK_H
#define STACK_H

typedef struct StackNode StackNode;
typedef struct Stack Stack;

// Definicja wêz³a stosu
struct StackNode {
    void* data;
    StackNode* next;
};

// Definicja stosu
struct Stack {
    StackNode* top;
    int size;
    void (*free_data)(void*);
    void (*print_data)(const void*);
    int (*compare_data)(const void*, const void*);
    int (*save_data)(FILE*, const void*);
    void* (*read_data)(FILE*);
};

// Funkcje zewnêtrzne
Stack* stack_init(
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
);
void stack_destroy(Stack* stack);
int stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void* stack_peek(const Stack* stack);
int stack_is_empty(const Stack* stack);
void stack_print(const Stack* stack);
void* stack_find(const Stack* stack, const void* data);
int stack_save_to_file(const Stack* stack, const char* filename);
Stack* stack_load_from_file(const char* filename,
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
);

#endif