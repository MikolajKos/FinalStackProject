#include "stdafx.h"
#include "stack.h"
#include "cli_mess.h"

/**
 * @brief Inicjalizuje nowy stos.
 *
 * @param Funkcje niezb�dne dla prawid�owej obs�ugi stosu
 *
 * @details Funkcja alokuje pami�� dla strukutry stosu i inicjalizuje jej pola, w tym wska�niki do funkcji obs�ugi danych.
 *          W przypadku b��du alokacji pami�ci, zg�aszany jest b��d CLI_MESS_ALLOC_ERROR, a funkcja zwraca NULL.
 *
 * @return Wska�nik na nowo utworzony stos lub NULL, je�li alokacja pami�ci si� nie powiedzie.
 *
 * @note Pami�� zaalokowan� dla stosu nale�y zwolni� za pomoc� odpowiedniej funkcji, aby unikn�� wyciek�w pami�ci.
 */
Stack* initialize_stack(
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        throw_cli_mess(CLI_MESS_ALLOC_ERROR);
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

/**
 * @brief Zwalnia pami�� zajmowan� przez stos i jego elementy.
 *
 * @param stack Wska�nik na struktur� stosu, kt�ry ma zosta� zwolniony.
 *
 * @details Funkcja iteracyjnie usuwa elementy stosu, zwalniaj�c pami�� zajmowan� przez przechowywane dane za pomoc� funkcji `free_data`,
 *          je�li zosta�a dostarczona. Nast�pnie zwalnia pami�� samej struktury stosu.
 *          Je�li przekazany wska�nik `stack` jest NULL, zg�aszany jest b��d CLI_MESS_STACK_EMPTY.
 */
void free_stack(Stack* stack) {
    if (!stack) {
        throw_cli_mess(CLI_MESS_STACK_EMPTY);
        return;
    }

    while (!stack_is_empty(stack)) {
        void* data = pop(stack);
        if (data && stack->free_data) {
            stack->free_data(data);
        }
    }
    //SF Tu gubimy pamiec
    free(stack);
}

/**
 * @brief Dodaje nowy element na szczyt stosu.
 *
 * @param stack Wska�nik na struktur� stosu, do kt�rego ma zosta� dodany nowy element.
 * @param data Wska�nik na dane, kt�re maj� zosta� dodane do stosu.
 *
 * @details Funkcja dodaje do stosu dane przekazane w argumencie data typu void*.
 *
 * @return Zwraca 1, je�li operacja zako�czy�a si� sukcesem, lub 0 w przypadku b��du (np. niepowodzenie alokacji pami�ci
 *         lub niezainicjalizowany stos).
 *
 * @note Pami�� dla danych dodawanych do stosu musi by� zarz�dzana przez u�ytkownika.
 */
int push(Stack* stack, void* data) {
    if (!stack) {
        throw_cli_mess(CLI_MESS_STACK_NOT_INITIALIZED);
        return 0;
    }

    StackItem* item = (StackItem*)malloc(sizeof(StackItem));
    if (!item) {
        throw_cli_mess(CLI_MESS_ALLOC_ERROR);
        return 0;
    }

    item->data = data;
    item->next = stack->top;
    stack->top = item;
    stack->size++;

    return 1;
}

/**
 * @brief Usuwa i zwraca element znajduj�cy si� na szczycie stosu.
 *
 * @param stack Wska�nik na struktur� stosu, z kt�rego ma zosta� usuni�ty element.
 *
 * @return Wska�nik na dane przechowywane w usuni�tym elemencie.
 *         Je�li stos jest pusty, zwraca NULL i zg�asza CLI_MESS_STACK_EMPTY.
 *
 * @details Funkcja usuwa element znajduj�cy si� na szczycie stosu, zwalnia pami�� zajmowan� przez ten element,
 *          aktualizuje wska�nik top stosu oraz zwraca wska�nik na dane usuni�tego elementu.
 */
void* pop(Stack* stack) {
    if (!stack || stack_is_empty(stack)) {
        throw_cli_mess(CLI_MESS_STACK_EMPTY);
        return NULL;
    }

    StackItem* temp = stack->top;
    void* data = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;

    return data;
}

/**
 * @brief Zwraca dane z elementu znajduj�cego si� na szczycie stosu bez jego usuwania.
 *
 * @param stack Wska�nik na struktur� stosu.
 *
 * @details Je�li stos jest pusty lub niezainicjalizowany, zg�aszany jest b��d CLI_MESS_STACK_EMPTY i zwracane jest NULL.
 *
 * @return Wska�nik na dane z elementu na szczycie stosu lub NULL w przypadku b��du.
 */
void* peek(const Stack* stack) {
    if (!stack || stack_is_empty(stack)) {
        throw_cli_mess(CLI_MESS_STACK_EMPTY);
        return NULL;
    }
    return stack->top->data;
}

int stack_is_empty(const Stack* stack) {
    return (!stack || stack->top == NULL);
}

/**
 * @brief Wypisuje wszystkie dane przechowywane w stosie bez ich usuwania.
 *
 * @param stack Wska�nik na struktur� stosu.
 *
 * @details Je�li stos jest niezainicjalizowany, zg�aszany jest b��d CLI_MESS_STACK_NOT_INITIALIZED.
 *          W przypadku pustego stosu wypisywana jest odpowiednia informacja.
 *          Funkcja iteruje po wszystkich elementach stosu, wywo�uj�c `print_data` dla danych ka�dego elementu.
 *
 * @return Funkcja nie zwraca warto�ci.
 */
void peek_all(const Stack* stack) {
    if (!stack) {
        throw_cli_mess(CLI_MESS_STACK_NOT_INITIALIZED);
        return;
    }

    if (stack_is_empty(stack)) {
        throw_cli_mess(CLI_MESS_STACK_EMPTY);
        return;
    }

    StackItem* current = stack->top;
    while (current) {
        stack->print_data(current->data);
        current = current->next;
    }
}

/**
 * @brief Wyszukuje element w stosie spe�niaj�cy okre�lone kryterium.
 *
 * @param stack Wska�nik na struktur� stosu.
 * @param data Wska�nik na dane, kt�re maj� by� por�wnywane z elementami stosu.
 *
 * @details Funkcja przeszukuje stos od g�ry do do�u, por�wnuj�c elementy za pomoc� funkcji `compare_data`.
 *          Je�li stos jest niezainicjalizowany, zg�aszany jest b��d CLI_MESS_STACK_NOT_INITIALIZED.
 *          W przypadku znalezienia dopasowanego elementu zwracany jest wska�nik na jego dane,
 *          a w przeciwnym razie zwracane jest NULL.
 *
 * @return Wska�nik na dane znalezionego elementu lub NULL, je�li brak dopasowania.
 */
void* stack_find_by_criteria(const Stack* stack, const void* data) {
    if (!stack) {
        throw_cli_mess(CLI_MESS_STACK_NOT_INITIALIZED);
        return NULL;
    }

    StackItem* current = stack->top;
    while (current) {
        if (stack->compare_data(current->data, data) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Zapisuje stos do pliku.
 *
 * @param stack Wska�nik na stos.
 * @param filename Nazwa pliku, do kt�rego ma zosta� zapisany stos.
 *
 * @details Funkcja zapisuje rozmiar stosu oraz dane jego element�w do pliku w trybie binarnym.
 *          Je�li wyst�pi b��d podczas otwierania pliku lub zapisu danych, zg�aszany jest odpowiedni b��d.
 *
 * @return 1, je�li zapis zako�czy� si� sukcesem, 0 w przypadku b��du.
 */
int stack_save_to_file(const Stack* stack, const char* filename) {
    if (!stack) {
        throw_cli_mess(CLI_MESS_STACK_NOT_INITIALIZED);
        return 0;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        throw_cli_mess(CLI_MESS_FILE_OPEN);
        return 0;
    }

    // Zapisz rozmiar stosu
    fwrite(&stack->size, sizeof(int), 1, file);

    // Zapisz elementy stosu
    StackItem* current = stack->top;
    while (current) {
        if (!stack->save_data(file, current->data)) {
            throw_cli_mess(CLI_MESS_FILE_WRITE);
            fclose(file);
            return 0;
        }
        current = current->next;
    }

    fclose(file);
    return 1;
}

/**
 * @brief Wczytuje stos z pliku.
 *
 * @param Przekazane funkcje odpowiedzialne za prawid�owy odczyt z pliku.
 *
 * @details Funkcja wczytuje rozmiar stosu i jego elementy z pliku, a nast�pnie tworzy stos z odpowiednimi funkcjami.
 *          W przypadku b��du (np. podczas odczytu lub alokacji) zwraca NULL.
 *
 * @return Wska�nik na wczytany stos lub NULL w przypadku b��du.
 */
Stack* stack_load_from_file(const char* filename,
    void (*free_data)(void*),
    void (*print_data)(const void*),
    int (*compare_data)(const void*, const void*),
    int (*save_data)(FILE*, const void*),
    void* (*read_data)(FILE*)
) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        throw_cli_mess(CLI_MESS_FILE_OPEN);
        return NULL;
    }

    Stack* stack = initialize_stack(free_data, print_data, compare_data, save_data, read_data);
    if (!stack) {
        fclose(file);
        return NULL;
    }

    int size;
    if (fread(&size, sizeof(int), 1, file) != 1) {
        throw_cli_mess(CLI_MESS_FILE_READ);
        free_stack(stack);
        fclose(file);
        return NULL;
    }

    // Wczytaj elementy
    for (int i = 0; i < size; i++) {
        void* data = read_data(file);
        if (!data || !push(stack, data)) {
            free_stack(stack);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return stack;
}