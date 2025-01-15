#include <stdio.h>
#include "error_handling.h"

// Funkcja wewn�trzna
static const char* error_messages[] = {
    "Brak b��du",
    "B��d alokacji pami�ci",
    "B��d otwarcia pliku",
    "B��d odczytu pliku",
    "B��d zapisu do pliku",
    "Stos jest pusty",
    "Stos jest pe�ny",
    "Nieprawid�owe dane wej�ciowe",
    "Stos nie zosta� zainicjalizowany"
};

// Funkcje zewn�trzne
void handle_error(ERROR_CODE error) {
    if (error != ERROR_NONE) {
        fprintf(stderr, "B��D: %s\n", error_messages[error]);
    }
}

const char* get_error_message(ERROR_CODE error) {
    return error_messages[error];
}