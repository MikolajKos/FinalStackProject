#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "MY_STUDENT.h"
#include "error_handling.h"

// Funkcje wewnêtrzne
static void process_push(Stack* stack);
static void process_pop(Stack* stack);
static void process_peek(Stack* stack);
static void process_find(Stack* stack);
static void process_save(Stack* stack);
static void process_load(Stack** stack);

// Funkcje zewnêtrzne
void show_menu(void) {
    printf("1. Dodaj studenta (PUSH)\n");
    printf("2. Usun studenta (POP)\n");
    printf("3. Pokaz pierwszego studenta (PEEK)\n");
    printf("4. Znajdz studenta\n");
    printf("5. Wyswietl wszystkich studentow\n");
    printf("6. Zapisz do pliku\n");
    printf("7. Wczytaj z pliku\n");
    printf("8. Wyczysc stos\n");
    printf("0. Wyjscie\n");
    printf("Wybierz opcje: ");
}

void handle_menu(Stack* stack) {
    int choice;
    do {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            handle_error(ERROR_INVALID_INPUT);
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case MENU_PUSH:
            process_push(stack);
            break;
        case MENU_POP:
            process_pop(stack);
            break;
        case MENU_PEEK:
            process_peek(stack);
            break;
        case MENU_FIND:
            process_find(stack);
            break;
        case MENU_PRINT:
            print_student_list(stack);
            break;
        case MENU_SAVE:
            process_save(stack);
            break;
        case MENU_LOAD:
            process_load(&stack);
            break;
        case MENU_CLEAR:
            stack_destroy(stack);
            stack = stack_init(free_student, print_student,
                compare_students_by_surname,
                save_student_to_file,
                read_student_from_file);
            printf("Stos zosta³ wyczyszczony.\n");
            break;
        case MENU_EXIT:
            printf("Do widzenia!\n");
            break;
        default:
            printf("Nieprawid³owa opcja!\n");
        }
    } while (choice != MENU_EXIT);
}

MY_STUDENT* input_student(void) {
    char surname[100];
    int birth_year;

    printf("Podaj nazwisko studenta: ");
    if (fgets(surname, sizeof(surname), stdin) == NULL) {
        return NULL;
    }
    surname[strcspn(surname, "\n")] = 0;  // Usuñ znak nowej linii

    printf("Podaj rok urodzenia: ");
    if (scanf("%d", &birth_year) != 1) {
        clear_input_buffer();
        return NULL;
    }
    clear_input_buffer();

    FIELD_OF_STUDY field = input_field_of_study();

    return create_student(surname, birth_year, field);
}

void print_student_list(const Stack* stack) {
    if (stack_is_empty(stack)) {
        printf("Stos jest pusty.\n");
        return;
    }
    printf("\nLista studentów:\n");
    stack_print(stack);
}

FIELD_OF_STUDY input_field_of_study(void) {
    printf("\nDostêpne kierunki studiów:\n");
    for (int i = 0; i <= CHEMIA; i++) {
        printf("%d. %s\n", i + 1, FIELD_NAMES[i]);
    }

    int choice;
    do {
        printf("Wybierz kierunek (1-%d): ", CHEMIA + 1);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > CHEMIA + 1) {
            printf("Nieprawid³owy wybór!\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);

    clear_input_buffer();
    return (FIELD_OF_STUDY)(choice - 1);
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Implementacje funkcji wewnêtrznych
static void process_push(Stack* stack) {
    MY_STUDENT* student = input_student();
    if (student) {
        if (!stack_push(stack, student)) {
            free_student(student);
        }
    }
}

static void process_pop(Stack* stack) {
    void* data = stack_pop(stack);
    if (data) {
        printf("Usuniêto studenta:\n");
        print_student(data);
        free_student(data);
    }
}

static void process_peek(Stack* stack) {
    void* data = stack_peek(stack);
    if (data) {
        printf("Pierwszy student na stosie:\n");
        print_student(data);
    }
}

static void process_find(Stack* stack) {
    printf("Podaj nazwisko studenta do znalezienia: ");
    char surname[100];
    if (fgets(surname, sizeof(surname), stdin)) {
        surname[strcspn(surname, "\n")] = 0;

        MY_STUDENT temp = { surname, 0, INFORMATYKA };  // Tylko nazwisko jest istotne przy porównywaniu
        void* found = stack_find(stack, &temp);

        if (found) {
            printf("Znaleziono studenta:\n");
            print_student(found);
        }
        else {
            printf("Nie znaleziono studenta o nazwisku %s\n", surname);
        }
    }
}

static void process_save(Stack* stack) {
    printf("Podaj nazwê pliku do zapisu: ");
    char filename[100];
    if (fgets(filename, sizeof(filename), stdin)) {
        filename[strcspn(filename, "\n")] = 0;
        if (stack_save_to_file(stack, filename)) {
            printf("Pomyœlnie zapisano do pliku %s\n", filename);
        }
    }
}

static void process_load(Stack** stack) {
    printf("Podaj nazwê pliku do odczytu: ");
    char filename[100];
    if (fgets(filename, sizeof(filename), stdin)) {
        filename[strcspn(filename, "\n")] = 0;
        Stack* new_stack = stack_load_from_file(filename,
            free_student,
            print_student,
            compare_students_by_surname,
            save_student_to_file,
            read_student_from_file);
        if (new_stack) {
            stack_destroy(*stack);
            *stack = new_stack;
            printf("Pomyœlnie wczytano z pliku %s\n", filename);
        }
    }
}