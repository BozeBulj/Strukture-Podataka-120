#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definiranje maksimalnih duljina za stringove
#define MAX_LINE_LENGTH 256
#define MAX_NAME_LENGTH 100
#define DATE_LENGTH 11 // YYYY-MM-DD\0

// --- Strukture vezanih listi (Item List) ---

// Struktura za pojedini artikl na raèunu
typedef struct ItemNode {
    char name[MAX_NAME_LENGTH]; // Naziv artikla
    int quantity;               // Kolièina
    double price;               // Cijena po jedinici
    struct ItemNode* next;
} ItemNode;

// Struktura za glavu vezane liste artikala
typedef struct ItemList {
    ItemNode* head;
} ItemList;

// --- Strukture vezanih listi (Bill List) ---

// Struktura za pojedini raèun (èvor vezane liste raèuna)
typedef struct BillNode {
    char date[DATE_LENGTH];     // Datum izdavanja raèuna (YYYY-MM-DD)
    ItemList items;             // Vezana lista artikala na ovom raèunu
    struct BillNode* next;
} BillNode;

// Struktura za glavu vezane liste raèuna
typedef struct BillList {
    BillNode* head;
} BillList;

// Struktura za pohranu rezultata upita
typedef struct QueryResult {
    double total_spent;
    int total_quantity;
} QueryResult;

// --- Prototipi Funkcija ---

// Inicijalizacija
bool initialize_bill_list(BillList* list); // Vraæa true/false za status
bool initialize_item_list(ItemList* list); // Vraæa true/false za status

// Umetanje u listu artikala (sortirano po nazivu)
ItemNode* create_item_node(const char* name, int quantity, double price);
bool insert_item_sorted(ItemList* list, ItemNode* new_node); // Vraæa true/false

// Umetanje u listu raèuna (sortirano po datumu)
BillNode* create_bill_node(const char* date);
bool insert_bill_sorted(BillList* list, BillNode* new_node); // Vraæa true/false

// Uèitavanje podataka iz datoteka
bool read_all_bills(BillList* list, const char* index_file_name);
bool read_single_bill(BillNode* bill_node, const char* file_name);

// Funkcija za obradu upita
QueryResult calculate_total_for_item(BillList* list, const char* item_name, const char* start_date, const char* end_date);
bool process_query(BillList* list); // Vraæa true/false ako je upit uspješno izvršen

// Pomoæne funkcije i èišæenje (èišæenje zadržano kao void jer je njegova jedina svrha side-effect)
bool is_date_between(const char* date, const char* start_date, const char* end_date);
void free_bill_list(BillList* list);
void free_item_list(ItemList* list);

// --- Glavna funkcija ---

int main() {
    BillList all_bills;
    if (!initialize_bill_list(&all_bills)) {
        fprintf(stderr, "Greska pri inicijalizaciji liste racuna.\n");
        return 1;
    }

    // Uèitavanje svih podataka iz racuni.txt i prateæih datoteka
    printf("Ucitavanje podataka...\n");
    if (!read_all_bills(&all_bills, "racuni.txt")) {
        fprintf(stderr, "Greska pri ucitavanju podataka. Izlazak iz programa.\n");
        free_bill_list(&all_bills);
        return 1;
    }
    printf("Ucitavanje zavrseno. Ukupno ucitanih racuna: %s\n", all_bills.head ? "prisutni" : "nema");

    // Obrada korisnièkog upita
    (void)process_query(&all_bills); // Ignoriramo povratnu vrijednost jer je to kraj programa

    // Oslobaðanje memorije
    free_bill_list(&all_bills);

    return 0;
}

// --- Implementacije Funkcija ---

/**
 * @brief Inicijalizira glavu vezane liste raèuna.
 * @param list Pokazivaè na BillList strukturu.
 * @return true ako je uspješno (uvijek u ovom sluèaju).
 */
bool initialize_bill_list(BillList* list) {
    list->head = NULL;
    return true;
}

/**
 * @brief Inicijalizira glavu vezane liste artikala.
 * @param list Pokazivaè na ItemList strukturu.
 * @return true ako je uspješno (uvijek u ovom sluèaju).
 */
bool initialize_item_list(ItemList* list) {
    list->head = NULL;
    return true;
}

// --- Funkcije za Listu Artikala ---

