#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "common_func.h"
#include "customer_func.h"
#include "manager_func.h"

//struct
typedef struct {
	int yr, mth, dy, hr, min, sec;
}TIME;

typedef struct {
	char ID[7], passw[18], name[31];
}MANAGER;

typedef struct {
	TIME t;
	int ATM, accNo, txnCode;
	double amt, prevBal, newBal;
}BASEINFO;

typedef struct {
	char accNo[7], PIN[7];
	char name[23], gender, adds[26], state[12], hp[12];
	double bal;
	TIME lastTrans;
}CUSTOMER;

typedef struct {
	BASEINFO base;
	char bankCode[9];
	int cheqNo;
}CHEQUE;

typedef struct {
	BASEINFO base;
	int recAccNo, recPrevBal, recNewBal;
}TRANSFER;

int main() {
	printf("\n\tINITIALIZING...\n");

	//variables
	int entryCount = 0, nSel = -2, nAttempt = 0, nATM = 0;
	char chSel = 'N';
	CUSTOMER *cust;
	MANAGER *mnger;

	//file pointers
	FILE *custINF = fopen("Customer.txt", "r");
	FILE *mngerINF = fopen("Manager.txt", "r");

	//compute the number of customers inside the "Customer.txt"
	entryCount = countEntry(custINF);

	//declare a dynamic allocated CUSTOMER struct array
	cust = new CUSTOMER[entryCount];

	for (int i = 0; i < entryCount; i++) {

		//create struct pointer to replace customer[i] and customer[i].lastTrans
		CUSTOMER *tmp = &cust[i];
		TIME *tmp2 = &cust[i].lastTrans;

		//obtain the customers' details and store into "cust"
		fscanf(custINF, "%[^|]|%[^|]|%[^\t]%*[^|]|%c|%[^\t]%*[^|]|%[^\t]%*[^|]|%[^|]|%lf %d-%d-%d %d:%d:%d\n", 
			tmp->accNo, tmp->PIN, tmp->name, &tmp->gender, tmp->adds, tmp->state, tmp->hp, &tmp->bal,
			&tmp2->yr, &tmp2->mth, &tmp2->dy, &tmp2->hr, &tmp2->min, &tmp2->sec);
		//printf("%s\n%s\n%s\n%c\n%s\n%s\n%s\n%.2lf\n%d-%d-%d %d:%d:%d\n\n", 
		//	tmp->accNo, tmp->PIN, tmp->name, tmp->gender, tmp->adds, tmp->state, tmp->hp, tmp->bal,
		//	tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec);
	}

	//compute the number of customers inside the "Manager.txt"
	entryCount = countEntry(mngerINF);

	//declare a dynamic allocated MANAGER struct array
	mnger = new MANAGER[entryCount];

	for (int i = 0; i < entryCount; i++) {

		//create struct pointer to replace mnger[i]
		MANAGER *tmp = &mnger[i];

		//obtain the managers' details and store into "mnger"
		fscanf(mngerINF, "%[^|]|%[^\t]%*[^|]|%[^\n]\n", tmp->ID, tmp->passw, tmp->name);
		//printf("%s\n%s\n%s\n\n", tmp->ID, tmp->passw, tmp->name);
	}

	try {
		do { //main menu loop starts
			printIntro("MAIN MENU", "", 0);
			printf("\n\t1 -> CUSTOMER MENU\n\t2 -> MANAGER MENU\n");
			printBreak();
			nSel = validIpt(-1, 2);
			switch (nSel) {
			case 1: //customer menu
				do { //customer menu loop starts
					printIntro("MAIN MENU > CUSTOMER MENU", "", 0);
					printf("\n\t1 -> DEPOSITS\n\t2 -> WITHDRAWALS/TRANSFERS\n");
					printBreak();
					nSel = validIpt(-1, 2);
					switch (nSel) {
					case 1: //deposits subsystem
						nATM = randomATM(1);
						do { //deposits subsystem loop starts
							printIntro("MAIN MENU > CUSTOMER MENU > DEPOSITS SUBSYSTEM", "", nATM);
							printf("\n\t1 -> CASH DEPOSITS\n\t2 -> CHEQUE DEPOSITS\n");
							printBreak();
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //cash deposits
							case 2: //cheque deposits
								continue;
							case 0: //back
								break;
							case -1: //exit
								chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
								if (chSel == 'Y') throw - 1;
								continue;
							}
						} while (nSel != 0); //deposits subsystem loop ends
						reset(&chSel, &nSel, &nAttempt, &nATM);
						continue;
					case 2: //withdrawals/transfers subsystem
						nATM = randomATM(2);
						do { //withdrawals/transfers subsystem loop starts
							printIntro("MAIN MENU > CUSTOMER MENU > WITHDRAWALS/TRANSFERS SUBSYSTEM", "", nATM);
							printf("\n\t1 -> CASH WITHDRAWAL\n\t2 -> CASH/FUNDS TRANSFER\n");
							printBreak();
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //cash withdrawal
							case 2: //cash/funds transfer
								continue;
							case 0: //back
								break;
							case -1: //exit
								chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
								if (chSel == 'Y') throw - 1;
								continue;
							}
						} while (nSel != 0); //withdrawals/transfers subsystem loop ends
						reset(&chSel, &nSel, &nAttempt, &nATM);
						continue;
					case 0: //back
						break;
					case -1 : //exit
						chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
						if (chSel == 'Y') throw - 1;
						continue;
					}
				} while (nSel != 0); //customer menu loop ends
				reset(&chSel, &nSel, &nAttempt, &nATM);
				continue;
			case 2: //manager menu
				do { //manager menu loop starts
					printIntro("MAIN MENU > MANAGER MENU", "", 0);
					printf("\n\t1 -> PRINT TRANSACTION LOGS\n\t2 -> PERFORM UPDATES\n");
					printBreak();
					nSel = validIpt(-1, 2);
					switch (nSel) {
					case 1: //trans logs subsystem
						do { //trans logs loop starts
							printIntro("MAIN MENU > MANAGER MENU > PRINT TRANSACTION LOGS", "", 0);
							printf("\n\t1 -> CASH DEPOSIT LOG\n\t2 -> CHEQUE DEPOSIT LOG\n\t3 -> WITHDRAWAL LOG\n\t4 -> FUNDS TRANSFER LOG\n");
							printBreak();
							nSel = validIpt(-1, 4);
							switch (nSel) {
							case 1: //cash deposit log
							case 2: //cheque deposit log
							case 3: //withdrawal log
							case 4: //funds transfer log
								continue;
							case 0: //back
								break;
							case -1: //exit
								chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
								if (chSel == 'Y') throw - 1;
								continue;
							}
						} while (nSel != 0); //trans logs loop ends
						reset(&chSel, &nSel, &nAttempt, &nATM);
						continue;
					case 2: //perform updates subsystem
						do {
							printIntro("MAIN MENU > MANAGER MENU > PERFORM UPDATES", "", 0);
							printf("\n\t1 -> CHEQUE CLEARING\n\t2 -> HIGH WITHDRAWALS ALERT\n");
							printBreak();
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //cheque clearing
							case 2: //high withdrawals alert
								continue;
							case 0: //back
								break;
							case -1: //exit
								chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
								if (chSel == 'Y') throw - 1;
								continue;
							}
						} while (nSel != 0);
						reset(&chSel, &nSel, &nAttempt, &nATM);
						continue;
					case 0: //back
						break;
					case -1: //exit
						chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
						if (chSel == 'Y') throw - 1;
						continue;
					}
				} while (nSel != 0); //manager menu loop ends
				reset(&chSel, &nSel, &nAttempt, &nATM);
				continue;
			case 0: //exit
			case -1: //exit
				chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
				if (chSel == 'Y') throw -1;
				continue;
			};
		} while (1); //main menu loop ends
	}

	catch(int) {
		printExit("THANK YOU FOR CHOOSING US\n    HAVE A NICE DAY");
	}

	//free the memory
	delete[] cust;

	//close the files
	fclose(custINF);
	return 0;
}