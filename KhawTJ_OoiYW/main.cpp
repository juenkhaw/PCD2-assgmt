#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "common_func.h"
#include "customer_func.h"
#include "manager_func.h"
#include "struct.h"

int main() {
	printf("\n\tINITIALIZING...\n");

	//core variables
	int custCount = 0, mngerCount = 0;
	int nSel = 0, nATM = 0;
	char chSel = 'N';
	char accNo[7], pinNo[18];
	CUSTOMER *cust = 0;
	MANAGER *mnger = 0;
	CUSTOMER *currCust;
	MANAGER *currMnger;

	//core file pointers
	FILE *custINF;
	FILE *mngerINF;
	FILE *test = fopen("test.txt", "w+");

	//variables for deposits

	//variables for withdrawals
	FILE *wdOUTF = fopen("Withdrawals.dat", "a+");
	FILE *transOUTF = fopen("Transfers.dat", "a+");

	printTransLog(transOUTF);

	try { //start to seek for the runtime error and point to terminate the system

		//check whether the files have opened or not
		custINF = fopen("Customer.txt", "r+");
		if (!custINF) throw - 11;
		mngerINF = fopen("Manager.txt", "r");
		if (!mngerINF) throw - 12;

		//initializing process
		//read and store the data from "Customer.txt"
		cust = readF(custINF, &custCount, cust);

		//read and store the data from "Manager.txt"
		mnger = readF(mngerINF, &mngerCount, mnger);

		//main processing loop
		do { //main menu loop starts
			printHeader("MAIN MENU", "", 0);
			printBreak();
			printf("\n\t1 -> CUSTOMER MENU\n\t2 -> MANAGER MENU\n\n\t  SELECT > ");
			nSel = validIpt(-1, 2);
			switch (nSel) {
			case 1: //customer menu

				//customer log-in function
				//reset the current customer pointer to null : currCust ptr is point to the customer who has logged in to his acc.
				currCust = nullptr;
				printHeader("LOG-IN : CUSTOMER", "", 0);
				printf("\n\t0 -> BACK\n\n    PLEASE ENTER YOUR ACC. NO. > ");
				scanf("%[^\n]", accNo);
				discard_junk();

				//the system will jump back to main menu if user key in "0"
				if (strcmp(accNo, "0") == 0)
					continue;

				//check for the existance of the acc. no. entered by user
				for (int i = 0; i < custCount; i++) {
					if (strcmp(accNo, cust[i].accNo) == 0) {

						//if the acc. no. entered by user does exist, the currCust ptr shall point to his acc.
						currCust = &cust[i];
						break;
					}
				}

				//if the acc. no. entered by user is not found, system shall prompt user for entering again
				if (currCust == nullptr) {
					printHeader("ERROR - ACC. NOT FOUND", "", 0);
					printf("  THE ACC. NO. YOU HAD ENTERED WAS NOT FOUND/INVALID\n\n  ACC. NO. : %s\n\n  PLEASE TRY AGAIN\n", accNo);
					readKey();
					continue;
				}

				//if the acc. no. is valid, user is prompted for entering password within only 3 attempts
				do {
					printHeader("LOG-IN : CUSTOMER", "", 0);
					printf("\n\t0 -> BACK\n\n    ACCOUNT OWNER : %s\n\n    PLEASE ENTER PIN. NO. > ", currCust->name);
					scanf("%[^\n]", pinNo);
					discard_junk();

					//if user has entered "0", back to main menu
					if (strcmp(pinNo, "0") == 0)
						break;

					//if the passw is matched, then system shall proceed to next page
					if (strcmp(pinNo, currCust->PIN) == 0)
						break;

					//if the wrong passw is entered, add 1 to attempts
					else {
						currCust->lock++;
						//if the user has been failed to log into the acc. for 3 times, the system shall lock the acc and terminate
						if (currCust->lock == 3)
							throw - 1;
						printf("\n  INVALID PASSWORD\n  WARNING - YOU HAVE ONLY 3 ATTEMPTS TO LOG INTO YOUR ACC.\n"
							"  YOU HAVE ONLY %d ATTEMPT(S) LEFT\n", 3 - currCust->lock);
						readKey();
					}
				} while (currCust->lock != 3);

				if (strcmp(pinNo, "0") == 0)
					continue;

				//if this is the customer's first time to log in his new acc.
				if (currCust->lastTrans.yr == 0) {
					strcpy(currCust->PIN, setPass("WELCOME, THIS IS YOUR FIRST TIME TO LOG INTO YOUR BANK ACC.\n"
						"  FOR SECURITY CONCERN, YOU ARE RECOMMENDED TO RESET YOUR PIN. NO.", 5, currCust));
					currCust->lastTrans = setTime();
					throw 1;
				}

				do { //customer menu loop starts
					printHeader("MAIN MENU > CUSTOMER MENU", currCust->name, 0);
					printBreak();
					printf("\n\t1 -> DEPOSITS\n\t2 -> WITHDRAWALS/TRANSFERS\n\n\t  SELECT > ");
					nSel = validIpt(-1, 2);
					switch (nSel) {
					case 1: //deposits subsystem
						nATM = randomATM(1);
						do { //deposits subsystem loop starts
							printHeader("MAIN MENU > CUSTOMER MENU > DEPOSITS SUBSYSTEM", currCust->name, nATM);
							printBreak();
							printf("\n\t1 -> CASH DEPOSITS\n\t2 -> CHEQUE DEPOSITS\n\n\t  SELECT > ");
							//get the user selection in typical range
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
							printBreak();
							printf("\n\t1 -> CASH WITHDRAWAL\n\t2 -> CASH/FUNDS TRANSFER\n\n\t  SELECT > ");
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //cash withdrawal
								wdFunc(wdOUTF, currCust, nATM);
								continue;
							case 2: //cash/funds transfer
								transFunc(transOUTF, currCust, cust, custCount, nATM);
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
					case -1: //exit
						chSel = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
						if (chSel == 'Y') throw 0;
						continue;
					}
				} while (nSel != 0); //customer menu loop ends
				reset(&chSel, &nSel, &nATM);
				continue;
			case 2: //manager menu

				//manager log-in function
				//reset the current manager pointer to null : currMnger is a ptr that pointed to the manager who has logged in
				currMnger = nullptr;
				printHeader("LOG-IN : MANAGER/STAFF", "", 0);
				printf("\n\t0 -> BACK\n\n    PLEASE ENTER YOUR ID. NO. > ");
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
					printHeader("ERROR - MANAGER/STAFF ENTRY NOT FOUND", "", 0);
					printf("  THE ID. NO. YOU HAD ENTERED WAS NOT FOUND/INVALID\n\n  ID. NO. : %s\n\n  PLEASE TRY AGAIN\n", accNo);
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
					printHeader("MAIN MENU > MANAGER MENU", currMnger->name, 0);
					printBreak();
					printf("\n\t1 -> PRINT TRANSACTION LOGS\n\t2 -> PERFORM UPDATES\n\t3 -> ACCOUNTS MANAGEMENT\n\n\t  SELECT > ");
					nSel = validIpt(-1, 3);
					switch (nSel) {
					case 1: //trans logs subsystem
						do { //trans logs loop starts
							printHeader("MAIN MENU > MANAGER MENU > PRINT TRANSACTION LOGS", currMnger->name, 0);
							printBreak();
							printf("\n\t1 -> CASH DEPOSIT LOG\n\t2 -> CHEQUE DEPOSIT LOG\n\t3 -> WITHDRAWAL LOG\n\t4 -> FUNDS TRANSFER LOG\n\n\t  SELECT > ");
							nSel = validIpt(-1, 4);
							switch (nSel) {
							case 1: //cash deposit log
							case 2: //cheque deposit log
								continue;
							case 3: //withdrawal log
								printWdLog(wdOUTF);
								continue;
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
							printHeader("MAIN MENU > MANAGER MENU > PERFORM UPDATES", currMnger->name, 0);
							printBreak();
							printf("\n\t1 -> CHEQUE CLEARING\n\t2 -> HIGH WITHDRAWALS ALERT\n\n\t  SELECT > ");
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
					case 3:
						do {
							printHeader("MAIN MENU > MANAGER MENU > ACCOUNTS MANAGEMENT", currMnger->name, 0);
							printBreak();
							printf("\n\t1 -> UNLOCK ACCOUNT\n\t2 -> REGISTER FOR NEW CUSTOMER/STAFF\n\n\t  SELECT > ");
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //unlock account
								unlockAcc(cust, custCount);
								continue;
							case 2: //register for new customer/staff
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

	catch (int exception) { //once error or point of termination has triggered, the code below shall excecute immediately
		//point to termination
		if (exception == 0)
			printExit("THANK YOU FOR CHOOSING US\n  HAVE A NICE DAY", "0");
		if (exception == 1) {
			printExit("PIN. NO. HAS BEEN SUCCESFULLY RESET", "1");
			printf("\n  PLEASE RESTART THE SYSTEM IN ORDER TO LOG INTO YOUR BANK ACC.\n\n\t  ACC. NO. : %s\n\n", accNo);
		}
		if (exception == -1) {
			printExit("SORRY! YOU HAVE FAILED TO LOG INTO THE ACC. WITHIN 3 ATTEMPTS", "-1");
			printf("\n  THE ACC. BELOW HAS BEEN LOCKED DUE TO SECURITY CONCERN\n\n\t  ACC. NO. : %s\n\n"
				"  PLEASE CONTACT A QUALIFIED MANAGER/STAFF IN ORDER TO UNLOCK THE ACC. ABOVE\n\n", accNo);
		}
		//runtime error
		printError(exception);
	}
	catch (...) { //if any unknown error has occured
		printExit("AN UNEXPECTED EXCEPTION OCCURRED", "n/a");
	}

	writeF(test, cust, custCount);

	printf("\t\t<SYSTEM TERMINATED>\n\n");

	//free the memory
	delete[] cust;
	delete[] mnger;

	//close the files
	fclose(custINF);
	fclose(mngerINF);
	fclose(wdOUTF);
	fclose(transOUTF);
	fclose(test);
	
	return 0;
}