#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

#include "struct.h"

//replace fflush(stdin)
inline void discard_junk() {
	char c;
	while ((c = getchar()) != '\n'&&c != EOF);
}

//sustain the screen from being clear
void readKey() {
	printf("\n  PRESS ANY KEY TO CONTINUE...\n");
	system("pause>nul");
}

//reset the value
void reset(char *chSel, int *nSel, int *nATM) {
	*chSel = 'N';
	*nSel = -2;
	*nATM = 0;
}

//print the time on a console with a fixed format : c can only be ' ' or '\n'
void printTime(char c) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	printf("%04d-%02d-%02d %02d:%02d:%02d\n%c", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, c);
}

//print the header : leave name and atm empty if there is no deposit or withdrawal customer function
void printHeader(char* dir, char *name, int atm) {
	system("cls");
	printf("\n  BANKING SYSTEM    ");
	(atm != 0) ? printf("ATM %02d     ", atm) : printf("ATM n/a    ");
	printTime('\n');
	printf("  %s\n\n", dir);
	if (strcmp(name, "") != 0)
		printf("  WELCOME - %s\n\n", name);
}

//print exit screen : exp is to determine exit code
void printExit(char* msg, char* exp) {
	system("cls");
	printf("\n  BANKING SYSTEM    ATM n/a    ");
	printTime('\n');
	printf("  EXIT CODE : %s\n\n  %s\n\n", exp, msg);
}

// print the fixed menu
void printBreak() {
	printf("\n\t0 -> BACK\n       -1 -> QUIT\n");
}

//accept, validate and return character input
char validIpt(char* msg) {
	char sel, c[2];
	int check;
	printf("\n  %s (Y = Yes, N = No) > ", msg);
	do {
		check = scanf(" %c%1[^\n]", &sel, c);

		//remove the junk inside the input buffer
		(check != 1) ? discard_junk() : 0;
	} while ((toupper(sel) != 'N'&&toupper(sel) != 'Y' || check != 1) && printf("\n  INVALID INPUT DETECTED\n  Please try again > "));

	//return the validated char input
	return toupper(sel);
}

//accept, validate and return integer input
int validIpt(int lLimit, int uLimit) {
	int ipt, check;
	char c;
	printf("\n  Your choice > ");
	do {
		check = scanf("%d%c", &ipt, &c);

		//remove the junk inside the input buffer
		(check != 2 || c != '\n') ? discard_junk() : 0;
	} while ((check != 2 || c != '\n' || ipt < lLimit || ipt > uLimit) && printf("\n  INVALID INPUT DETECTED\n  Please try again > "));

	//return the validated int input
	return ipt;
}

//generate random ATM number
int randomATM(int sel) {
	int atm = 0;
	srand(time(NULL));
	atm = rand() % 5 + ((sel == 1) ? 1 : 6);
	return atm;
}

//used in exception section only
void printError(int exp) {
	//runtime error
	if (exp == -11)
		printExit("EXCEPTION OCCURED - FAILED TO OPEN \"CUSTOMER.TXT\"", "E1");
	if (exp == -12)
		printExit("EXCEPTION OCCURED - FAILED TO OPEN \"MANAGER.TXT\"", "E2");
	if (exp == -13)
		printExit("EXCEPTION OCCURED - POSSIBLE DATA LOSS IN \"CUSTOMER\"", "E3");
	if (exp == -14)
		printExit("EXCEPTION OCCURED - POSSIBLE DATA LOSS IN \"MANAGET.TXT\"", "E4");
}

//read and set the password : set limit to 5 for pin. no. / 23 for passw
char* setPass(char* msg, int limit) {
	char passw[24], passw2[24], valid;
	do {
		printHeader("RESET PASSWORD", "", 0);
		valid = true;
		printf("\n  %s\n  NEW PASSWORD > ", msg);
		scanf("%[^\n]", passw);
		discard_junk();
		printf("\n  RE-ENTER NEW PASSWORD > ");
		scanf("%[^\n]", passw2);
		discard_junk();
		if (limit == 5) {
			for (int i = 0; i < 5; i++) {
				if (isdigit(passw[i]) == 0) {
					printf("\n  PIN. NO. MUST CONTAIN ONLY DIGIT NUMBERS\n");
					valid = false;
					break;
				}
			}
			if (passw[limit]!='\0') {
				printf("\n  PIN. NO. MUST CONTAIN EXACTLY 5 DIGIT NUMBERS\n");
				valid = false;
			}
		}
		else {
			for (int i = 0; i < 24; i++) {
				if (passw[i] != '\0')
					valid = false;
				else {
					valid = true;
					break;
				}
			}
			if (valid == false)
				printf("\n  NEW PASSWORD MUST CONTAIN ONLY AT MOST 23 CHARACTERS\n");
		}
		if (strcmp(passw, passw2) != 0) {
			valid = false;
			printf("\n  RE-ENTRED PASSWORD DOES NOT MATCH WITH THE PREVIOUS ONE\n");
		}
		if (valid == false) {
			printf("\n  PLEASE TRY AGAIN\n");
			readKey();
		}
	} while (valid == false);
	return passw2;
}

