#ifndef MY_STUDENT_H
#define MY_STUDENT_H

typedef enum {
    INFORMATYKA,
    MATEMATYKA,
    FIZYKA,
    CHEMIA
    // mo¿na dodaæ wiêcej kierunków
} FIELD_OF_STUDY;

extern const char* FIELD_NAMES[];

typedef struct {
    char* surname;
    int birth_year;
    FIELD_OF_STUDY field;
} MY_STUDENT;

// Funkcje zewnêtrzne
MY_STUDENT* create_student(const char* surname, int birth_year, FIELD_OF_STUDY field);
void free_student(void* student);
void print_student(const void* student);
int compare_students_by_surname(const void* student1, const void* student2);
int save_student_to_file(FILE* file, const void* student);
MY_STUDENT* read_student_from_file(FILE* file);

#endif