
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Struktura za čvor stoga
typedef struct Node {
    double value;
    struct Node *next;
} Node;

// ---------------- DEKLARACIJE ----------------
Node* push(Node *top, double val);
Node* pop(Node *top, double *val);
double evaluatePostfix(char *filename);

// ---------------- MAIN ----------------
int main() {
    double result = evaluatePostfix("postfix.txt");
    printf("Rezultat: %.2f\n", result);
    return 0;
}

// ---------------- FUNKCIJE ----------------

// Gura broj na vrh stoga
Node* push(Node *top, double val) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = val;
    newNode->next = top;
    return newNode;
}

// Skida broj sa stoga
Node* pop(Node *top, double *val) {
    if (!top) {
        printf("Greška: stog je prazan!\n");
        exit(1);
    }
    *val = top->value;
    Node *temp = top;
    top = top->next;
    free(temp);
    return top;
}

// Računanje izraza u postfiks obliku
double evaluatePostfix(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Greška pri otvaranju datoteke!\n");
        exit(1);
    }

    Node *stack = NULL;
    char c;
    double a, b;

    while ((c = fgetc(f)) != EOF) {
        if (isdigit(c)) {
            stack = push(stack, c - '0'); // pretvaranje znaka u broj
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            stack = pop(stack, &b);
            stack = pop(stack, &a);

            switch (c) {
                case '+': stack = push(stack, a + b); break;
                case '-': stack = push(stack, a - b); break;
                case '*': stack = push(stack, a * b); break;
                case '/': stack = push(stack, a / b); break;
            }
        }
        // preskače razmake i nove redove
    }

    fclose(f);
    double result;
    stack = pop(stack, &result);
    return result;
}