//file r/w functions--------------------------------------------------------

//compute the number of entries inside a file
int countEntry(FILE *buf) {
	int count = 0;
	while (fscanf(buf, "%*[^\n]\n") != EOF)
		count++;
	rewind(buf);
	return count;
}

//print the time on a file with a fixed format
void printTime(FILE *buf) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	fprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

//read customer.txt and store into a struct array : parameter -> tag is just to differentiate the overloaded functions which read different files
CUSTOMER* readF(FILE *buf, int *size, CUSTOMER *tag) {
	CUSTOMER *storage;

	//compute the number of customers inside the "Customer.txt"
	*size = countEntry(buf);

	//declare a dynamic allocated CUSTOMER struct array
	storage = new CUSTOMER[*size];
	for (int i = 0; i < *size; i++) {
		static int check = 0;

		//create struct pointer to replace customer[i] and customer[i].lastTrans
		CUSTOMER *tmp = &storage[i];
		TIME *tmp2 = &storage[i].lastTrans;

		//obtain the customers' details and store into "cust"
		check = fscanf(buf, "%[^|]|%[^|]|%[^_]%*[^|]|%c|%[^_]%*[^|]|%[^_]%*[^|]|%[^|]|%d|%lf %d-%d-%d %d:%d:%d\n",
			tmp->accNo, tmp->PIN, tmp->name, &tmp->gender, tmp->adds, tmp->state, tmp->hp, &tmp->lock, &tmp->bal,
			&tmp2->yr, &tmp2->mth, &tmp2->dy, &tmp2->hr, &tmp2->min, &tmp2->sec);
		printf("%s\n%s\n%s\n%c\n%s\n%s\n%s\n%d\n%.2lf\n%d-%d-%d %d:%d:%d\n\n", 
			tmp->accNo, tmp->PIN, tmp->name, tmp->gender, tmp->adds, tmp->state, tmp->hp, tmp->lock, tmp->bal,
			tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec);

		//if there is possible lost data, return the error code
		if (check != 15) throw - 13;
	}
	return storage;
}

//read manager.txt and store into a struct array : parameter -> tag is just to differentiate the overloaded functions which read different files
MANAGER* readF(FILE *buf, int *size, MANAGER *tag) {
	MANAGER *storage;

	//compute the number of customers inside the "Customer.txt"
	*size = countEntry(buf);

	//declare a dynamic allocated CUSTOMER struct array
	storage = new MANAGER[*size];
	for (int i = 0; i < *size; i++) {
		static int check = 0;

		//create struct pointer to replace mnger[i]
		MANAGER *tmp = &storage[i];

		//obtain the managers' details and store into "mnger"
		check = fscanf(buf, "%[^|]|%[^_]%*[^|]|%[^\n]\n", tmp->ID, tmp->passw, tmp->name);
		//printf("%s\n%s\n%s\n\n", tmp->ID, tmp->passw, tmp->name);

		//if there is possible lost data, return the error code
		if (check != 3) throw - 14;
	}
	return storage;
}

//write and update customer.txt
void writeF(FILE *buf, CUSTOMER *storage, int size) {
	for (int i = 0; i < size; i++) {
		CUSTOMER *tmp = &storage[i];
		TIME *tmp2 = &storage[i].lastTrans;
		if (tmp->lock < 3)
			tmp->lock = 0;
		fprintf(buf, "%5s|%5s|%-35s\t|%c|%-28s\t|%-13s\t|%s|%d|%08.2lf %02d-%02d-%04d %02d:%02d:%02d\n",
			tmp->accNo, tmp->PIN, strcat(tmp->name, "_"), tmp->gender, strcat(tmp->adds, "_"), strcat(tmp->state, "_"), tmp->hp, tmp->lock, tmp->bal,
			tmp2->dy, tmp2->mth, tmp2->yr, tmp2->hr, tmp2->min, tmp2->sec);
	}
}