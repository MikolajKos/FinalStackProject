#include <stdio.h>
#include "stack.h"
#include "MY_STUDENT.h"
#include "interface.h"
#include "error_handling.h"

//SF
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main() {
    // Inicjalizacja stosu
    Stack* stack = stack_init(free_student,
        print_student,
        compare_students_by_surname,
        save_student_to_file,
        read_student_from_file);

    if (!stack) {
        handle_error(ERROR_STACK_NOT_INITIALIZED);
        return 1;
    }

    // Obs³uga menu
    handle_menu(stack);

    // Zwolnienie pamiêci
    stack_destroy(stack);

    //SF
    if (_CrtDumpMemoryLeaks())
        printf("LEAK OF MEMORY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    return 0;
}