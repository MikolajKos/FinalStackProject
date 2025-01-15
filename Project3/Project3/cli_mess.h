#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef enum {
    ERROR_NONE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_FILE_OPEN,
    ERROR_FILE_READ,
    ERROR_FILE_WRITE,
    ERROR_STACK_EMPTY,
    ERROR_STACK_FULL,
    ERROR_INVALID_INPUT,
    ERROR_STACK_NOT_INITIALIZED
} ERROR_CODE;

// Funkcje zewnêtrzne
void handle_error(ERROR_CODE error);
const char* get_error_message(ERROR_CODE error);

#endif