#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

//replace fflush(stdin)
inline void discard_junk() {
	char c;
	while ((c = getchar()) != '\n'&&c != EOF);
}

//reset the value
void reset(char *chSel, int *nSel, int *nAttempt, int *nATM) {
	*chSel = 'N';
	*nSel = -2;
	*nAttempt = 0;
	*nATM = 0;
}

//compute the number of entries inside a file
int countEntry(FILE *buf) {
	int count = 0;
	while (fscanf(buf, "%*[^\n]\n") != EOF)
		count++;
	rewind(buf);
	return count;
}

//print the time on a console with a fixed format
void printTime(char c) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	printf("%04d-%02d-%02d %02d:%02d:%02d\n%c", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, c);
}

//print the time on a file with a fixed format
void printTime(FILE *buf) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	fprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

//print the header
void printIntro(char* dir, char* name, int atm) {
	system("cls");
	printf("\n    BANKING SYSTEM    ");
	(atm != 0) ? printf("ATM %02d     ", atm) : printf("ATM n/a    ");
	printTime('\n');
	printf("    %s\n\n    WELCOME %s\n\n", dir, name);
}

//print exit screen
void printExit(char* msg, char* exp) {
	system("cls");
	printf("\n    BANKING SYSTEM    ATM n/a    ");
	printTime('\n');
	printf("    EXIT CODE : %s\n\n    %s\n\n    <SYSTEM TERMINATED>\n\n", exp, msg);
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

//user log-in screen