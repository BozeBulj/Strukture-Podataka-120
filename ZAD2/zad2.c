#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NOT_OPENED (-1)

// Definicija strukture osobe
typedef struct Person {
    char firstName[20];
    char lastName[20];
    int birthYear;
    struct Person* next; // pokazivaè na sljedeæu osobu u listi
} Person;

// ---------------------------- FUNKCIJE ----------------------------

// A. Dodaj osobu na poèetak liste
Person* addB(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person)); // alokacija memorije
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

    newPerson->next = head; // nova osoba pokazuje na trenutni poèetak
    return newPerson;       // nova osoba postaje novi poèetak
}

// C. Dodaj osobu na kraj liste
Person* addE(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
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
    newPerson->next = NULL; // zadnji element nema sljedeæeg

    // ako je lista prazna, nova osoba postaje prvi element
    if (head == NULL)
        return newPerson;

    // inaèe prolazimo do kraja
    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newPerson; // zadnji pokazuje na novog
    return head;
}

// B. Ispis svih osoba u listi
int printList(Person* head) {
    Person* current = head;
    int count = 0;

    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    printf("\n--- Sadržaj liste ---\n");
    while (current != NULL) {
        printf("Ime: %s, Prezime: %s, Godina roðenja: %d\n",
            current->firstName, current->lastName, current->birthYear);
        current = current->next;
        count++;
    }
    printf("---------------------\n");
    return count; // vraæa broj osoba
}

// D. Pronalaženje osobe po prezimenu
Person* findByLastName(Person* head, const char* lastName) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0) {
            printf("Pronaðena osoba: %s %s, %d\n",
                temp->firstName, temp->lastName, temp->birthYear);
            return temp;
        }
        temp = temp->next;
    }
    printf("Osoba s prezimenom '%s' nije pronaðena.\n", lastName);
    return NULL;
}

// E. Brisanje osobe po prezimenu
Person* deleteByLastName(Person* head, const char* lastName) {
    Person* temp = head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba s prezimenom '%s' nije pronaðena.\n", lastName);
        return head;
    }

    if (prev == NULL) {
        // brišemo prvi element
        head = temp->next;
    }
    else {
        // preskaèemo element koji se briše
        prev->next = temp->next;
    }

    free(temp);
    printf("Osoba s prezimenom '%s' obrisana.\n", lastName);
    return head;
}

// Oslobaðanje memorije
void freeList(Person* head) {
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// ---------------------------- MAIN ----------------------------
int main() {
    Person* head = NULL; // poèetak liste
    int choice;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Dodaj osobu na poèetak\n");
        printf("2 - Ispisi listu\n");
        printf("3 - Dodaj osobu na kraj\n");
        printf("4 - Pronaði po prezimenu\n");
        printf("5 - Obriši po prezimenu\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            head = addB(head);
            break;
        case 2: {
            int total = printList(head);
            printf("Ukupan broj osoba: %d\n", total);
            break;
        }
        case 3:
            head = addE(head);
            break;
        case 4: {
            char prezime[20];
            printf("Unesite prezime za pretragu: ");
            scanf(" %19s", prezime);
            findByLastName(head, prezime);
            break;
        }
        case 5: {
            char prezime[20];
            printf("Unesite prezime za brisanje: ");
            scanf(" %19s", prezime);
            head = deleteByLastName(head, prezime);
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