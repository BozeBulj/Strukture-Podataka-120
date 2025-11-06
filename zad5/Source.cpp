
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Struktura èvora stoga
typedef struct Node {
    double value;          // Vrijednost na stogu
    struct Node* next;     // Pokazivaè na sljedeæi element
} Node;

// ====== Deklaracije funkcija ======
Node* push(Node* top, double value);          // Dodaje vrijednost na vrh stoga
Node* pop(Node* top, double* value);          // Uklanja vrh stoga i vraæa ga kroz *value
int isOperator(char c);                       // Provjerava je li znak operator
double calculate(double a, double b, char op);// Izvršava osnovne aritmetièke operacije
double evaluatePostfix(const char* expression); // Raèuna vrijednost postfiks izraza
char* readFromFile(const char* filename);     // Uèitava cijeli izraz iz datoteke


// ====== Glavna funkcija ======
int main() {
    char* expression = readFromFile("postfix.txt");
    if (expression == NULL) {
        printf("Error reading file.\n");
        return 1;
    }

    printf("Postfix expression: %s\n", expression);

    double result = evaluatePostfix(expression);
    printf("Result = %.2f\n", result);

    free(expression);
    return 0;
}


// ====== Definicije funkcija ======

// ------------------------------------------------------------
// Dodaje novi element na vrh stoga
// ------------------------------------------------------------
Node* push(Node* top, double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    newNode->value = value;
    newNode->next = top;
    return newNode; // Novi èvor postaje vrh stoga
}

// ------------------------------------------------------------
// Skida element s vrha stoga i vraæa njegovu vrijednost
// ------------------------------------------------------------
Node* pop(Node* top, double* value) {
    if (top == NULL) {
        printf("Error: Stack underflow!\n");
        exit(1);
    }
    *value = top->value;
    Node* temp = top;
    top = top->next;
    free(temp);
    return top;
}

// ------------------------------------------------------------
// Provjerava je li znak operator
// ------------------------------------------------------------
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// ------------------------------------------------------------
// Izvršava osnovnu aritmetièku operaciju izmeðu a i b
// ------------------------------------------------------------
double calculate(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) {
            printf("Error: Division by zero!\n");
            exit(1);
        }
        return a / b;
    default:
        printf("Unknown operator '%c'\n", op);
        exit(1);
    }
}

// ------------------------------------------------------------
// Raèuna vrijednost izraza u postfiks obliku
// ------------------------------------------------------------
double evaluatePostfix(const char* expression) {
    Node* stack = NULL;
    char token[50];
    int i = 0, j = 0;

    while (expression[i] != '\0') {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        // Ako je broj
        if (isdigit(expression[i]) || (expression[i] == '-' && isdigit(expression[i + 1]))) {
            j = 0;
            // èitamo cijeli broj (ili negativan)
            while (isdigit(expression[i]) || expression[i] == '.' || expression[i] == '-') {
                token[j++] = expression[i++];
            }
            token[j] = '\0';
            double num = atof(token);
            stack = push(stack, num);
        }
        // Ako je operator
        else if (isOperator(expression[i])) {
            double b, a;
            stack = pop(stack, &b);
            stack = pop(stack, &a);
            double result = calculate(a, b, expression[i]);
            stack = push(stack, result);
            i++;
        }
        else {
            printf("Invalid character: %c\n", expression[i]);
            exit(1);
        }
    }

    double finalResult;
    stack = pop(stack, &finalResult);

    // Ako ima još elemenata na stogu -> izraz nije ispravan
    if (stack != NULL) {
        printf("Error: Invalid postfix expression!\n");
        exit(1);
    }

    return finalResult;
}

// ------------------------------------------------------------
// Èita cijeli postfiks izraz iz datoteke u string
// ------------------------------------------------------------
char* readFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    char* buffer = (char*)malloc(256 * sizeof(char));
    if (!buffer) return NULL;

    fgets(buffer, 256, file);
    fclose(file);
    return buffer;
}
