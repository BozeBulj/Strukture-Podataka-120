#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Struktura za èlan polinoma
typedef struct Node {
    int coeff;          // koeficijent
    int exp;            // eksponent
    struct Node* next;  // pokazivaè na sljedeæi èlan
} Node;

// ------------------ DEKLARACIJE ------------------
Node* insert(Node* head, int coeff, int exp);
Node* readPoly(FILE* f);
Node* addPoly(Node* p1, Node* p2);
Node* multPoly(Node* p1, Node* p2);
void printPoly(Node* p);
void freePoly(Node* p);

// ------------------ MAIN ------------------
int main() {
    FILE* f = fopen("poli.txt", "r");
    if (!f) {
        printf("Greška pri otvaranju datoteke!\n");
        return 1;
    }

    Node* p1 = readPoly(f);  // prvi polinom
    Node* p2 = readPoly(f);  // drugi polinom
    fclose(f);

    printf("Polinom 1: ");
    printPoly(p1);
    printf("Polinom 2: ");
    printPoly(p2);

    Node* sum = addPoly(p1, p2);
    printf("\nZbroj: ");
    printPoly(sum);

    Node* prod = multPoly(p1, p2);
    printf("\nUmnožak: ");
    printPoly(prod);

    freePoly(p1);
    freePoly(p2);
    freePoly(sum);
    freePoly(prod);
    return 0;
}

// ------------------ FUNKCIJE ------------------

// Dodaje novi èlan u polinom (sortirano po eksponentu)
Node* insert(Node* head, int coeff, int exp) {
    if (coeff == 0) return head;

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->coeff = coeff;
    newNode->exp = exp;
    newNode->next = NULL;

    if (!head || exp > head->exp) {
        newNode->next = head;
        return newNode;
    }

    Node* curr = head;
    while (curr->next && curr->next->exp > exp)
        curr = curr->next;

    if (curr->next && curr->next->exp == exp) {
        curr->next->coeff += coeff;
        free(newNode);
    }
    else {
        newNode->next = curr->next;
        curr->next = newNode;
    }
    return head;
}

// Èita jedan polinom iz datoteke (do kraja reda)
Node* readPoly(FILE* f) {
    Node* head = NULL;
    int coeff, exp;
    char c;

    while (fscanf(f, "%d %d", &coeff, &exp) == 2) {
        head = insert(head, coeff, exp);
        c = fgetc(f);
        if (c == '\n' || c == EOF) break; // kraj reda znaèi kraj polinoma
        ungetc(c, f);
    }
    return head;
}

// Zbrajanje dvaju polinoma
Node* addPoly(Node* p1, Node* p2) {
    Node* res = NULL;
    while (p1) {
        res = insert(res, p1->coeff, p1->exp);
        p1 = p1->next;
    }
    while (p2) {
        res = insert(res, p2->coeff, p2->exp);
        p2 = p2->next;
    }
    return res;
}

// Množenje dvaju polinoma
Node* multPoly(Node* p1, Node* p2) {
    Node* res = NULL;
    for (Node* a = p1; a; a = a->next)
        for (Node* b = p2; b; b = b->next)
            res = insert(res, a->coeff * b->coeff, a->exp + b->exp);
    return res;
}

// Ispis polinoma
void printPoly(Node* p) {
    if (!p) {
        printf("0\n");
        return;
    }
    while (p) {
        printf("%d", p->coeff);
        if (p->exp != 0)
            printf("x^%d", p->exp);
        if (p->next && p->next->coeff > 0)
            printf(" + ");
        else if (p->next)
            printf(" ");
        p = p->next;
    }
    printf("\n");
}

// Oslobaðanje memorije
void freePoly(Node* p) {
    Node* tmp;
    while (p) {
        tmp = p;
        p = p->next;
        free(tmp);
    }
}
