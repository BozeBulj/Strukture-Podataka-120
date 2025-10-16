/*Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char ime[15];
	char prezime[15];
	int bod;
}student;

int ispis(student*, int, float);

int main() {

	int n=0;
	char ime1[50];
	char prez[50];
	int bodovi;
	float maxBod = 60.0;


	FILE* f = fopen("nesto.txt", "r");
	if (f == NULL) {
		printf("GReska: file ne postoji");
		return -1;
	}

	while (fscanf(f, "%s %s %d", ime1, prez, &bodovi) == 3) { //brojac redaka
		n++;
	}
	rewind(f);

	student* s = (student*)malloc(n * sizeof(student));

	for (int i = 0; i < n; i++) {
		fscanf(f, "%s %s %d", s[i].ime, s[i].prezime, &s[i].bod);

		if (s[i].bod > 60) {
			printf("greska, bodovi nemogu bit veci od 60, ispravite u fileu");
			return -1;
		}
	}

	ispis(s, n, maxBod);
	free(s);
	return 0;
}

int ispis(student *x,int k, float max) {

	int i;
	float relBod=0;
	for (i = 0; i < k; i++) {
		relBod= ((float)x[i].bod / max) * 100.0;
		printf("%s %s %d %.2f\n", x[i].ime, x[i].prezime, x[i].bod, relBod);
	}
	return 0;
}
	
	