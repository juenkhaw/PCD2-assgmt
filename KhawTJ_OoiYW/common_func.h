#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sstream>
#include <iostream>

#include "struct.h"

//replace fflush(stdin)
inline void discard_junk() {
	//grab all the chars that are remained inside the buffer
	char c;
	while ((c = getchar()) != '\n'&&c != EOF);
}

using namespace std;

//functions with templates are served as a pattern for creating other similar functions to avoid duplication and numerous of overloaded functions

//accept any datatype of values and return it as a form of C-style string
template<typename T>
inline const char* toString(T tX) {
	//stream is set up to accept a variety of sources input
	ostringstream os;
	os << tX;
	//convert the buffer into string
	string buf = os.str();
	//return as a C-style string
	return buf.c_str();
}

//accept any datatype of values and return true if the user wished to back
template<typename T>
inline bool confirmBack(T tX) {
	ostringstream os;
	os << tX;
	string buf = os.str();
	//return true if the user wished to back by key in "0"
	return (buf.compare("0") == 0) ? true : false;
}

//input validating function--------------------------------------------------------

//accept, validate and return character input
char validIpt(char* msg) {
	char sel, c[2];
	int check;
	printf("\n  %s (Y = Yes, N = No) > ", msg);
	do {
		//reset the value
		sel = c[0] = '\0';
		check = scanf(" %c%1[^\n]", &sel, c);

		//remove the junk inside the input buffer
		(check != 1) ? discard_junk() : 0;

		//continue if the char input is not 'N', 'Y' and invalid
	} while ((toupper(sel) != 'Y'&&toupper(sel) != 'N' || check != 1) && printf("\n  INVALID INPUT DETECTED\n  PLEASE TRY AGAIN > "));

	discard_junk();
	//return the validated char input
	return toupper(sel);
}

//accept, validate and return integer input
int validIpt(int lwrLimit, int uprLimit) {
	int ipt, check;
	char c;
	do {
		//reset the value
		c = '\0';
		check = scanf("%d%c", &ipt, &c);

		//remove the junk inside the input buffer
		(check != 2 || c != '\n') ? discard_junk() : 0;

		//continue if the int input is not within the range specified and invalid
	} while ((check != 2 || c != '\n' || ipt < lwrLimit || ipt > uprLimit) && printf("\n  INVALID INPUT DETECTED\n  PLEASE TRY AGAIN > "));

	//return the valid int input
	return ipt;
}

//accept, validate and return double input
double validIpt() {
	double ipt;
	int check;
	char c;
	do {
		//reset the value
		c = '\0';
		check = scanf("%lf%c", &ipt, &c);

		//remove the junk inside the input buffer
		(check != 2 || c != '\n') ? discard_junk() : 0;

		//continue if the double input is invalid
	} while ((check != 2 || c != '\n') && printf("\n  INVALID INPUT DETECTED\n  PLEASE TRY AGAIN > "));

	//return the valid double input
	return ipt;
}

//main menu functions----------------------------------------------------------

//sustain the screen from being cleared
void readKey() {
	printf("\n  PRESS ANY KEY TO CONTINUE...\n");
	system("pause>nul");
}

