#include "stdafx.h"
#include "stack.h"
#include "cli_mess.h"

/**
 * @brief Inicjalizuje nowy stos.
 *
 * @param Funkcje niezbêdne dla prawid³owej obs³ugi stosu
 *
 * @details Funkcja alokuje pamiêæ dla strukutry stosu i inicjalizuje jej pola, w tym wskaŸniki do funkcji obs³ugi danych.
 *          W przypadku b³êdu alokacji pamiêci, zg³aszany jest b³¹d CLI_MESS_ALLOC_ERROR, a funkcja zwraca NULL.
 *
 * @return WskaŸnik na nowo utworzony stos lub NULL, jeœli alokacja pamiêci siê nie powiedzie.
 *
 * @note Pamiêæ zaalokowan¹ dla stosu nale¿y zwolniæ za pomoc¹ odpowiedniej funkcji, aby unikn¹æ wycieków pamiêci.
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
 * @brief Zwalnia pamiêæ zajmowan¹ przez stos i jego elementy.
 *
 * @param stack WskaŸnik na strukturê stosu, który ma zostaæ zwolniony.
 *
 * @details Funkcja iteracyjnie usuwa elementy stosu, zwalniaj¹c pamiêæ zajmowan¹ przez przechowywane dane za pomoc¹ funkcji `free_data`,
 *          jeœli zosta³a dostarczona. Nastêpnie zwalnia pamiêæ samej struktury stosu.
 *          Jeœli przekazany wskaŸnik `stack` jest NULL, zg³aszany jest b³¹d CLI_MESS_STACK_EMPTY.
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
 * @param stack WskaŸnik na strukturê stosu, do którego ma zostaæ dodany nowy element.
 * @param data WskaŸnik na dane, które maj¹ zostaæ dodane do stosu.
 *
 * @details Funkcja dodaje do stosu dane przekazane w argumencie data typu void*.
 *
 * @return Zwraca 1, jeœli operacja zakoñczy³a siê sukcesem, lub 0 w przypadku b³êdu (np. niepowodzenie alokacji pamiêci
 *         lub niezainicjalizowany stos).
 *
 * @note Pamiêæ dla danych dodawanych do stosu musi byæ zarz¹dzana przez u¿ytkownika.
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
 * @brief Usuwa i zwraca element znajduj¹cy siê na szczycie stosu.
 *
 * @param stack WskaŸnik na strukturê stosu, z którego ma zostaæ usuniêty element.
 *
 * @return WskaŸnik na dane przechowywane w usuniêtym elemencie.
 *         Jeœli stos jest pusty, zwraca NULL i zg³asza CLI_MESS_STACK_EMPTY.
 *
 * @details Funkcja usuwa element znajduj¹cy siê na szczycie stosu, zwalnia pamiêæ zajmowan¹ przez ten element,
 *          aktualizuje wskaŸnik top stosu oraz zwraca wskaŸnik na dane usuniêtego elementu.
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
 * @brief Zwraca dane z elementu znajduj¹cego siê na szczycie stosu bez jego usuwania.
 *
 * @param stack WskaŸnik na strukturê stosu.
 *
 * @details Jeœli stos jest pusty lub niezainicjalizowany, zg³aszany jest b³¹d CLI_MESS_STACK_EMPTY i zwracane jest NULL.
 *
 * @return WskaŸnik na dane z elementu na szczycie stosu lub NULL w przypadku b³êdu.
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
 * @param stack WskaŸnik na strukturê stosu.
 *
 * @details Jeœli stos jest niezainicjalizowany, zg³aszany jest b³¹d CLI_MESS_STACK_NOT_INITIALIZED.
 *          W przypadku pustego stosu wypisywana jest odpowiednia informacja.
 *          Funkcja iteruje po wszystkich elementach stosu, wywo³uj¹c `print_data` dla danych ka¿dego elementu.
 *
 * @return Funkcja nie zwraca wartoœci.
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
 * @brief Wyszukuje element w stosie spe³niaj¹cy okreœlone kryterium.
 *
 * @param stack WskaŸnik na strukturê stosu.
 * @param data WskaŸnik na dane, które maj¹ byæ porównywane z elementami stosu.
 *
 * @details Funkcja przeszukuje stos od góry do do³u, porównuj¹c elementy za pomoc¹ funkcji `compare_data`.
 *          Jeœli stos jest niezainicjalizowany, zg³aszany jest b³¹d CLI_MESS_STACK_NOT_INITIALIZED.
 *          W przypadku znalezienia dopasowanego elementu zwracany jest wskaŸnik na jego dane,
 *          a w przeciwnym razie zwracane jest NULL.
 *
 * @return WskaŸnik na dane znalezionego elementu lub NULL, jeœli brak dopasowania.
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
 * @param stack WskaŸnik na stos.
 * @param filename Nazwa pliku, do którego ma zostaæ zapisany stos.
 *
 * @details Funkcja zapisuje rozmiar stosu oraz dane jego elementów do pliku w trybie binarnym.
 *          Jeœli wyst¹pi b³¹d podczas otwierania pliku lub zapisu danych, zg³aszany jest odpowiedni b³¹d.
 *
 * @return 1, jeœli zapis zakoñczy³ siê sukcesem, 0 w przypadku b³êdu.
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
 * @param Przekazane funkcje odpowiedzialne za prawid³owy odczyt z pliku.
 *
 * @details Funkcja wczytuje rozmiar stosu i jego elementy z pliku, a nastêpnie tworzy stos z odpowiednimi funkcjami.
 *          W przypadku b³êdu (np. podczas odczytu lub alokacji) zwraca NULL.
 *
 * @return WskaŸnik na wczytany stos lub NULL w przypadku b³êdu.
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