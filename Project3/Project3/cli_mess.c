#include <stdio.h>
#include "cli_mess.h"

#define MAX_POINTERS 1000

static void* memory_pointers[MAX_POINTERS];
static int pointer_count = 0;

void register_memory_for_cleanup(void* ptr) {
    if (ptr && pointer_count < MAX_POINTERS) {
        memory_pointers[pointer_count++] = ptr;
    }
}

void cleanup_all_memory(void) {
    for (int i = 0; i < pointer_count; i++) {
        if (memory_pointers[i]) {
            free_stack(memory_pointers[i]);
     
            memory_pointers[i] = NULL;
        }
    }
    pointer_count = 0;
}

static const char* text_messages[] = {
    "W Brak bledu",
    "E Blad alokowania pamieci",
    "E Nie mozna otworzyc pliku",
    "E Blad odczytu pliku",
    "E Blad zapisu do pliku",
    "W Stos jest pusty",
    "W Stos jest pelny",
    "W Nieprawidlowe dane wejsciowe",
    "W Stos nie zosta³ zainicjalizowany"
};

//SF ta funkcja nie zwalnia zasoby aplikacji i nie przerywa wykonanie zadania w przypadku fatalnego zdarzenia.

void throw_cli_mess(CLIENT_MESSAGES mess) {
    if (mess != CLI_MESS_NONE) {
        puts(text_messages[mess] + 2);
        
        if (text_messages[mess][0] == 'E') {
            cleanup_all_memory();
            exit(1);
        }
    }
}