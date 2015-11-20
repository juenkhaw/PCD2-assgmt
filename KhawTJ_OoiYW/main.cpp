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
	char accNo[7], pinNo[25];
	CUSTOMER *cust = 0;
	MANAGER *mnger = 0;
	CUSTOMER *currCust;
	MANAGER *currMnger;

	//core file pointers
	FILE *custINF;
	FILE *mngerINF;
	FILE *test = fopen("test.txt", "w+");
	FILE *test2 = fopen("test2.txt", "w+");

	//variables for deposits
	FILE *cashdp = fopen("CashDeposits.dat", "a+");
	FILE *chequedp = fopen("ChequeDeposits.dat", "a+");

	//variables for withdrawals
	FILE *wdOUTF = fopen("Withdrawals.dat", "a+b");
	FILE *transOUTF = fopen("Transfers.dat", "a+b");

	try { //start to seek for the runtime error and point to terminate the system

		//check whether the files have opened or not
		custINF = fopen("Customer.txt", "r+");
		if (!custINF) throw - 11;
		mngerINF = fopen("Manager.txt", "r");
		if (!mngerINF) throw - 12;

		//initializing process
		//read and store the data from the txt or dat files
		cust = readF(custINF, &custCount, cust);
		mnger = readF(mngerINF, &mngerCount, mnger);

		//main processing loop
		do { //main menu starts
			chSel = 'N';
			nSel = 0;
			accNo[0] = pinNo[0] = '\0';
			printHeader("MAIN MENU", "", 0);
			printf("\n\t0 -> EXIT\n\n\t1 -> CUSTOMER MENU\n\t2 -> MANAGER MENU\n\n\tSELECT > ");
			nSel = validIpt(0, 2);
			switch (nSel) {
			case 1: //customer menu

				//log in function for customer
				currCust = nullptr;
				printHeader("LOG-IN - CUSTOMER", "", 0);

				//prompt for customer acc no.
				printf("\n\t 0 -> BACK\n\n  PLEASE ENTER YOUR ACC. NO. > ");
				scanf(" %6[^\n]", accNo);
				discard_junk();

				//search for the acc no. in the customer list
				currCust = findAcc(cust, accNo, custCount, 0);

				//if the acc is not found -> back
				if (currCust == nullptr) break;

				//if the acc is locked -> quit
				if (currCust->lock == 3) throw - 2;

				//prompt for the pin no. for only 3 times : exit if the user used up 3 attempts
				printf("\n\tACCOUNT OWNER : %s\n\n  WARNING - YOU HAVE ONLY BEEN GIVEN 3 ATTEMPTS\n  PLEASE KEY IN YOUR PIN. NO. > ", currCust->name);
				strcpy(pinNo, readPassw(currCust));
				if (strcmp("0", pinNo) == 0) break;

				//prompt for new password if the acc is being logged in for the first time
				if (currCust->lastTrans.yr == 0) {
					strcpy(currCust->PIN, setPassw("WELCOME, THIS IS YOUR FIRST TIME TO LOG INTO YOUR BANK ACC.\n"
						"  FOR SECURITY CONCERN, YOU ARE RECOMMENDED TO RESET YOUR PIN. NO.", 5, currCust, nullptr));
					currCust->lastTrans = setTime();
					throw 1;
				}
				do { //customer menu starts
					printHeader("MAIN MENU > CUSTOMER MENU", currCust->name, 0);
					printBreak();
					printf("\n\t1 -> DEPOSITS\n\t2 -> WITHDRAWALS/TRANSFERS\n\t3 -> CHECK ACCOUNT BALANCE\n\t4 -> CHANGE PIN. NO.\n\n\t  SELECT > ");
					nSel = validIpt(-1, 4);
					switch (nSel) {
					case 1: //deposit subsystem
						nATM = randomATM(nSel);
						printHeader("MAIN MENU > CUSTOMER MENU > DEPOSITS SUBSYSTEM", currCust->name, nATM);
						printBreak();
						printf("\n\t1 -> CASH DEPOSITS\n\t2 -> CHEQUE DEPOSITS\n\n\t  SELECT > ");
						nSel = validIpt(-1, 2);
						switch (nSel) { //deposits subsystem starts
						case 1: //cash deposits
							break;
						case 2: //cheque deposits
							break;
						case 0: //back
							break;
						case -1: //quit
							confirmBreak(&chSel);
							break;
						} while (nSel != 0); //deposits subsystem ends
						nSel = -2;
						break;
					case 2: //withdrawal subsystem
						nATM = randomATM(nSel);
						do { //withdrawal subsystem starts
							printHeader("MAIN MENU > CUSTOMER MENU > WITHDRAWALS/TRANSFERS SUBSYSTEM", currCust->name, nATM);
							printBreak();
							printf("\n\t1 -> CASH WITHDRAWAL\n\t2 -> CASH/FUNDS TRANSFER\n\n\t  SELECT > ");
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //cash withdrawal
								wdFunc(wdOUTF, currCust, nATM);
								break;
							case 2: //cash/funds transfers
								transFunc(transOUTF, currCust, cust, custCount, nATM);
								break;
							case 0: //back
								break;
							case -1: //quit
								confirmBreak(&chSel);
								break;
							}
						} while (nSel != 0); //withdrawal subsystem ends
						nSel = -2;
						break;
					case 3: //check balance
						accDetails(currCust, wdOUTF, transOUTF, cashdp, chequedp);
						break;
					case 4: //change pin no.
						resetPassw(currCust);
						break;
					case 0: //back
						break;
					case -1: //quit
						confirmBreak(&chSel);
						break;
					}
				} while (nSel != 0); //customer menu ends
				nSel = -2;
				break;
			case 2://manager menu

				//log in function for manager
				currMnger = nullptr;
				printHeader("LOG IN - MANAGER", "", 0);

				//prompt for id no.
				printf("\n\t 0 -> BACK\n\n  PLEASE ENTER YOUR ID. NO. > ");
				scanf(" %6[^\n]", accNo);

				//search the id no received in the manager list
				currMnger = findAcc(mnger, accNo, mngerCount);

				//if it is not found -> back
				if (currMnger == nullptr) break;

				//prompt for password repeatly if incorrect password is detected
				printf("\n\tMANAGER/STAFF : %s\n\n  PLEASE KEY IN YOUR PASSWORD > ", currMnger->name);
				strcpy(pinNo, readPassw(currMnger));
				if (strcmp("0", pinNo) == 0) break;

				do { //manager menu starts
					printHeader("MAIN MENU > MANAGER MENU", currMnger->name, 0);
					printBreak();
					printf("\n\t1 -> PRINT TRANSACTION LOGS\n\t2 -> PERFORM UPDATES\n\t3 -> ACCOUNTS MANAGEMENT\n\t4 -> CHANGE PASSWORD\n\n\t  SELECT > ");
					nSel = validIpt(-1, 4);
					switch (nSel) {
					case 1: //print transaction logs
						do { //print transaction logs starts
							printHeader("MAIN MENU > MANAGER MENU > PRINT TRANSACTION LOGS", currMnger->name, 0);
							printBreak();
							printf("\n\t1 -> CASH DEPOSIT LOG\n\t2 -> CHEQUE DEPOSIT LOG\n\t3 -> WITHDRAWAL LOG\n\t4 -> FUNDS TRANSFER LOG\n\n\t  SELECT > ");
							nSel = validIpt(-1, 4);
							switch (nSel) {
							case 1: //cash deposit log
							case 2: //cheque deposit log
								break;
							case 3: //withdrawal log
								printWdLog(wdOUTF);
								break;
							case 4: //funds transfer log
								printTransLog(transOUTF);
								break;
							case 0: //back
								break;
							case -1: //quit
								confirmBreak(&chSel);
								break;
							}
						} while (nSel != 0); //print transaction logs ends
						nSel = -2;
						break;
					case 2: //perform updates
						do { //perform updates starts
							printHeader("MAIN MENU > MANAGER MENU > PERFORM UPDATES", currMnger->name, 0);
							printBreak();
							printf("\n\t1 -> CHEQUE CLEARING\n\t2 -> HIGH WITHDRAWALS ALERT\n\n\t  SELECT > ");
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //cheque clearings
							case 2: //high withdrawals alert
							case 0: //back
								break;
							case -1: //quit
								confirmBreak(&chSel);
								break;
							}
						} while (nSel != 0); //perform updates ends
						nSel = -2;
						break;
					case 3: //accounts management
						do { //accounts management starts
							printHeader("MAIN MENU > MANAGER MENU > ACCOUNTS MANAGEMENT", currMnger->name, 0);
							printBreak();
							printf("\n\t1 -> UNLOCK ACCOUNT\n\t2 -> REGISTER FOR NEW CUSTOMER/STAFF\n\n\t  SELECT > ");
							nSel = validIpt(-1, 2);
							switch (nSel) {
							case 1: //unlock acc
								unlockAcc(cust, custCount);
								break;
							case 2: //registration
							case 0: //back
								break;
							case -1: //quit
								confirmBreak(&chSel);
								break;
							}
						} while (nSel != 0); //account management ends
						nSel = -2;
						break;
					case 4: //change password
						resetPassw(currMnger);
						break;
					case 0: //back
						break;
					case -1: //quit
						confirmBreak(&chSel);
						break;
					}
				} while (nSel != 0); //mnager menu ends
				nSel = -2;
				break;
			case 0: //quit
				confirmBreak(&chSel);
				break;
			}
		} while (1); //main menu ends
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
			printExit("SORRY, YOU HAVE FAILED TO LOG INTO THE ACC. WITHIN 3 ATTEMPTS", "-1");
			printf("\n  THE ACC. BELOW HAS BEEN LOCKED DUE TO SECURITY CONCERN\n\n\t  ACC. NO. : %s\n\n"
				"  PLEASE CONTACT A QUALIFIED MANAGER/STAFF IN ORDER TO UNLOCK THE ACC. ABOVE\n\n", accNo);
		}
		if (exception == -2) {
			printExit("YOUR ACC. IS LOCKED DUE TO SECURITY CONCERN", "-2");
			printf("\n  PLEASE CONTACT A QUALIFIED MANAGER/STAFF IN ORDER TO UNLOCK YOUR ACC.\n\n\t  <SYSTEM TERMINATED>\n");
		}
		//runtime error
		printError(exception);
	}
	catch (...) { //if any unknown error has occured
		printExit("AN UNEXPECTED EXCEPTION OCCURRED", "n/a");
	}

	writeF(test, cust, custCount);
	//writeF(custINF, cust, custCount);
	writeF(test2, mnger, mngerCount);
	//writeF(mngerINF, mnger, mngerCount);

	printf("\t\t<SYSTEM TERMINATED>\n\n");

	//free the memory
	delete[] cust;
	delete[] mnger;

	//close the files
	fclose(custINF);
	fclose(mngerINF);
	fclose(wdOUTF);
	fclose(transOUTF);
	fclose(cashdp);
	fclose(chequedp);
	fclose(test);
	fclose(test2);
	
	return 0;
}