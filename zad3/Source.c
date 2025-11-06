#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktura za osobu
typedef struct Person {
    char name[50];
    char surname[50];
    int birthYear;
    struct Person* next;
} Person;

// ====== Deklaracije funkcija ======

// Stvara novu osobu
Person* createPerson(char* name, char* surname, int birthYear);

// Dodaje osobu iza odreðene osobe (po prezimenu)
Person* addAfter(Person* head, char* targetSurname, char* name, char* surname, int birthYear);

// Dodaje osobu ispred odreðene osobe (po prezimenu)
Person* addBefore(Person* head, char* targetSurname, char* name, char* surname, int birthYear);

// Sortira listu po prezimenima
Person* sortList(Person* head);

// Sprema listu u datoteku
int writeToFile(Person* head, const char* filename);

// Uèitava listu iz datoteke
Person* readFromFile(const char* filename);

// Ispisuje cijelu listu
int printList(Person* head);


// ====== Glavna funkcija ======
int main() {
    // Kreiranje poèetne liste
    Person* head = createPerson("Marko", "Peric", 1998);
    head->next = createPerson("Ana", "Horvat", 2000);
    head->next->next = createPerson("Ivan", "Zoric", 1995);

    printf("Initial list:\n");
    printList(head);

    printf("\nAdding after 'Horvat':\n");
    head = addAfter(head, "Horvat", "Luka", "Kovac", 1999);
    printList(head);

    printf("\nAdding before 'Zoric':\n");
    head = addBefore(head, "Zoric", "Petar", "Maric", 1997);
    printList(head);

    printf("\nSorted list by surname:\n");
    head = sortList(head);
    printList(head);

    printf("\nWriting list to file 'people.txt'...\n");
    if (writeToFile(head, "people.txt"))
        printf("File saved successfully.\n");

    printf("\nReading list from file:\n");
    Person* newList = readFromFile("people.txt");
    printList(newList);

    return 0;
}


// ====== Definicije funkcija ======

// ------------------------------------------------------------
// Stvara novog èlana liste i vraæa pokazivaè na njega
// ------------------------------------------------------------
Person* createPerson(char* name, char* surname, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));  // Dinamièka alokacija memorije
    if (!newPerson) return NULL; // Provjera alokacije

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

// ------------------------------------------------------------
// Dodaje novi element iza odreðenog (po prezimenu)
// ------------------------------------------------------------
Person* addAfter(Person* head, char* targetSurname, char* name, char* surname, int birthYear) {
    Person* temp = head;

    // Traženje osobe s traženim prezimenom
    while (temp != NULL && strcmp(temp->surname, targetSurname) != 0)
        temp = temp->next;

    // Ako osoba nije pronaðena
    if (temp == NULL) {
        printf("Person with surname '%s' not found.\n", targetSurname);
        return head;
    }

    // Stvaranje nove osobe i umetanje iza pronaðene
    Person* newPerson = createPerson(name, surname, birthYear);
    newPerson->next = temp->next;
    temp->next = newPerson;

    return head; // Vraæamo (moguæe nepromijenjenu) glavu liste
}

// ------------------------------------------------------------
// Dodaje novi element ispred odreðenog (po prezimenu)
// ------------------------------------------------------------
Person* addBefore(Person* head, char* targetSurname, char* name, char* surname, int birthYear) {
    // Ako je lista prazna
    if (head == NULL) return head;

    // Ako je prvi element cilj
    if (strcmp(head->surname, targetSurname) == 0) {
        Person* newPerson = createPerson(name, surname, birthYear);
        newPerson->next = head;
        return newPerson; // Novi postaje glava
    }

    Person* prev = head;
    Person* curr = head->next;

    // Traženje elementa koji treba doæi ispred ciljanog
    while (curr != NULL && strcmp(curr->surname, targetSurname) != 0) {
        prev = curr;
        curr = curr->next;
    }

    // Ako cilj nije pronaðen
    if (curr == NULL) {
        printf("Person with surname '%s' not found.\n", targetSurname);
        return head;
    }

    // Stvaranje i umetanje nove osobe
    Person* newPerson = createPerson(name, surname, birthYear);
    newPerson->next = curr;
    prev->next = newPerson;

    return head;
}

// ------------------------------------------------------------
// Sortira listu abecedno po prezimenu
// ------------------------------------------------------------
Person* sortList(Person* head) {
    if (head == NULL || head->next == NULL) return head;

    Person* i;
    Person* j;
    char tempName[50], tempSurname[50];
    int tempYear;

    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->surname, j->surname) > 0) {
                // Zamjena sadržaja èvorova
                strcpy(tempName, i->name);
                strcpy(tempSurname, i->surname);
                tempYear = i->birthYear;

                strcpy(i->name, j->name);
                strcpy(i->surname, j->surname);
                i->birthYear = j->birthYear;

                strcpy(j->name, tempName);
                strcpy(j->surname, tempSurname);
                j->birthYear = tempYear;
            }
        }
    }

    return head;
}

// ------------------------------------------------------------
// Sprema listu u datoteku
// ------------------------------------------------------------
int writeToFile(Person* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file.\n");
        return 0; // neuspjeh
    }

    Person* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }

    fclose(file);
    return 1; // uspjeh
}

// ------------------------------------------------------------
// Uèitava listu iz datoteke
// ------------------------------------------------------------
Person* readFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error reading file.\n");
        return NULL;
    }

    Person* head = NULL;
    Person* tail = NULL;
    char name[50], surname[50];
    int year;

    // Èitanje redak po redak
    while (fscanf(file, "%s %s %d", name, surname, &year) == 3) {
        Person* newPerson = createPerson(name, surname, year);
        if (head == NULL)
            head = newPerson;
        else
            tail->next = newPerson;
        tail = newPerson;
    }

    fclose(file);
    return head;
}

// ------------------------------------------------------------
// Ispisuje sve elemente liste u terminal
// ------------------------------------------------------------
int printList(Person* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return 0;
    }

    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    return 1;
}

