#ifndef INTERFACE_H
#define INTERFACE_H

#include "stack.h"
#include "MY_STUDENT.h"

// Funkcje zewnêtrzne
void show_menu(void);
void handle_menu(Stack* stack);
MY_STUDENT* input_student(void);
void interf_print_stack(const Stack* stack);
StudyField input_field_of_study(void);
void clear_input_buffer(void);

#endif