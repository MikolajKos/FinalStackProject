#ifndef INTERFACE_H
#define INTERFACE_H

#include "stack.h"
#include "MY_STUDENT.h"

typedef enum {
    MENU_EXIT,
    MENU_PUSH,
    MENU_POP,
    MENU_PEEK,
    MENU_FIND,
    MENU_PRINT,
    MENU_SAVE,
    MENU_LOAD,
    MENU_CLEAR
} MENU_OPTION;

// Funkcje zewnêtrzne
void show_menu(void);
void handle_menu(Stack* stack);
MY_STUDENT* input_student(void);
void print_student_list(const Stack* stack);
FIELD_OF_STUDY input_field_of_study(void);
void clear_input_buffer(void);

#endif