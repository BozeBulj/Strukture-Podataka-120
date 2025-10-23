/*Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
/*
return EXIT_FAILURE
return EXIT_SUCCESS
 
 
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
		printf("GReska: file ne postoji"); //otvorit file
		return -1;
	}

	while (fscanf(f, "%s %s %d", ime1, prez, &bodovi) == 3) { //brojac redaka
		n++;
	}
	rewind(f); //vraca se na pocetak filea

	student* s = (student*)malloc(n * sizeof(student)); //dinamicka alokacija

	for (int i = 0; i < n; i++) {
		fscanf(f, "%s %s %d", s[i].ime, s[i].prezime, &s[i].bod);

		if (s[i].bod > 60) {
			printf("greska, bodovi nemogu bit veci od 60, ispravite u fileu"); //ako ima vise od 60 bodova nemore pokrenit
			return -1;
		}
	}
	fclose(f);

	ispis(s, n, maxBod); //salje pokazivac na strukturu, broj redaka i maxBod koji su 60;
	
	free(s); //oslobada memoriju
	
	return 0;
}

int ispis(student *x,int k, float max) {

	int i;
	float relBod=0;
	for (i = 0; i < k; i++) {
		relBod= ((float)x[i].bod / max) * 100.0;
		printf("%s %s %d %.2f\n", x[i].ime, x[i].prezime, x[i].bod, relBod); //ispis svega
	}
	return 0;
}*/

/*2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/
	
struct St* pos; //pokazivac position OVO U NOVI FILE STAVIT
typedef struct St{
	char name[15];
	char lastN[15];
	int year;
	pos next;
}newperson;

int addBegin(); //dodaje na pocetku liste
int print(); //ispisuje
int addEnd(); //dodaje na kraj liste
int find_last_name(); //trazi po prezimenu
int Delete(); //brise

int main() {

	St head = { .name = "", .lastN = "", god = 0, next = NULL};

	newperson = { .name = "", .lastN = "", god = 0, next = NULL};
	q->next = newperson;
	newperson->next=q->next

	return 0;
}

int addBegin() {


	return 0;
}

/*
A) alokacija tipa pos
B)
*/