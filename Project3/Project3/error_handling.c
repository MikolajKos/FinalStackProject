#include <stdio.h>
#include "error_handling.h"

// Funkcja wewnêtrzna
static const char* error_messages[] = {
    "Brak b³êdu",
    "B³¹d alokacji pamiêci",
    "B³¹d otwarcia pliku",
    "B³¹d odczytu pliku",
    "B³¹d zapisu do pliku",
    "Stos jest pusty",
    "Stos jest pe³ny",
    "Nieprawid³owe dane wejœciowe",
    "Stos nie zosta³ zainicjalizowany"
};

// Funkcje zewnêtrzne
void handle_error(ERROR_CODE error) {
    if (error != ERROR_NONE) {
        fprintf(stderr, "B£¥D: %s\n", error_messages[error]);
    }
}

const char* get_error_message(ERROR_CODE error) {
    return error_messages[error];
}