//print the time on a console with a fixed format
void printTime() {
	SYSTEMTIME t;
	GetLocalTime(&t);
	printf("%04d-%02d-%02d %02d:%02d:%02d\n", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

//print the time structure onto the console screen
void printTime(TIME t) {
	printf("%04d-%02d-%02d %02d:%02d:%02d", t.yr, t.mth, t.dy, t.hr, t.min, t.sec);
}

//set the system time into time structure (memory)
TIME setTime() {
	TIME buf;
	SYSTEMTIME t;
	GetLocalTime(&t);
	buf.yr = t.wYear;
	buf.mth = t.wMonth;
	buf.dy = t.wDay;
	buf.hr = t.wHour;
	buf.min = t.wMinute;
	buf.sec = t.wSecond;
	return buf;
}

//print the header : leave name and ATM empty if there is no deposit or withdrawal customer function
void printHeader(char* dir, char *name, int nATM) {
	system("cls");
	printf("\n  TARBANK BANKING SYSTEM    ");
	(nATM != 0) ? printf("ATM %02d     ", nATM) : printf("ATM n/a    ");
	printTime();
	if (strcmp(name, "") != 0)
		printf("  WELCOME, %s\n", name);
	printf("  %s\n", dir);
	for (int i = 0; i < 79; i++) printf("=");
	printf("\n");
}

//print exit screen : exception is to determine exit code
void printExit(char* msg, char* exception) {
	system("cls");
	printf("\n  TARBANK BANKING SYSTEM    ATM n/a    ");
	printTime();
	printf("  EXIT CODE : %s\n\n  %s\n", exception, msg);
	for (int i = 0; i < 78; i++) printf("=");
	printf("\n");
}

// print the fixed menu
void printBreak() {
	printf("\n\t0 -> BACK\n       -1 -> QUIT\n");
}

//generate random ATM number
int randomATM(int nSel) {
	int atm = 0;
	srand(time(NULL));

	//generate random number : 1 to 5 for deposit / 6 to 10 for withdrawal
	atm = rand() % 5 + ((nSel == 1) ? 1 : 6);
	return atm;
}

//confirmation of quiting the system
void confirmBreak(char *chSel) {
	*chSel = 'N';
	*chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");

	//throw 0 to jump out from the main processing loop directly
	if (*chSel == 'Y') throw 0;
}

//commonly-used functions--------------------------------------------

//accept and validate the password : customer version
char* readPassw(CUSTOMER *currCust) {
	char passw[7];
	do {
		//reset the value
		passw[0] = '\0';
		scanf(" %6[^\n]", passw);
		//if user tried to back, return 0
		if (strcmp("0", passw) == 0) return "0";
		//if the pin. no. is matched and valid, return it
		if (strcmp(currCust->PIN, passw) == 0) return passw;
		//else the user will be warned about the attempts left for him/her to try again
		else {
			currCust->lock++;
			if (currCust->lock == 3) throw - 1;
			printf("\n  INVALID PIN. NO.\n  WARNING - YOU HAVE ONLY %d ATTEMPT(S) LEFT\n  PLEASE TRY AGAIN > ", 3 - currCust->lock);
		}
	} while (1);
}

//accept and validate the password : manager version
char* readPassw(MANAGER *currMnger) {
	char passw[25];
	do {
		//reset the value
		passw[0] = '\0';
		scanf(" %24[^\n]", passw);
		//if user tried to back, return 0
		if (strcmp("0", passw) == 0) return "0";
		//if the password is matched and valid, return it
		if (strcmp(currMnger->passw, passw) == 0) return passw;
		//else the user will be prompted for trying again
		else
			printf("\n  INVALID PASSWORD - PLEASE TRY AGAIN > ");
	} while (1);
}

//read and set the password : set limit to 5 for pin. no. / 23 for pass w
char* setPassw(char* msg, int passwLength, CUSTOMER *currCust, MANAGER *currMnger) {
	char passw[25], passw2[25], valid;
	do {
		//reset the value
		passw[0] = passw2[0] = '\0';
		valid = true;

		if (passwLength == 5) printHeader("RESET PIN. NO.", "",  0);
		else printHeader("RESET PASSWORD", "",  0);

		//prompt user to enter and re-enter new password
		if (passwLength == 5)
			printf("\n  %s\n\n\t  ACC. NO. : %s\n\n  NEW PIN. NO. > ", msg, currCust->accNo);
		else
			printf("\n  %s\n\n\t  ID. NO. : %s\n\n  NEW PASSWORD > ", msg, currMnger->ID);
		scanf(" %24[^\n]", passw);
		discard_junk();
		printf("\n  RE-ENTER NEW %s > ", (passwLength==5)?"PIN. NO.":"PASSWORD");
		scanf(" %24[^\n]", passw2);
		discard_junk();

		//validation for pin. no.
		if (passwLength == 5) {
			for (int i = 0; i < 5; i++) {

				//if pin. no. contains non-digit character
				if (isdigit(passw[i]) == 0) {
					printf("\n  PIN. NO. MUST CONTAIN ONLY DIGIT NUMBERS\n");
					valid = false;
					break;
				}
			}

			//if pin. no. does not contain exactly 5 digit characters
			if (passw[passwLength]!='\0') {
				printf("\n  PIN. NO. MUST CONTAIN EXACTLY 5 DIGIT NUMBERS\n");
				valid = false;
			}
		}

		//validation of password
		else {

			//if the password entered contains more than 23 characters
			for (int i = 0; i < 24; i++) {
				if (passw[i] == '\0' && i < 5) {
					valid = false;
					break;
				}
				if (passw[i] != '\0')
					valid = false;
				else {
					valid = true;
					break;
				}
			}
			if (valid == false)
				printf("\n  NEW PASSWORD MUST CONTAIN ONLY AT MOST 23 CHARACTERS\n  AND AT LEAST 6 CHARACTERS\n\n");
		}

		//if password and re-entered password do not match to each other
		if (strcmp(passw, passw2) != 0) {
			valid = false;
			printf("\n  RE-ENTRED %s DID NOT MATCH WITH THE PREVIOUS ONE\n", (passwLength==5)?"PIN. NO.":"PASSWORD");
		}

		//user is prompted to try again when the password/pin. no. is invalid
		if (valid == false) {
			printf("\n  PLEASE TRY AGAIN\n");
			readKey();
		}
	} while (valid == false);

	//otherwise returns the valid password
	return passw2;
}

//seek for a customer from the customer list
CUSTOMER *findAcc(CUSTOMER *cust, char *accNo, int custCount, int nATM) {
	//the system will jump back to main menu if user key in "0"
	if (strcmp(accNo, "0") == 0) return nullptr;

	//check for the existence of the account no. entered by the user
	for (int i = 0; i < custCount; i++) {
		if (strcmp(accNo, cust[i].accNo) == 0) {
			//if the account no. entered by user does exist, the currCust pointer shall point to his account
			return &cust[i];	
		}
	}

	//if the account no. entered by the user is not found, system will prompt the user for entering again
	printHeader("ERROR - ACC. NOT FOUND", "", nATM);
	printf("\n  THE ACC. NO. YOU HAD ENTERED WAS NOT FOUND/INVALID\n\n  ACC. NO. : %s\n\n  PLEASE TRY AGAIN\n", accNo);
	readKey();
	return nullptr;
}

//seek for a manager from the manager list
MANAGER *findAcc(MANAGER *mnger, char *id, int mngerCount) {
	MANAGER *currMnger = nullptr;

	//jump back to the main menu if user key in "0"
	if (strcmp(id, "0") == 0) return nullptr;

	//check for the existence of the id. no. entered by the user
	for (int i = 0; i < mngerCount; i++) {
		if (strcmp(id, mnger[i].ID) == 0) {
			currMnger = &mnger[i];
			break;
		}
	}

	//if the id. no. entered by the user is not found, system will prompt the user for entering again
	if (currMnger == nullptr) {
		printHeader("ERROR - ID. NOT FOUND", "", 0);
		printf("\n  THE ID. NO. YOU HAD ENTERED WAS NOT FOUND/INVALID\n\n  ID. NO. : %s\n\n  PLEASE TRY AGAIN\n", id);
		readKey();
		return nullptr;
	}
	return currMnger;
}

//file r/w functions--------------------------------------------------------

//compute the number of entries inside a file
int countEntry(FILE *buf) {
	int count = 0;

	//skip through every chars on a line until the pointer reach the '\n'
	while (fscanf(buf, "%*[^\n]\n") != EOF)
		count++;

	//reset the pointer back to the beginning of the file
	rewind(buf);
	return count;
}

//print the time on a file with a fixed format
void printTime(FILE *buf) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	fprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

//read customer.txt and store into a structure array : parameter -> tag is just to differentiate the overloaded functions which read different files
CUSTOMER* readF(FILE *custINF, int *custCount, CUSTOMER *tag) {
	int check;
	CUSTOMER *storage;

	//compute the number of customers inside the "Customer.txt"
	*custCount = countEntry(custINF);

	//declare a dynamic allocated CUSTOMER structure array
	storage = new CUSTOMER[*custCount];
	for (int i = 0; i < *custCount; i++) {
		check = 0;

		//create structure pointer to replace customer[i] and customer[i].lastTrans
		CUSTOMER *tmp = &storage[i];
		TIME *tmp2 = &storage[i].lastTrans;

		//obtain the customers' details and store into the structure
		check = fscanf(custINF, "%[^|]|%[^|]|%[^_]%*[^|]|%c|%[^_]%*[^|]|%[^_]%*[^|]|%[^|]|%d|%lf %d-%d-%d %d:%d:%d\n",
			tmp->accNo, tmp->PIN, tmp->name, &tmp->gender, tmp->adds, tmp->state, tmp->hp, &tmp->lock, &tmp->bal,
			&tmp2->dy, &tmp2->mth, &tmp2->yr, &tmp2->hr, &tmp2->min, &tmp2->sec);
		//printf("%s\n%s\n%s\n%c\n%s\n%s\n%s\n%d\n%.2lf\n%d-%d-%d %d:%d:%d\n\n", 
		//	tmp->accNo, tmp->PIN, tmp->name, tmp->gender, tmp->adds, tmp->state, tmp->hp, tmp->lock, tmp->bal,
		//	tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec);

		//if there is possible lost data, return the error code
		if (check != 15) throw - 13;
	}
	return storage;
}

//read manager.txt and store into a structure array : parameter -> tag is just to differentiate the overloaded functions which read different files
MANAGER* readF(FILE *mngerINF, int *mngerCount, MANAGER *tag) {
	int check;
	MANAGER *storage;

	//compute the number of customers inside the "Customer.txt"
	*mngerCount = countEntry(mngerINF);

	//declare a dynamic allocated CUSTOMER structure array
	storage = new MANAGER[*mngerCount];
	for (int i = 0; i < *mngerCount; i++) {
		check = 0;

		//create structure pointer to replace mnger[i]
		MANAGER *tmp = &storage[i];

		//obtain the managers' details and store into the structure
		check = fscanf(mngerINF, "%[^|]|%[^_]%*[^|]|%[^\n]\n", tmp->ID, tmp->passw, tmp->name);
		//printf("%s\n%s\n%s\n\n", tmp->ID, tmp->passw, tmp->name);

		//if there is possible lost data, return the error code
		if (check != 3) throw - 14;
	}
	//return the dynamic structure array with data
	return storage;
}

//read withdrawal records and store into a dynamic structure array that is going to be returned
BASEINFO* readF(FILE *wdOUTF, int *count, BASEINFO *tag) {
	BASEINFO *storage = nullptr, buf;
	int check;
	rewind(wdOUTF);

	//compute the number of entry and store it on a dynamic structure array
	while (fread(&buf, sizeof(BASEINFO), 1, wdOUTF) != 0)
		(*count)++;
	rewind(wdOUTF);

	//create a temporary dynamic structure array to store the deposit/withdrawal info
	storage = new BASEINFO[*count];
	for (int i = 0; i < *count; i++) {
		check = fread(&storage[i], sizeof(BASEINFO), 1, wdOUTF);

		//if there is possible lost of data occurred
		if (check != 1) throw - 15;
	}
	//return the dynamic structure array with info
	return storage;
}

//read transfer records and store into a dynamic structure array that is going to be returned
TRANSFER* readF(FILE *transOUTF, int *count, TRANSFER *tag) {
	TRANSFER *storage = nullptr, buf;
	int check;
	rewind(transOUTF);

	//compute the number of entry and store it on a dynamic structure array
	while (fread(&buf, sizeof(TRANSFER), 1, transOUTF) != 0)
		(*count)++;
	rewind(transOUTF);

	//create a temporary dynamic structure array to store the transfers info
	storage = new TRANSFER[*count];
	for (int i = 0; i < *count; i++) {
		check = fread(&storage[i], sizeof(TRANSFER), 1, transOUTF);
		
		//if there is possible lost of data occurred
		if (check != 1) throw - 16;
	}
	//return the dynamic structure array with data
	return storage;
}

//write and update customer.txt
void writeF(FILE *custINF, CUSTOMER *cust, int custCount) {
	rewind(custINF);
	for (int i = 0; i < custCount; i++) {

		//create temporary pointer for address of structure below
		CUSTOMER *tmp = &cust[i];
		TIME *tmp2 = &cust[i].lastTrans;

		//if the attempt to log in is not above 3, reset it back to 0
		if (tmp->lock < 3)
			tmp->lock = 0;

		//update the data into the destination file
		fprintf(custINF, "%5s|%5s|%-36s|%c|%-29s|%-14s|%s|%d|%010.2lf %02d-%02d-%04d %02d:%02d:%02d\n",
			tmp->accNo, tmp->PIN, strcat(tmp->name, "_"), tmp->gender, strcat(tmp->adds, "_"), strcat(tmp->state, "_"), tmp->hp, tmp->lock, tmp->bal,
			tmp2->dy, tmp2->mth, tmp2->yr, tmp2->hr, tmp2->min, tmp2->sec);
	}
}

//write and update manager.txt
void writeF(FILE *mngerINF, MANAGER *mnger, int mngerCount) {
	rewind(mngerINF);
	for (int i = 0; i < mngerCount; i++) {
		MANAGER *tmp = &mnger[i];
		fprintf(mngerINF, "%s|%-25s|%s\n", tmp->ID, strcat(tmp->passw, "_"), tmp->name);
	}
}

//used in exception section only
void printError(int exception) {
	//runtime error
	if (exception == -11)
		printExit("EXCEPTION OCCURED - FAILED TO OPEN \"CUSTOMER.TXT\"", "E1");
	if (exception == -12)
		printExit("EXCEPTION OCCURED - FAILED TO OPEN \"MANAGER.TXT\"", "E2");
	if (exception == -13)
		printExit("EXCEPTION OCCURED - POSSIBLE DATA LOSS IN \"CUSTOMER\"", "E3");
	if (exception == -14)
		printExit("EXCEPTION OCCURED - POSSIBLE DATA LOSS IN \"MANAGET.TXT\"", "E4");
	if (exception == -15)
		printExit("EXCEPTION OCCURED - POSSIBLE DATA LOSS IN \"WITHDRAWALS.DAT\"", "E5");
	if (exception == -16)
		printExit("EXCEPTION OCCURED - POSSIBLE DATA LOSS IN \"TRANSFERS.DAT\"", "E6");
}