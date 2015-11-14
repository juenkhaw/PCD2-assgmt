#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "common_func.h"
#include "customer_func.h"
#include "manager_func.h"
#include "struct.h"

int main() {
	printf("\n\tINITIALIZING...\n");

	//variables
	int custCount = 0, mngerCount = 0;
	int nSel = 0, nATM = 0;
	char chSel = 'N';
	char accNo[7], pinNo[18];
	CUSTOMER *cust;
	MANAGER *mnger;
	CUSTOMER *currCust;
	MANAGER *currMnger;

	//variables for deposits

	//variables for withdrawals

	//file pointers
	FILE *custINF = fopen("Customer.txt", "r");
	FILE *mngerINF = fopen("Manager.txt", "r");

	//compute the number of customers inside the "Customer.txt"
	custCount = countEntry(custINF);

	//declare a dynamic allocated CUSTOMER struct array
	cust = new CUSTOMER[custCount];

	for (int i = 0; i < custCount; i++) {

		//create struct pointer to replace customer[i] and customer[i].lastTrans
		CUSTOMER *tmp = &cust[i];
		TIME *tmp2 = &cust[i].lastTrans;

		//obtain the customers' details and store into "cust"
		fscanf(custINF, "%[^|]|%[^|]|%[^\t]%*[^|]|%c|%[^\t]%*[^|]|%[^\t]%*[^|]|%[^|]|%d|%lf %d-%d-%d %d:%d:%d\n", 
			tmp->accNo, tmp->PIN, tmp->name, &tmp->gender, tmp->adds, tmp->state, tmp->hp, &tmp->lock, &tmp->bal,
			&tmp2->yr, &tmp2->mth, &tmp2->dy, &tmp2->hr, &tmp2->min, &tmp2->sec);
		//printf("%s\n%s\n%s\n%c\n%s\n%s\n%s\n%d\n%.2lf\n%d-%d-%d %d:%d:%d\n\n", 
		//	tmp->accNo, tmp->PIN, tmp->name, tmp->gender, tmp->adds, tmp->state, tmp->hp, tmp->lock, tmp->bal,
		//	tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec);
	}

	//compute the number of customers inside the "Manager.txt"
	mngerCount = countEntry(mngerINF);

	//declare a dynamic allocated MANAGER struct array
	mnger = new MANAGER[mngerCount];

	for (int i = 0; i < mngerCount; i++) {

		//create struct pointer to replace mnger[i]
		MANAGER *tmp = &mnger[i];

		//obtain the managers' details and store into "mnger"
		fscanf(mngerINF, "%[^|]|%[^\t]%*[^|]|%[^\n]\n", tmp->ID, tmp->passw, tmp->name);
		//printf("%s\n%s\n%s\n\n", tmp->ID, tmp->passw, tmp->name);
	}

	try {
		do { //main menu loop starts
			printHeader("MAIN MENU", "", 0);
			printf("\n\t1 -> CUSTOMER MENU\n\t2 -> MANAGER MENU\n");
			printBreak();
			nSel = validIpt(-1, 2);
			switch (nSel) {
			case 1: //customer menu
				//customer log-in function

				//reset the current customer pointer to null
				currCust = nullptr;
				printHeader("LOG-IN : CUSTOMER", "", 0);
				printf("\t0 -> BACK\n\n    PLEASE ENTER YOUR ACC. NO. > ");
				scanf("%[^\n]", accNo);
				discard_junk();

				//the system will jump back to main menu if user key in "0"
				if (strcmp(accNo, "0") == 0)
					continue;

				//check for the existance of the acc. no. entered by user
				for (int i = 0; i < custCount; i++) {
					if (strcmp(accNo, cust[i].accNo) == 0) {
						currCust = &cust[i];
						break;
					}
				}

				//if the acc. no. not found, system shall prompt user for key in again
				if (currCust == nullptr) {
					printHeader("ERROR", "", 0);
					printf("  THE ACC. NO. YOU HAVE ENTERED HAS NOT FOUND/INVALID\n\n  ACC. NO. : %s\n\n  PLEASE TRY AGAIN\n", accNo);
					readKey();
					continue;
				}

				//if the acc. no. is valid, user is prompted for entering password
				do {
					printHeader("LOG-IN : CUSTOMER", "", 0);
					printf("\t0 -> BACK\n\n    PLEASE ENTER PIN. NO. FOR ACC. %s > ", accNo);
					scanf("%[^\n]", pinNo);
					discard_junk();
					if (strcmp(pinNo, "0") == 0)
						break;
					if (strcmp(pinNo, currCust->PIN) == 0)
						break;
					else {
						currCust->lock++;
						//if the user has been failed to log into the acc. for 3 times, the system shall lock the acc.
						if (currCust->lock == 3)
							throw - 1;
						printf("\n  INVALID PASSWORD\n  WARNING - YOU HAVE ONLY 3 ATTEMPTS TO LOG INTO YOUR ACC.\n"
							"  YOU HAVE %d ATTEMPT(S) LEFT\n", 3-currCust->lock);
						readKey();
					}
				} while (currCust->lock != 3);
				if (strcmp(pinNo, "0") == 0)
					continue;
				do { //customer menu loop starts
					printHeader("MAIN MENU > CUSTOMER MENU", currCust->name, 0);
					printf("\n\t1 -> DEPOSITS\n\t2 -> WITHDRAWALS/TRANSFERS\n");
					printBreak();
					nSel = validIpt(-1, 2);
					switch (nSel) {
					case 1: //deposits subsystem
						nATM = randomATM(1);
						do { //deposits subsystem loop starts
							printHeader("MAIN MENU > CUSTOMER MENU > DEPOSITS SUBSYSTEM", currCust->name, nATM);
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
								if (chSel == 'Y') throw 0;
								continue;
							}
						} while (nSel != 0); //deposits subsystem loop ends
						reset(&chSel, &nSel, &nATM);
						continue;
					case 2: //withdrawals/transfers subsystem
						nATM = randomATM(2);
						do { //withdrawals/transfers subsystem loop starts
							printHeader("MAIN MENU > CUSTOMER MENU > WITHDRAWALS/TRANSFERS SUBSYSTEM", currCust->name, nATM);
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
								if (chSel == 'Y') throw 0;
								continue;
							}
						} while (nSel != 0); //withdrawals/transfers subsystem loop ends
						reset(&chSel, &nSel, &nATM);
						continue;
					case 0: //back
						break;
					case -1 : //exit
						chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
						if (chSel == 'Y') throw 0;
						continue;
					}
				} while (nSel != 0); //customer menu loop ends
				reset(&chSel, &nSel, &nATM);
				continue;
			case 2: //manager menu
				//manager log-in function

				//reset the current manager pointer to null
				currMnger = nullptr;
				printHeader("LOG-IN : MANAGER", "", 0);
				printf("\t0 -> BACK\n\n    PLEASE ENTER YOUR ID. NO. > ");
				scanf("%[^\n]", accNo);
				discard_junk();

				//back to the main menu if user has entered "0"
				if (strcmp(accNo, "0") == 0)
					continue;
				for (int i = 0; i < mngerCount; i++) {
					if (strcmp(accNo, mnger[i].ID) == 0) {
						currMnger = &mnger[i];
						break;
					}
				}

				//if ID. no. is invalid, user is prompted to enter again
				if (currMnger == nullptr) {
					printHeader("ERROR", "", 0);
					printf("  THE ID. NO. YOU HAVE ENTERED HAS NOT FOUND/INVALID\n\n  ACC. NO. : %s\n\n  PLEASE TRY AGAIN\n", accNo);
					readKey();
					continue;
				}

				//if the ID. no. is valid, user is required to enter password.
				do {
					printf("    PLEASE ENTER PASSW. > ");
					scanf("%[^\n]", pinNo);
					discard_junk();
					if (strcmp(pinNo, "0") == 0)
						break;

					//compare the passsword entered and the exact password
					if (strcmp(pinNo, currMnger->passw) == 0)
						break;
				} while (printf("\n  INVALID PASSWORD - PLEASE TRY AGAIN\n"));
				if (strcmp(pinNo, "0") == 0)
					continue;
				do { //manager menu loop starts
					printHeader("MAIN MENU > MANAGER MENU", "", 0);
					printf("\n\t1 -> PRINT TRANSACTION LOGS\n\t2 -> PERFORM UPDATES\n");
					printBreak();
					nSel = validIpt(-1, 2);
					switch (nSel) {
					case 1: //trans logs subsystem
						do { //trans logs loop starts
							printHeader("MAIN MENU > MANAGER MENU > PRINT TRANSACTION LOGS", "", 0);
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
								if (chSel == 'Y') throw 0;
								continue;
							}
						} while (nSel != 0); //trans logs loop ends
						reset(&chSel, &nSel, &nATM);
						continue;
					case 2: //perform updates subsystem
						do {
							printHeader("MAIN MENU > MANAGER MENU > PERFORM UPDATES", "", 0);
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
								if (chSel == 'Y') throw 0;
								continue;
							}
						} while (nSel != 0);
						reset(&chSel, &nSel, &nATM);
						continue;
					case 0: //back
						break;
					case -1: //exit
						chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
						if (chSel == 'Y') throw 0;
						continue;
					}
				} while (nSel != 0); //manager menu loop ends
				reset(&chSel, &nSel, &nATM);
				continue;
			case 0: //exit
			case -1: //exit
				chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
				if (chSel == 'Y') throw 0;
				continue;
			};
		} while (1); //main menu loop ends
	}

	catch(int exception) {
		if (exception == 0) {
			printExit("THANK YOU FOR CHOOSING US\n  HAVE A NICE DAY", "0");
		}
		if (exception == -1) {
			printExit("SORRY! YOU HAVE FAILED TO LOG INTO THE ACC. WITHIN 3 ATTEMPTS", "-1");
			printf("  THE ACC. BELOW HAS LOCKED FOR SECURITY CONCERN\n\n\t  ACC. NO. : %s\n\n"
				"  PLEASE CONTACT A QUALIFIED MANAGER/STAFF IN ORDER TO UNLOCK THE ACC. ABOVE\n\n", accNo);
		}
	}
	catch (...) {
		printExit("AN UNEXPECTED EXCEPTION OCCURRED", "n/a");
	}

	printf("\t\t<SYSTEM TERMINATED>\n\n");

	//free the memory
	delete[] cust;
	delete[] mnger;

	//close the files
	fclose(custINF);
	fclose(mngerINF);
	
	return 0;
}