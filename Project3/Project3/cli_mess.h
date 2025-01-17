#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include "stdafx.h"
#include "stack.h"

typedef enum {
    CLI_MESS_NONE,
    CLI_MESS_ALLOC_ERROR,
    CLI_MESS_FILE_OPEN,
    CLI_MESS_FILE_READ,
    CLI_MESS_FILE_WRITE,
    CLI_MESS_STACK_EMPTY,
    CLI_MESS_STACK_FULL,
    CLI_MESS_INVALID_INPUT,
    CLI_MESS_STACK_NOT_INITIALIZED
} CLIENT_MESSAGES;

void throw_cli_mess(CLIENT_MESSAGES mess);

void register_memory_for_cleanup(void* ptr);
void cleanup_all_memory(void);

#endif