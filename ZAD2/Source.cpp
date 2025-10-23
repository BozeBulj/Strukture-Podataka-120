#define _CRT_SECURE_NO_WARNINGS
/*Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicija strukture osobe
typedef struct Person {
    char firstName[20];
    char lastName[20];
    int birthYear;
    struct Person* next; // pokazivaè na sljedeæu osobu
} Person;

// Funkcija za dodavanje nove osobe na poèetak liste
Person* addB(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person)); //alokacija memorije za newPeron
    if (newPerson == NULL) {
        printf("Greška pri alokaciji memorije!\n");
        return head;
    }

    printf("Unesite ime: ");
    scanf(" %19s", newPerson->firstName);
    printf("Unesite prezime: ");
    scanf(" %19s", newPerson->lastName);
    printf("Unesite godinu roðenja: ");
    scanf("%d", &newPerson->birthYear);

    // nova osoba pokazuje na trenuttni poèetak
    newPerson->next = head;

    // nova osoba postaje novi poèetak
    return newPerson;
}

// funkcija za ispis svih osoba u listi
int printList(Person* head) {
    Person* current = head;
    int count = 0;

    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    printf("\n--- Sadrzaj liste ---\n");
    while (current != NULL) { //ve dok vrijednost current odnosno head nije NUll
        printf("Ime: %s, Prezime: %s, Godina roðenja: %d\n",
            current->firstName, current->lastName, current->birthYear); //upisuje sve u strukuturu
        current = current->next;
        count++;
    }
    printf("\n");
    return count; // vraæa broj osoba
}

// osslobaðanje memorije
void freeList(Person* head) {
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next; 
        free(temp); //cisti svu memoriju
    }
}

int main() {
    Person* head = NULL; // poèetak liste
    int choice;

    do {
        printf("\n1 - Dodaj osobu na pocetak\n");
        printf("2 - Ispisi listu\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            head = addB(head);
            break;
        case 2: {
            int total = printList(head);
            printf("Ukupan broj osoba: %d\n", total); //korisnik bira sta ce, moglo se i priko if i else
            break;
        }
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    } while (choice != 0);

    freeList(head);
    return 0;
}