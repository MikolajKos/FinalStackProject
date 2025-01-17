#ifndef MY_STUDENT_H
#define MY_STUDENT_H

#include "stdafx.h"

typedef enum {
    MA,  // Matematyka
    PR,  // Programowanie
    FA,  // Fizyka
    IF,   // Informatyka
    END_OF_ENUM //Oznaczenie zakoñczenia
} StudyField;

extern const char* FIELD_NAMES[];

// Struktura studenta
typedef struct {
    char* surname;
    int birth_year;
    StudyField field;
} MY_STUDENT;

// Funkcje zewnêtrzne
MY_STUDENT* create_student(const char* surname, int birth_year, StudyField field);
void free_student(void* student);
void print_student(const void* student);
int compare_students_by_surname(const void* student1, const void* student2);
int save_student_to_file(FILE* file, const void* student);
MY_STUDENT* read_student_from_file(FILE* file);

#endif