#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "MY_STUDENT.h"
#include "cli_mess.h"

//Usun��em mo�liwo�� wyboru typu CAR, stos jest uniwersalny dostosowany do przyjmowania warto�ci dowolnego typu

// Funkcje wewn�trzne
static void interf_push(Stack* stack);
static void interf_pop(Stack* stack);
static void interf_peek(Stack* stack);
static void interf_find_by_criteria(Stack* stack);
static void interf_save_to_file(Stack* stack);
static void interf_load_from_file(Stack** stack);

void UserMenu() {
    Stack* stack = initialize_stack(free_student,
        print_student,
        compare_students_by_surname,
        save_student_to_file,
        read_student_from_file);
    
    register_memory_for_cleanup(stack);
    
    int option;
    do {
        printf_s("Wybierz dzialanie:\n");

        printf("  1 - dodaj nowy element\n");
        printf("  2 - usun element (POP)\n");
        printf("  3 - pokaz pierwszy rekord (PEEK)\n");
        printf("  4 - znajdz element\n");
        printf("  5 - wypisz wszystkie elementy stosu\n");
        printf("  6 - zapisz na dysku\n");
        printf("  7 - odczytaj z dysku\n");
        printf("  8 - zwolnij pamiec stosu\n");
        printf("  0 - wyjscie\n");
        printf("Wybierz opcje: ");
        if (scanf("%d", &option) != 1) {
            throw_cli_mess(CLI_MESS_INVALID_INPUT);
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (option) {
        case 1:
            interf_push(stack);
            break;
        case 2:
            interf_pop(stack);
            break;
        case 3:
            interf_peek(stack);
            break;
        case 4:
            interf_find_by_criteria(stack);
            break;
        case 5:
            interf_print_stack(stack);
            break;
        case 6:
            interf_save_to_file(stack);
            break;
        case 7:
            interf_load_from_file(&stack);
            break;
        case 8:
            //SF: warning C4047: 'function': 'Stack *' differs in levels of indirection from 'Stack **'
            free_stack(stack);
            stack = initialize_stack(free_student, print_student,
                compare_students_by_surname,
                save_student_to_file,
                read_student_from_file);
            printf("Stos zostal wyczyszczony.\n");
            break;
        case 0:
            free_stack(stack);
            break;
        default:
            printf("Nieprawidlowa opcja!\n");
        }
    } while (option != 0);
}

MY_STUDENT* input_student(void) {
    char surname[100];
    int birth_year;

    printf("Podaj nazwisko studenta: ");
    if (fgets(surname, sizeof(surname), stdin) == NULL) {
        return NULL;
    }
    surname[strcspn(surname, "\n")] = 0;  // Usu� znak nowej linii

    printf("Podaj rok urodzenia: ");
    if (scanf("%d", &birth_year) != 1) {
        clear_input_buffer();
        return NULL;
    }
    clear_input_buffer();

    StudyField field = input_field_of_study();

    return create_student(surname, birth_year, field);
}

void interf_print_stack(const Stack* stack) {
    if (stack_is_empty(stack)) {
        throw_cli_mess(CLI_MESS_STACK_EMPTY);
        return;
    }
    printf("\nLista studentow:\n");
    peek_all(stack);
}

StudyField input_field_of_study(void) {
    printf("\nDostepne kierunki studiow:\n");
    for (int i = 0; i < END_OF_ENUM; i++) {
        printf("%d. %s\n", i + 1, FIELD_NAMES[i]);
    }

    int choice;
    do {
        printf("Wybierz kierunek (1-%d): ", END_OF_ENUM);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > END_OF_ENUM) {
            printf("Nieprawidlowy wybor!\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);

    printf("\n");
    clear_input_buffer();
    return (StudyField)(choice - 1);
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Implementacje funkcji wewn�trznych
static void interf_push(Stack* stack) {
    MY_STUDENT* student = input_student();
    if (student) {
        if (!push(stack, student)) {
            free_student(student);
        }
    }
}

static void interf_pop(Stack* stack) {
    void* data = pop(stack);
    if (data) {
        printf("Usunieto studenta:\n");
        print_student(data);
        free_student(data);
    }
}

static void interf_peek(Stack* stack) {
    void* data = peek(stack);
    if (data) {
        printf("Pierwszy student na stosie:\n");
        print_student(data);
    }
}

static void interf_find_by_criteria(Stack* stack) {
    printf("Podaj nazwisko studenta do znalezienia: ");
    char surname[100];
    if (fgets(surname, sizeof(surname), stdin)) {
        surname[strcspn(surname, "\n")] = 0;

        MY_STUDENT temp = { surname, 0, END_OF_ENUM };  // Tylko nazwisko jest istotne przy por�wnywaniu
        void* found = stack_find_by_criteria(stack, &temp);

        //if (surname[0] == 'x')
        //    *surname = NULL;  //SF ????

        if (found) {
            printf("Znaleziono studenta:\n");
            print_student(found);
        }
        else {
            printf("Nie znaleziono studenta o nazwisku %s\n", surname);
        }
    }
}

static void interf_save_to_file(Stack* stack) {
    printf("Podaj nazwe pliku do zapisu: ");
    char filename[100];
    if (fgets(filename, sizeof(filename), stdin)) {
        filename[strcspn(filename, "\n")] = 0;

        //SF warning C4047: 'function': 'void **' differs in levels of indirection from 'StackItem *'
        if (stack_save_to_file(stack, filename)) {
            printf("Pomy�lnie zapisano do pliku %s\n", filename);
        }
    }
}

static void interf_load_from_file(Stack** stack) {
    printf("Podaj nazw� pliku do odczytu: ");
    char filename[100];
    if (fgets(filename, sizeof(filename), stdin)) {
        filename[strcspn(filename, "\n")] = 0;
        Stack* tmp_stack = stack_load_from_file(filename,
            free_student,
            print_student,
            compare_students_by_surname,
            save_student_to_file,
            read_student_from_file);

        if (tmp_stack) {
            //SF: warning C4047: 'function': 'Stack *' differs in levels of indirection from 'Stack **'
            free_stack(*stack);
            *stack = tmp_stack;
            printf("Pomy�lnie wczytano z pliku %s\n", filename);
        }
    }
}

//Funkcj� odpowiedzialn� za inicjalizacj� stosu implementuj� w stack.c, a nast�pnie wywo�uj� j� w odpowiednich miejscach,
//funkcja interf_initialize_stack nie mia�a wp�ywiu na dzia�anie pierwotnej funkcji inicjalizuj�cej
/*Stack* interf_initialize_stack(void) {
    Stack* stack = initialize_stack();
    //SF: warning C4716: 'interf_initialize_stack': must return a value
}*/