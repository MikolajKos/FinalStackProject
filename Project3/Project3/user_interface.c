#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_interface.h"
#include "MY_STUDENT.h"
#include "cli_mess.h"

// Funkcje wewnêtrzne
static void interf_push(Stack* stack);
static void interf_pop(Stack* stack);
static void interf_peek(Stack* stack);
static void interf_find(Stack* stack);
static void interf_save(Stack* stack);
static void interf_load(Stack** stack);

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
    int option;
    do {
        show_menu();
        if (scanf("%d", &option) != 1) {
            handle_error(ERROR_INVALID_INPUT);
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
            interf_find(stack);
            break;
        case 5:
            interf_print_stack(stack);
            break;
        case 6:
            interf_save(stack);
            break;
        case 7:
            interf_load(&stack);
            break;
        case 8:
            free_stack(stack);
            stack = initialize_stack(free_student, print_student,
                compare_students_by_surname,
                save_student_to_file,
                read_student_from_file);
            printf("Stos zosta³ wyczyszczony.\n");
            break;
        case 0:
            printf("Do widzenia!\n");
            break;
        default:
            printf("Nieprawid³owa opcja!\n");
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
    surname[strcspn(surname, "\n")] = 0;  // Usuñ znak nowej linii

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
        printf("Stos jest pusty.\n");
        return;
    }
    printf("\nLista studentów:\n");
    stack_print_all(stack);
}

StudyField input_field_of_study(void) {
    printf("\nDostêpne kierunki studiów:\n");
    for (int i = 0; i < END_OF_ENUM; i++) {
        printf("%d. %s\n", i + 1, FIELD_NAMES[i]);
    }

    int choice;
    do {
        printf("Wybierz kierunek (1-%d): ", END_OF_ENUM);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > END_OF_ENUM) {
            printf("Nieprawid³owy wybór!\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);

    clear_input_buffer();
    return (StudyField)(choice - 1);
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Implementacje funkcji wewnêtrznych
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
        printf("Usuniêto studenta:\n");
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

static void interf_find(Stack* stack) {
    printf("Podaj nazwisko studenta do znalezienia: ");
    char surname[100];
    if (fgets(surname, sizeof(surname), stdin)) {
        surname[strcspn(surname, "\n")] = 0;

        MY_STUDENT temp = { surname, 0, END_OF_ENUM };  // Tylko nazwisko jest istotne przy porównywaniu
        void* found = stack_find_by_criteria(stack, &temp);

        if (found) {
            printf("Znaleziono studenta:\n");
            print_student(found);
        }
        else {
            printf("Nie znaleziono studenta o nazwisku %s\n", surname);
        }
    }
}

static void interf_save(Stack* stack) {
    printf("Podaj nazwê pliku do zapisu: ");
    char filename[100];
    if (fgets(filename, sizeof(filename), stdin)) {
        filename[strcspn(filename, "\n")] = 0;
        if (stack_save_to_file(stack, filename)) {
            printf("Pomyœlnie zapisano do pliku %s\n", filename);
        }
    }
}

static void interf_load(Stack** stack) {
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
            free_stack(*stack);
            *stack = new_stack;
            printf("Pomyœlnie wczytano z pliku %s\n", filename);
        }
    }
}