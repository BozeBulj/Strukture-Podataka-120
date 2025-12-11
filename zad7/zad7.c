#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 50

/* ---------------------- DEKLARACIJE ---------------------- */

typedef struct Directory {
    char name[NAME_SIZE];
    struct Directory* child;
    struct Directory* sibling;
    struct Directory* parent;
} Directory;

Directory* createDirectory(const char* name);
int makeDirectory(Directory* current, const char* name);
Directory* changeDirectory(Directory* current, const char* name);
Directory* goBack(Directory* current);
int listDirectory(Directory* current);
int freeDirectory(Directory* dir);


/* ------------------------------ MAIN ------------------------------ */

int main() {

    Directory* root = createDirectory("C:");
    Directory* current = root;

    int choice;
    char dirname[NAME_SIZE];

    while (1) {
        printf("\n----------------------------------\n");
        printf("Trenutni direktorij: %s\n", current->name);
        printf("1 - md (napravi direktorij)\n");
        printf("2 - cd dir (udji u direktorij)\n");
        printf("3 - cd.. (nazad)\n");
        printf("4 - dir (ispis sadrzaja)\n");
        printf("5 - exit\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Unesi ime novog direktorija: ");
            scanf("%s", dirname);
            makeDirectory(current, dirname);
            break;

        case 2:
            printf("Unesi ime direktorija: ");
            scanf("%s", dirname);
            current = changeDirectory(current, dirname);
            break;

        case 3:
            current = goBack(current);
            break;

        case 4:
            listDirectory(current);
            break;

        case 5:
            printf("Oslobadanje memorije...\n");
            freeDirectory(root);
            printf("Izlaz...\n");
            return 0;

        default:
            printf("Neispravan odabir!\n");
        }
    }

    return 0;
}


/* ---------------------- FUNKCIJE ---------------------- */

// kreira novi direktorij (èvor)
Directory* createDirectory(const char* name) {
    Directory* newDir = (Directory*)malloc(sizeof(Directory));

    if (!newDir) return NULL;

    strcpy(newDir->name, name);
    newDir->child = NULL;
    newDir->sibling = NULL;
    newDir->parent = NULL;

    return newDir;
}

// md - dodaje novi direktorij u trenutni
int makeDirectory(Directory* current, const char* name) {

    Directory* newDir = createDirectory(name);
    if (!newDir) return 0;

    newDir->parent = current;

    // ako nema djece, ovo je prvo
    if (current->child == NULL) {
        current->child = newDir;
        return 1;
    }

    // inaèe ga dodaj kao zadnjeg brata
    Directory* temp = current->child;
    while (temp->sibling != NULL)
        temp = temp->sibling;

    temp->sibling = newDir;

    return 1;
}

// cd dir - idi u poddirektorij
Directory* changeDirectory(Directory* current, const char* name) {
    Directory* temp = current->child;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->sibling;
    }

    printf("Direktorij '%s' ne postoji!\n", name);
    return current;
}

// cd.. - povratak u roditelja
Directory* goBack(Directory* current) {
    if (current->parent != NULL)
        return current->parent;

    printf("Vec ste u root direktoriju!\n");
    return current;
}

// dir - ispis poddirektorija
int listDirectory(Directory* current) {

    Directory* temp = current->child;

    if (temp == NULL) {
        printf("Direktorij je prazan.\n");
        return 0;
    }

    printf("Sadrzaj direktorija:\n");
    while (temp != NULL) {
        printf(" - %s\n", temp->name);
        temp = temp->sibling;
    }

    return 1;
}

// rekurzivno oslobaða memoriju cijelog stabla
int freeDirectory(Directory* dir) {

    if (dir == NULL)
        return 0;

    // obriši dijete
    freeDirectory(dir->child);

    // obriši braæu
    freeDirectory(dir->sibling);

    // zatim vlastiti èvor
    free(dir);

    return 1;
}