/**
 * @brief Kreira novi èvor artikla.
 * @param name Naziv artikla.
 * @param quantity Kolièina artikla.
 * @param price Cijena po jedinici.
 * @return Pokazivaè na novi ItemNode ili NULL ako alokacija nije uspjela.
 */
ItemNode* create_item_node(const char* name, int quantity, double price) {
    ItemNode* new_node = (ItemNode*)malloc(sizeof(ItemNode));
    if (new_node == NULL) {
        perror("Greska pri alokaciji ItemNode");
        return NULL;
    }
    strncpy(new_node->name, name, MAX_NAME_LENGTH - 1);
    new_node->name[MAX_NAME_LENGTH - 1] = '\0'; // Osiguraj null-terminaciju
    new_node->quantity = quantity;
    new_node->price = price;
    new_node->next = NULL;
    return new_node;
}

/**
 * @brief Umeæe èvor artikla u listu sortirano po nazivu (leksikografski).
 * @param list Pokazivaè na ItemList strukturu.
 * @param new_node Pokazivaè na èvor artikla za umetanje.
 * @return true ako je umetanje uspješno.
 */
bool insert_item_sorted(ItemList* list, ItemNode* new_node) {
    if (new_node == NULL) return false;

    // 1. Ako je lista prazna ili novi èvor ide na poèetak
    if (list->head == NULL || strcmp(new_node->name, list->head->name) < 0) {
        new_node->next = list->head;
        list->head = new_node;
        return true;
    }

    ItemNode* current = list->head;
    // 2. Traži mjesto za umetanje (dok je naziv tekuæeg manji ili jednak nazivu novog)
    while (current->next != NULL && strcmp(new_node->name, current->next->name) > 0) {
        current = current->next;
    }

    // 3. Umetni èvor
    new_node->next = current->next;
    current->next = new_node;
    return true;
}

// --- Funkcije za Listu Raèuna ---

/**
 * @brief Kreira novi èvor raèuna.
 * @param date Datum raèuna u formatu YYYY-MM-DD.
 * @return Pokazivaè na novi BillNode ili NULL ako alokacija nije uspjela.
 */
BillNode* create_bill_node(const char* date) {
    BillNode* new_node = (BillNode*)malloc(sizeof(BillNode));
    if (new_node == NULL) {
        perror("Greska pri alokaciji BillNode");
        return NULL;
    }
    strncpy(new_node->date, date, DATE_LENGTH - 1);
    new_node->date[DATE_LENGTH - 1] = '\0';
    (void)initialize_item_list(&new_node->items); // Inicijalizacija podliste
    new_node->next = NULL;
    return new_node;
}

/**
 * @brief Umeæe èvor raèuna u listu sortirano po datumu (YYYY-MM-DD).
 * @param list Pokazivaè na BillList strukturu.
 * @param new_node Pokazivaè na èvor raèuna za umetanje.
 * @return true ako je umetanje uspješno.
 */
bool insert_bill_sorted(BillList* list, BillNode* new_node) {
    if (new_node == NULL) return false;

    // 1. Ako je lista prazna ili novi èvor ide na poèetak
    if (list->head == NULL || strcmp(new_node->date, list->head->date) < 0) {
        new_node->next = list->head;
        list->head = new_node;
        return true;
    }

    BillNode* current = list->head;
    // 2. Traži mjesto za umetanje
    while (current->next != NULL && strcmp(new_node->date, current->next->date) > 0) {
        current = current->next;
    }

    // 3. Umetni èvor
    new_node->next = current->next;
    current->next = new_node;
    return true;
}

// --- Funkcije za Uèitavanje Podataka ---

/**
 * @brief Èita sve nazive datoteka raèuna iz index_file_name i uèitava svaki raèun.
 * @param list Pokazivaè na glavnu BillList strukturu.
 * @param index_file_name Naziv datoteke koja sadrži popis raèuna (npr. racuni.txt).
 * @return true ako je èitanje datoteke uspješno i ako je barem jedan raèun pokušan za èitanje.
 */
