#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MY_STUDENT.h"
#include "cli_mess.h"

//SF W zadaniu bylo zrealizowac zapis-odczyt, wydruk i poszukiwanie danych w obsludze stosu, jednak Pan przelozyl to na obsluge interfejsu
//Tym samym unknal Pan otdzielenia obslugi KONTENERA od obslugi danych - jednego z podstawowych momentow zadania projektowego.
//Prosze zrealizowac te czynnosci w obsludze stosu.

const char* FIELD_NAMES[] = {"Matematyka", "Programowanie", "Filologia Angielska", "Informatyka"};

/**
 * @brief Tworzy nowego studenta.
 *
 * @param surname Nazwisko studenta.
 * @param birth_year Rok urodzenia studenta.
 * @param field Kierunek studiów.
 *
 * @details Funkcja alokuje pamiêæ dla obiektu `MY_STUDENT`, kopiuje nazwisko i ustawia rok urodzenia oraz kierunek studiów.
 *          W przypadku b³êdu alokacji pamiêci zwraca NULL.
 *
 * @return WskaŸnik na utworzonego studenta lub NULL w przypadku b³êdu.
 */
MY_STUDENT* create_student(const char* surname, int birth_year, StudyField field) {
    MY_STUDENT* student = (MY_STUDENT*)malloc(sizeof(MY_STUDENT));
    if (!student) {
        throw_cli_mess(CLI_MESS_ALLOC_ERROR);
        return NULL;
    }

    student->surname = (char*)malloc(strlen(surname) + 1);
    if (!student->surname) {
        free(student);
        throw_cli_mess(CLI_MESS_ALLOC_ERROR);
        return NULL;
    }

    strcpy(student->surname, surname);
    student->birth_year = birth_year;
    student->field = field;

    return student;
}

void free_student(void* student) {
    if (student) {
        MY_STUDENT* s = (MY_STUDENT*)student;
        free(s->surname);
        free(s);
    }
}

void print_student(const void* student) {
    const MY_STUDENT* s = (const MY_STUDENT*)student;
    printf("Student: %s, Rok urodzenia: %d, Kierunek: %s\n",
        s->surname, s->birth_year, FIELD_NAMES[s->field]);
}

int compare_students_by_surname(const void* student1, const void* student2) {
    const MY_STUDENT* s1 = (const MY_STUDENT*)student1;
    const MY_STUDENT* s2 = (const MY_STUDENT*)student2;
    return strcmp(s1->surname, s2->surname);
}

int save_student_to_file(FILE* file, const void* student) {
    //Zmieni³em z: MyStudent* student = (MyStudent*)*pdat; Przekazujê wskaŸnik do danych zamiast wskaŸnika na wskaŸnik
    const MY_STUDENT* s = (const MY_STUDENT*)student;

    //Odczyt d³ugoœci nazwiska
    //SF dlugosc nazwiska trzeba dodac do struktury studenta, wyznaczyc przed zapisem i zapisac do pliku razem z innymi skladowymi obiektu MY_STUDENT
    size_t surname_length = strlen(s->surname);
    if (fwrite(&surname_length, sizeof(size_t), 1, file) != 1) return 0;

    //Zapis nazwiska
    if (fwrite(s->surname, sizeof(char), surname_length, file) != surname_length) return 0;

    //Zapis roku urodzenia
    if (fwrite(&s->birth_year, sizeof(int), 1, file) != 1) return 0;

    //Zapis kierunku studiów
    if (fwrite(&s->field, sizeof(StudyField), 1, file) != 1) return 0;

    return 1;
}

MY_STUDENT* read_student_from_file(FILE* file) {
    size_t surname_length;

    // Odczytaj d³ugoœæ nazwiska
    if (fread(&surname_length, sizeof(size_t), 1, file) != 1) return NULL;

    char* surname = (char*)malloc(surname_length + 1);
    if (!surname) {
        throw_cli_mess(CLI_MESS_ALLOC_ERROR);
        return NULL;
    }

    // Odczytaj nazwisko
    if (fread(surname, sizeof(char), surname_length, file) != surname_length) {
        free(surname);
        return NULL;
    }
    surname[surname_length] = '\0';

    int birth_year;
    StudyField field;

    // Odczytaj pozosta³e dane
    if (fread(&birth_year, sizeof(int), 1, file) != 1 ||
        fread(&field, sizeof(StudyField), 1, file) != 1) {
        free(surname);
        return NULL;
    }

    //SF Student nie ma zadnego pojecia, do ktorego kontenera jego beda dodawali. Dla tego jest to funkcjonalnosc interfejsu, a nie studenta.
    
    //Wczeœniej zczytane dane z pliku dodawa³em do stosu bezpoœrednio z poziomu MY_STUDENT.c, teraz te logikê przenios³em do funkcji stosu i interfejsu,
    //do których przekazujê te funkcjê
    MY_STUDENT* student = create_student(surname, birth_year, field);
    free(surname);
    return student;
}