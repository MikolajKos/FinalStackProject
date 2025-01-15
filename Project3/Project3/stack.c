#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "error_handling.h"

// Funkcje zewnêtrzne
Stack* stack_init(
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        handle_error(ERROR_MEMORY_ALLOCATION);
        return NULL;
    }

    stack->top = NULL;
    stack->size = 0;
    stack->free_data = free_data;
    stack->print_data = print_data;
    stack->compare_data = compare_data;
    stack->save_data = save_data;
    stack->read_data = read_data;

    return stack;
}

void stack_destroy(Stack* stack) {
    if (!stack) return;

    while (!stack_is_empty(stack)) {
        void* data = stack_pop(stack);
        if (data && stack->free_data) {
            stack->free_data(data);
        }
    }
    //SF Tu gubimy pamiec
    free(stack);
}

int stack_push(Stack* stack, void* data) {
    if (!stack) {
        handle_error(ERROR_STACK_NOT_INITIALIZED);
        return 0;
    }

    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (!new_node) {
        handle_error(ERROR_MEMORY_ALLOCATION);
        return 0;
    }

    new_node->data = data;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;

    return 1;
}

void* stack_pop(Stack* stack) {
    if (!stack || stack_is_empty(stack)) {
        handle_error(ERROR_STACK_EMPTY);
        return NULL;
    }

    StackNode* temp = stack->top;
    void* data = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;

    return data;
}

void* stack_peek(const Stack* stack) {
    if (!stack || stack_is_empty(stack)) {
        handle_error(ERROR_STACK_EMPTY);
        return NULL;
    }
    return stack->top->data;
}

int stack_is_empty(const Stack* stack) {
    return (!stack || stack->top == NULL);
}

void stack_print(const Stack* stack) {
    if (!stack) {
        handle_error(ERROR_STACK_NOT_INITIALIZED);
        return;
    }

    if (stack_is_empty(stack)) {
        printf("Stos jest pusty\n");
        return;
    }

    StackNode* current = stack->top;
    while (current) {
        stack->print_data(current->data);
        current = current->next;
    }
}

void* stack_find(const Stack* stack, const void* data) {
    if (!stack) {
        handle_error(ERROR_STACK_NOT_INITIALIZED);
        return NULL;
    }

    StackNode* current = stack->top;
    while (current) {
        if (stack->compare_data(current->data, data) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

int stack_save_to_file(const Stack* stack, const char* filename) {
    if (!stack) {
        handle_error(ERROR_STACK_NOT_INITIALIZED);
        return 0;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        handle_error(ERROR_FILE_OPEN);
        return 0;
    }

    // Zapisz rozmiar stosu
    fwrite(&stack->size, sizeof(int), 1, file);

    // Zapisz elementy stosu
    StackNode* current = stack->top;
    while (current) {
        if (!stack->save_data(file, current->data)) {
            handle_error(ERROR_FILE_WRITE);
            fclose(file);
            return 0;
        }
        current = current->next;
    }

    fclose(file);
    return 1;
}

Stack* stack_load_from_file(const char* filename,
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        handle_error(ERROR_FILE_OPEN);
        return NULL;
    }

    Stack* stack = stack_init(free_data, print_data, compare_data, save_data, read_data);
    if (!stack) {
        fclose(file);
        return NULL;
    }

    int size;
    if (fread(&size, sizeof(int), 1, file) != 1) {
        handle_error(ERROR_FILE_READ);
        stack_destroy(stack);
        fclose(file);
        return NULL;
    }

    // Wczytaj elementy
    for (int i = 0; i < size; i++) {
        void* data = read_data(file);
        if (!data || !stack_push(stack, data)) {
            stack_destroy(stack);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return stack;
}