bool read_all_bills(BillList* list, const char* index_file_name) {
    FILE* index_file = fopen(index_file_name, "r");
    if (index_file == NULL) {
        perror("Greska pri otvaranju racuni.txt");
        return false;
    }

    char file_name[MAX_LINE_LENGTH];
    bool files_processed = false;

    while (fgets(file_name, MAX_LINE_LENGTH, index_file) != NULL) {
        files_processed = true;
        // Ukloni znak za novi red ('\n') ako postoji
        file_name[strcspn(file_name, "\n")] = 0;

        // Privremeni èvor raèuna, datum se popunjava u read_single_bill
        BillNode temp_bill;
        (void)initialize_item_list(&temp_bill.items);

        if (read_single_bill(&temp_bill, file_name)) {
            // Ako je èitanje uspješno, kreiraj pravi èvor i umetni u glavnu listu
            BillNode* new_bill_node = create_bill_node(temp_bill.date);
            if (new_bill_node != NULL) {
                // Premjesti listu artikala
                new_bill_node->items.head = temp_bill.items.head;
                // Umetni u sortiranu listu raèuna
                (void)insert_bill_sorted(list, new_bill_node);
            }
            else {
                // Oslobaðanje u sluèaju greške alokacije BillNode
                free_item_list(&temp_bill.items);
            }
        }
    }

    fclose(index_file);
    return files_processed; // Vraæa true ako je racuni.txt uspješno otvoren i obraðen
}

/**
 * @brief Èita pojedinu datoteku raèuna, popunjava datum i listu artikala.
 * @param bill_node Pokazivaè na BillNode strukturu za popunjavanje.
 * @param file_name Naziv datoteke raèuna.
 * @return true ako je èitanje i parsiranje datuma uspješno.
 */
bool read_single_bill(BillNode* bill_node, const char* file_name) {
    FILE* bill_file = fopen(file_name, "r");
    if (bill_file == NULL) {
        fprintf(stderr, "Upozorenje: Datoteka racuna '%s' nije pronadjena/otvorena.\n", file_name);
        return false;
    }

    char line[MAX_LINE_LENGTH];

    // 1. Èitanje datuma (prvi red)
    if (fgets(line, MAX_LINE_LENGTH, bill_file) == NULL) {
        fprintf(stderr, "Greska: Datoteka '%s' je prazna.\n", file_name);
        fclose(bill_file);
        return false;
    }
    // Ukloni znak za novi red
    line[strcspn(line, "\n")] = 0;

    // Provjeri format datuma
    if (strlen(line) != DATE_LENGTH - 1) { // Oèekuje 10 znakova datuma
        fprintf(stderr, "Upozorenje: Nepravilan format datuma ('%s') u datoteci '%s'. Preskacem.\n", line, file_name);
        fclose(bill_file);
        return false;
    }

    strncpy(bill_node->date, line, DATE_LENGTH - 1);
    bill_node->date[DATE_LENGTH - 1] = '\0';


    // 2. Èitanje artikala (ostali redovi)
    while (fgets(line, MAX_LINE_LENGTH, bill_file) != NULL) {
        char name_buffer[MAX_NAME_LENGTH];
        int quantity;
        double price;

        // Oznaka gdje poèinje parsiranje
        char* rest = line;

        // Parsiranje - Naziv, Kolièina, Cijena
        char* token = strtok_r(rest, ",", &rest); // Ime
        if (token == NULL) continue;
        strncpy(name_buffer, token, MAX_NAME_LENGTH - 1);
        name_buffer[MAX_NAME_LENGTH - 1] = '\0';

        token = strtok_r(NULL, ",", &rest); // Kolièina
        if (token == NULL || sscanf(token, "%d", &quantity) != 1) continue;

        token = strtok_r(NULL, ",", &rest); // Cijena
        if (token == NULL || sscanf(token, "%lf", &price) != 1) continue;

        // Uklanjanje razmaka oko naziva za precizniju usporedbu (opcionalno)
        // ... (dodatna logika za trimovanje)

        ItemNode* new_item = create_item_node(name_buffer, quantity, price);
        if (new_item != NULL) {
            (void)insert_item_sorted(&bill_node->items, new_item);
        }
    }

    fclose(bill_file);
    return true;
}

// --- Funkcije za Upit ---

/**
 * @brief Pomaže u provjeri da li je datum unutar zadanog vremenskog raspona [start_date, end_date].
 * @param date Datum za provjeru (YYYY-MM-DD).
 * @param start_date Poèetni datum raspona (YYYY-MM-DD).
 * @param end_date Krajnji datum raspona (YYYY-MM-DD).
 * @return true ako je datum unutar raspona, false inaèe.
 */
bool is_date_between(const char* date, const char* start_date, const char* end_date) {
    // strcmp radi leksikografsku usporedbu koja je valjana za format YYYY-MM-DD
    // date >= start_date AND date <= end_date
    return (strcmp(date, start_date) >= 0) && (strcmp(date, end_date) <= 0);
}

