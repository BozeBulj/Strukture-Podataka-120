#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char firstName[20];
    char lastName[20];
    int birthYear;
    struct Person* next;
} Person;

/* PROTOTIPOVI FUNKCIJA */
Person* createPersonFromInput(void);
Person* addB(Person* head);
Person* addE(Person* head);
int printList(Person* head);
Person* findByLastName(Person* head, const char*);
Person* deleteByLastName(Person* head, const char*);
Person* addAfter(Person* head, const char* targetLastName);
Person* addBefore(Person* head, const char* targetLastName);
Person* sortList(Person* head); // NOVO: sortiranje
void freeList(Person* head);

int main() {
    Person* head = NULL;
    int choice;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Dodaj osobu na poèetak\n");
        printf("2 - Ispisi listu\n");
        printf("3 - Dodaj osobu na kraj\n");
        printf("4 - Pronaði po prezimenu\n");
        printf("5 - Obriši po prezimenu\n");
        printf("6 - Dodaj osobu iza odreðenog prezimena\n");
        printf("7 - Dodaj osobu ispred odreðenog prezimena\n");
        printf("8 - Sortiraj listu po prezimenu\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }

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
        case 6: {
            char prezime[20];
            printf("Unesite prezime iza kojeg zelite umetnuti novi element: ");
            scanf(" %19s", prezime);
            head = addAfter(head, prezime);
            break;
        }
        case 7: {
            char prezime[20];
            printf("Unesite prezime ispred kojeg zelite umetnuti novi element: ");
            scanf(" %19s", prezime);
            head = addBefore(head, prezime);
            break;
        }
        case 8:
            head = sortList(head);
            printf("Lista je sortirana po prezimenu!\n");
            break;
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

/* Pomoæna funkcija: unos nove osobe */
Person* createPersonFromInput(void) {
    Person* p = (Person*)malloc(sizeof(Person));
    if (!p) {
        printf("Greška pri alokaciji memorije!\n");
        return NULL;
    }
    printf("Unesite ime: ");
    scanf(" %19s", p->firstName);
    printf("Unesite prezime: ");
    scanf(" %19s", p->lastName);
    printf("Unesite godinu roðenja: ");
    scanf("%d", &p->birthYear);
    p->next = NULL;
    return p;
}

/* A: dodaj na poèetak */
Person* addB(Person* head) {
    Person* newPerson = createPersonFromInput();
    if (!newPerson) return head;
    newPerson->next = head;
    return newPerson;
}

/* C: dodaj na kraj */
Person* addE(Person* head) {
    Person* newPerson = createPersonFromInput();
    if (!newPerson) return head;
    if (head == NULL) return newPerson;
    Person* temp = head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newPerson;
    return head;
}

/* Ispis liste */
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
    return count;
}

/* D: pronalaženje po prezimenu */
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

/* E: brisanje po prezimenu */
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
    if (prev == NULL) head = temp->next;
    else prev->next = temp->next;
    free(temp);
    printf("Osoba s prezimenom '%s' obrisana.\n", lastName);
    return head;
}

/* F: dodaj iza odreðenog prezimena */
Person* addAfter(Person* head, const char* targetLastName) {
    Person* cur = head;
    while (cur != NULL && strcmp(cur->lastName, targetLastName) != 0)
        cur = cur->next;

    if (cur == NULL) {
        printf("Prezime '%s' nije pronaðeno.\n", targetLastName);
        return head;
    }

    Person* newPerson = createPersonFromInput();
    if (!newPerson) return head;
    newPerson->next = cur->next;
    cur->next = newPerson;
    printf("Umetnuto iza prezimena '%s'.\n", targetLastName);
    return head;
}

/* G: dodaj ispred odreðenog prezimena */
Person* addBefore(Person* head, const char* targetLastName) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }
    if (strcmp(head->lastName, targetLastName) == 0) {
        Person* newPerson = createPersonFromInput();
        if (!newPerson) return head;
        newPerson->next = head;
        printf("Umetnuto ispred prezimena '%s'.\n", targetLastName);
        return newPerson;
    }

    Person* prev = head;
    Person* cur = head->next;
    while (cur != NULL && strcmp(cur->lastName, targetLastName) != 0) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Prezime '%s' nije pronaðeno.\n", targetLastName);
        return head;
    }

    Person* newPerson = createPersonFromInput();
    if (!newPerson) return head;
    prev->next = newPerson;
    newPerson->next = cur;
    printf("Umetnuto ispred prezimena '%s'.\n", targetLastName);
    return head;
}

/* H: sortiranje po prezimenu (bubble sort po pokazivaèima) */
Person* sortList(Person* head) {
    if (head == NULL || head->next == NULL) return head;

    int swapped;
    Person* ptr1, * lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->lastName, ptr1->next->lastName) > 0) {
                // zamjena cijelih struktura (ne samo pokazivaèa)
                char tempName[20], tempLast[20];
                int tempYear;

                strcpy(tempName, ptr1->firstName);
                strcpy(tempLast, ptr1->lastName);
                tempYear = ptr1->birthYear;

                strcpy(ptr1->firstName, ptr1->next->firstName);
                strcpy(ptr1->lastName, ptr1->next->lastName);
                ptr1->birthYear = ptr1->next->birthYear;

                strcpy(ptr1->next->firstName, tempName);
                strcpy(ptr1->next->lastName, tempLast);
                ptr1->next->birthYear = tempYear;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    return head;
}

/* Oslobaðanje memorije */
void freeList(Person* head) {
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