/**
 * @brief Izraèunava ukupnu potrošnju i kolièinu za specifièni artikl unutar zadanog vremenskog razdoblja.
 * @param list Pokazivaè na BillList.
 * @param item_name Naziv artikla koji se traži.
 * @param start_date Poèetni datum raspona (YYYY-MM-DD).
 * @param end_date Krajnji datum raspona (YYYY-MM-DD).
 * @return QueryResult strukturu koja sadrži ukupnu potrošnju i kolièinu.
 */
QueryResult calculate_total_for_item(BillList* list, const char* item_name, const char* start_date, const char* end_date) {
    QueryResult result = { 0.0, 0 };

    BillNode* current_bill = list->head;
    while (current_bill != NULL) {
        // 1. Provjeri da li je raèun unutar zadanog vremenskog raspona
        if (is_date_between(current_bill->date, start_date, end_date)) {
            // 2. Traži artikl unutar liste artikala tog raèuna
            ItemNode* current_item = current_bill->items.head;
            while (current_item != NULL) {
                // Provjeri da li se naziv artikla podudara
                if (strcmp(current_item->name, item_name) == 0) {
                    // Dodaj potrošnju i kolièinu
                    result.total_spent += (double)current_item->quantity * current_item->price;
                    result.total_quantity += current_item->quantity;
                }
                current_item = current_item->next;
            }
        }
        current_bill = current_bill->next;
    }
    return result;
}

/**
 * @brief Interaktivna funkcija za obradu korisnièkog upita.
 * @param list Pokazivaè na BillList.
 * @return true ako su svi ulazni podaci pravilno uneseni.
 */
bool process_query(BillList* list) {
    char item_name[MAX_NAME_LENGTH];
    char start_date[DATE_LENGTH];
    char end_date[DATE_LENGTH];

    printf("\n--- Upit o potrosnji ---\n");

    // Unos naziva artikla
    printf("Unesite naziv artikla (max %d karaktera): ", MAX_NAME_LENGTH - 1);
    if (fgets(item_name, MAX_NAME_LENGTH, stdin) == NULL) return false;
    item_name[strcspn(item_name, "\n")] = 0;

    // Unos poèetnog datuma
    printf("Unesite POCETNI datum (YYYY-MM-DD): ");
    if (fgets(start_date, DATE_LENGTH, stdin) == NULL) return false;
    start_date[strcspn(start_date, "\n")] = 0;

    // Unos krajnjeg datuma
    printf("Unesite KRAJNJI datum (YYYY-MM-DD): ");
    if (fgets(end_date, DATE_LENGTH, stdin) == NULL) return false;
    end_date[strcspn(end_date, "\n")] = 0;

    // Provjera unosa datuma (pojednostavljena)
    if (strlen(start_date) != 10 || strlen(end_date) != 10) {
        printf("Greska: Datum mora biti u formatu YYYY-MM-DD (10 znakova).\n");
        return false;
    }

    QueryResult result = calculate_total_for_item(list, item_name, start_date, end_date);

    printf("\n--- Rezultat Upita ---\n");
    printf("Artikl: %s\n", item_name);
    printf("Razdoblje: od %s do %s\n", start_date, end_date);
    printf("Ukupno potroseno: %.2f\n", result.total_spent);
    printf("Ukupna kolicina kupljena: %d\n", result.total_quantity);
    printf("------------------------\n");

    return true;
}

// --- Funkcije za èišæenje (Zadržane kao void jer je oslobaðanje resursa side-effect) ---

/**
 * @brief Oslobaða memoriju zauzetu vezanom listom artikala.
 * @param list Pokazivaè na ItemList strukturu.
 */
void free_item_list(ItemList* list) {
    ItemNode* current = list->head;
    ItemNode* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    list->head = NULL;
}

/**
 * @brief Oslobaða memoriju zauzetu vezanom listom raèuna (ukljuèujuæi ugniježðene liste artikala).
 * @param list Pokazivaè na BillList strukturu.
 */
void free_bill_list(BillList* list) {
    BillNode* current = list->head;
    BillNode* next_node;
    while (current != NULL) {
        next_node = current->next;
        free_item_list(&current->items); // Prvo oslobodi ugniježðenu listu
        free(current);
        current = next_node;
    }
    list->head = NULL;
}