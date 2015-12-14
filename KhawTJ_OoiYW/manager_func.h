#include "struct.h"

//deposit function--------------------------------------------------------

//deposit function--------------------------------------------------------

//print cash deposit log
void cash_log(FILE*cashdp) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	int count = 0, entry = 0;
	int y, m;
	BASEINFO *storage = 0;

	//prompt the user specific the year and month
	storage = readF(cashdp, &count, storage);
	printHeader("PRINT CASH TRANSACTION LOG", "", 0);
	printf("\n\t  0 -> BACK\n\n  PLEASE SELECT DATE WHICH CASH DEPOSITED\n\n");

	//prompt the user to specify the month and year
	y = validIpt("\t YEAR > ", 0, t.wYear);
	if (y == 0) return;
	m = validIpt("\tMONTH > ", 0, 12);
	if (m == 0) return;

	FILE* cashLOG = fopen("CashTxn.txt", "w+");

	//print the data with a neat format onto CashTxn.txt
	fprintf(cashLOG, "LIST OF CASH DEPOSITED\nLOG DATE & TIME");
	printTime(cashLOG);
	fprintf(cashLOG, "\t\tSPECIFIED DATE % 02d - %04d\n\nDATE & TIME\t ATMno\t  ACCno\tTXNcode\t   AMOUNT      prevBAL       newBAL\n", m, y);
	for (int i = 0; i < count; i++) {
		BASEINFO *tmp = &storage[i];
		TIME *tmp2 = &storage[i].t;
		if (tmp2->yr == y&&tmp2->mth == m) {
			fprintf(cashLOG, "%04d-%02d-%02d %02d:%02d:%02d\t  %02d\t  %5s\t     %d\t $%010.2lf  $%010.2lf  $%010.2lf\n",
				tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec,
				tmp->ATM, tmp->accNo, tmp->txnCode, tmp->amt, tmp->prevBal, tmp->newBal);
			entry++;
		}
	}
	//print the number of entries listed
	fprintf(cashLOG, "\n\t\t<%2d CASH DEPOSITED(S) LISTED>\n", entry);
	delete[] storage;
	fclose(cashLOG);
	printf("\n  FILE \"CashTxn.txt\" HAS BEEN SUCCESFULLY UPDATED\n");
	readKey();
}

//print cheque deposit log
void cheque_log(FILE*chequedp) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	int count = 0, entry = 0;
	int y, m;
	CHEQUE *storage = 0;

	//prompt the user specific the year and month
	storage = readF(chequedp, &count, storage);
	printHeader("PRINT CHEQUE TRANSACTION LOG", "", 0);
	printf("\n\t  0 -> BACK\n\n  PLEASE SELECT DATE WHICH CHEQUE DEPOSITED\n\n");

	//prompt the user to specify the month and year
	y = validIpt("\t YEAR > ", 0, t.wYear);
	if (y == 0) return;
	m = validIpt("\tMONTH > ", 0, 12);
	if (m == 0) return;

	FILE* chequeLOG = fopen("ChequeTxn.txt", "w+");

	//print the data with a neat format onto CashTxn.txt
	fprintf(chequeLOG, "LIST OF CHEQUE DEPOSITED\nLOG DATE & TIME");
	printTime(chequeLOG);
	fprintf(chequeLOG, "\t\tSPECIFIED DATE % 02d - %04d\n\nDATE & TIME\tATMno\t  ACCno\tTXNcode\t   TxnAmt         BANK\tChequeNo       PrevBAL       UpBAL	ChequeClear\n", m, y);
	for (int i = 0; i < count; i++) {
		CHEQUE *tmp = &storage[i];
		TIME *tmp2 = &storage[i].base.t;
		if (tmp2->yr == y&&tmp2->mth == m) {
			fprintf(chequeLOG, "%04d-%02d-%02d %02d:%02d:%02d\t  %02d\t  %5s\t     %d\t $%010.2lf     %-10s\t%-11s\t   $%010.2lf  $%010.2lf	    %d\n",
				tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec,
				tmp->base.ATM, tmp->base.accNo, tmp->base.txnCode, tmp->base.amt, tmp->bankCode, tmp->cheqNo,
				tmp->base.prevBal, tmp->base.newBal, tmp->check);
			entry++;
		}
	}
	//print the number of entries listed
	fprintf(chequeLOG, "\n\t\t<%2d CHEQUE DEPOSITED(S) LISTED>\n", entry);
	delete[] storage;
	fclose(chequeLOG);
	printf("\n  FILE \"ChequeTxn.txt\" HAS BEEN SUCCESFULLY UPDATED\n");
	readKey();
}


int cheq_clear(char *cheqNo, char *bank) {
	int valid = true;

	//if user tried to enter bank code consist of number or decimal places
	for (int i = 0; i < 10; i++) {
		if (bank[i] >= '0' && bank[i] <= '9' || bank[i] == ' ' || bank[i] == '.') {
			printf("\n  ERROR: BANK NAME --> CANNOT HAVE NUMBER OR DECIMAL PLACES");
			valid = false;
			break;
		}
	}
	for (int i = 0; i < 7; i++) {
		//if user tried to enter cheque number consist of characters or decimal places
		if (cheqNo[i] >= 'a' && cheqNo[i] <= 'z' || cheqNo[i] >= 'A' && cheqNo[i] <= 'Z' || cheqNo[i] == '.') {
			printf("\n  ERROR: CHEQUE NUMBER --> CANNOT HAVE CHARACTERS OR DECIMAL PLACES");
			valid = false;
			break;
		}
		//cheque number can consist of exactly 6-digits number only
		else if (cheqNo[i] == '\0' && cheqNo[6] != '\0' || cheqNo[6] != '\0') {
			printf("\n  ERROR: CHEQUE NUMBER --> CAN CONSIST OF EXACTLY 6-DIGITS NUMBER");
			valid = false;
			break;
		}
	}
	return valid;
}

int check_cheq(FILE *clear, char *accno, char *bankcode, char *cheq_num, int clear_chq) {
	char acc[7];
	char bank[30];
	char cheq_no[7];
	int chq_clear;

	int valid = true;
	//printf("  %s\t%s\t%s\t%d\n", accno, bankcode, cheq_num, clear_chq);

	//scan ChequeClear.txt to check whether the cheque updated or not
	while (fscanf(clear, "%[^\t]\t%[^\t]\t%[^\t]\t%d\n", &acc, &bank, &cheq_no, &chq_clear) != EOF) {
		//printf("  %s\t%s\t%s\t%d\n", acc, bank, cheq_no, chq_clear);
		if (strcmp(accno, acc) == 0 && strcmp(bankcode, bank) == 0 && strcmp(cheq_num, cheq_no) == 0 && clear_chq != chq_clear) {
			valid = false;
			break;
		}
	}
	return valid;
}

//cheque clearing
void cheque_clear(FILE*chequedp, CUSTOMER *cust, int custCount) {

	CUSTOMER *currCust = 0;
	CHEQUE *storage = 0;

	char accNo[7], bank[30], cheqNo[7];
	double amt, newbal;
	int valid, count = 0, chequeCount = 0;
	char check, confirm, choice;

	storage = readF(chequedp, &chequeCount, storage);

	do {
		valid = true;
		printHeader("CHEQUE CLEARING", "", 0);

		//print the available bank code, cheque number and status of cheque clear
		printf("\n CHEQUE CLEAR : 1-> ALREADY UPDATE\t 0-> FREE TO UPDATE\n\n");
		printf(" BANK CODE\t\tCHEQUE NUMBER\t\tCHEQUE CLEAR\n");
		printf(" =========\t\t=============\t\t============\n");
		for (int i = 0; i < chequeCount; i++) {
			CHEQUE *tmp = &storage[i];
			printf("  %s\t\t\t%s\t\t\t\t%d\n", tmp->bankCode, tmp->cheqNo, tmp->check);
		}

		printf("\n  PLEASE ENTER BANK CODE\n\n\t > ");
		scanf(" %[^\n]", bank);
		printf("\n  CHEQUE NUMBER (6-digit numbers) \n\n\t > ");
		scanf(" %[^\n]", cheqNo);

		valid = cheq_clear(cheqNo, bank);

		if (valid == false) {
			printf("\n\n  ERROR FOUND! DO YOU WISH TO CONTINUE OR EXIT?\n");
			printf("\t1 -> CONTINUE\n\t2 -> EXIT\n\n\tSELECT > ");
			do {
				check = validIpt();

				//2 > terminate the system
				if (check == 2) {
					confirm = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
					if (confirm == 'Y') throw 0;
					continue;
				}

				// if user does not enter 1 or 2
				if (check != 2 && check != 1)
					printf("\n  INVALID INPUT DETECED\n  PLEASE TRY AGAIN > ");
			} while (check != 2 && check != 1);

			// 1 or 2 choose N -> continue the system
			if (check == 1 || confirm == 'N') {
				readKey();
				continue;
			}
		}
	} while (confirm == 'N' || check == 1 && valid == false);
	//check the bank code and cheque number
	for (int i = 0; i < chequeCount; i++) {
		CHEQUE *tmp = &storage[i];

		//if pair print the account number and amount to update
		if (strcmp(tmp->bankCode, _strupr(bank)) == 0 && strcmp(tmp->cheqNo, cheqNo) == 0) {
			count++;
			// if cheque not update before
			if (tmp->check == 0) {
				printf("\n  ACCOUNT NUMBER > %s\t", tmp->base.accNo);
				printf("AMOUNT TO UPDATE > $%10.2lf\n", tmp->base.amt);
				newbal = tmp->base.newBal;
				choice = validIpt("\n  DO YOU WANT TO UPDATE THE CHEQUE?");
				if (choice == 'Y') {
					printf("\n\n  CHEQUE HAS BEEN UPDATED IN CUSTOMER ACCOUNT\n\n");
					strcpy(accNo, tmp->base.accNo);
					tmp->check = 1;
					amt = tmp->base.amt;

					currCust = findAcc(cust, tmp->base.accNo, custCount, 0);

					currCust->bal += amt;

					//update the files
					freopen("ChequeDeposits.dat", "w+b", chequedp);
					for (int i = 0; i < chequeCount; i++)
						fwrite(&storage[i], sizeof(CHEQUE), 1, chequedp);
					freopen("ChequeDeposits.dat", "a+b", chequedp);
					readKey();
					break;
				}
				else {
					printf("\n  CHEQUE CLEARING CANCEL\n");
					readKey();
					break;
				}
			}
			// if cheque updated 
			else {

				printf("\n\n  CHEQUE ALREADY UPDATED IN CUSTOMER ACCOUNT BEFORE\n  YOU CAN'T UPDATE AGAIN!\n");
				readKey();
			}

		}
		else continue;
	}
	if (count == 0) {
		printf("\n  --INVALID BANK CODE AND CHEQUE NUMBER--\n");
		readKey();
	}
}
//withdrawal function--------------------------------------------------------

//print withdrawal log
void printWdLog(FILE *wdOUTF) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	int count = 0, entry = 0;
	int y, m;
	BASEINFO *storage = 0;

	//store the data inside the withdrawal.dat
	storage = readF(wdOUTF, &count, storage);
	printHeader("PRINT WITHDRAWAL LOG", "", 0);
	printf("\n\t  0 -> BACK\n\n  PLEASE SPECIFY A DATE\n\n");

	//prompt the user to specify the month and year
	y = validIpt("\t YEAR", 0, t.wYear);
	if (y == 0) return;
	m = validIpt("\tMONTH", 0, 12);
	if (m == 0) return;

	//create withdrawal_log.txt
	FILE *wdLOG = fopen("Withdrawal_Log.txt", "w+");

	//print the data with a neat format onto withdrawal_log.txt
	fprintf(wdLOG, "LIST OF CASH WITHDRAWALS\nLOG DATE & TIME : ");
	printTime(wdLOG);
	fprintf(wdLOG, "\t\tSPECIFIED DATE : %02d-%04d\n\nDATE & TIME\t\t ATMno\t  ACCno\tTXNcode\t AMOUNT\t     prevBAL     newBAL\n", m, y);
	for (int i = 0; i < count; i++) {
		BASEINFO *tmp = &storage[i];
		TIME *tmp2 = &storage[i].t;
		if (tmp2->yr == y&&tmp2->mth == m) {
			fprintf(wdLOG, "%04d-%02d-%02d %02d:%02d:%02d\t  %02d\t  %5s    %d\t $%010.2lf $%010.2lf $%010.2lf\n",
				tmp2->yr, tmp2->mth, tmp2->dy, tmp2->hr, tmp2->min, tmp2->sec,
				tmp->ATM, tmp->accNo, tmp->txnCode, tmp->amt, tmp->prevBal, tmp->newBal);
			entry++;
		}
	}

	//print the number of entries listed
	fprintf(wdLOG, "\n\t\t<%2d WITHDRAWAL(S) LISTED>\n", entry);
	delete[] storage;
	fclose(wdLOG);
	printf("\n  FILE \"Withdrawal_Log.txt\" HAS BEEN SUCCESFULLY UPDATED\n");
	readKey();
}

//print transfer log
void printTransLog(FILE *transOUTF) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	int count = 0, entry = 0;
	int y, m;
	TRANSFER *storage = 0;

	//store the data inside the transfer.dat
	storage = readF(transOUTF, &count, storage);
	printHeader("PRINT TRANSFER LOG", "", 0);

	//prompt the user to specify the month and year
	printf("\n\t  0 -> BACK\n\n  PLEASE SPECIFY A DATE\n\n");
	y = validIpt("\t YEAR", 0, t.wYear);
	if (y == 0) return;
	m = validIpt("\tMONTH", 0, 12);
	if (m == 0) return;

	//create transfer_log.txt
	FILE *transLOG = fopen("Transfer_Log.txt", "w+");

	//print the data with a neat format onto the transfer_log.txt
	fprintf(transLOG, "LIST OF FUNDS TRANSFERS\nLOG DATE & TIME : ");
	printTime(transLOG);
	fprintf(transLOG, "\t\tSPECIFIED DATE : %02d-%04d\n\nDATE & TIME\t\t ATMno\t  ACCno\tTXNcode\t AMOUNT\t\tRecAccNo RecPrevBal  RecNewBal\t PrevBal     NewBal\n", m, y);
	for (int i = 0; i < count; i++) {
		TRANSFER *tmp = &storage[i];
		BASEINFO *tmp2 = &storage[i].base;
		TIME *tmp3 = &storage[i].base.t;
		if (tmp3->yr == y&&tmp3->mth == m) {
			fprintf(transLOG, "%04d-%02d-%02d %02d:%02d:%02d\t  %02d\t  %5s    %d\t $%010.2lf\t%5s \t $%010.2lf $%010.2lf $%010.2lf $%010.2lf\n",
				tmp3->yr, tmp3->mth, tmp3->dy, tmp3->hr, tmp3->min, tmp3->sec,
				tmp2->ATM, tmp2->accNo, tmp2->txnCode, tmp2->amt, tmp->recAccNo, tmp->recPrevBal, tmp->recNewBal, tmp2->prevBal, tmp2->newBal);
			entry++;
		}
	}

	//print the number of entries listed
	fprintf(transLOG, "\n\t\t<%2d TRANSFER(S) LISTED>\n", entry);
	delete[] storage;
	fclose(transLOG);
	printf("\n  FILE \"Transfer_Log.txt\" HAS BEEN SUCCESFULLY UPDATED\n");
	readKey();
}

//high withdrawal alert system
void wdAlert(CUSTOMER *cust, int custCount, FILE *wdOUTF, FILE *transOUTF) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	char confirm;
	int wdCount = 0, transCount = 0, alertCount = 0;
	double *amtOut = new double[custCount];
	for (int i = 0; i < custCount; i++) amtOut[i] = 0;
	FILE *outf;
	TIME current = setTime();
	BASEINFO *wd = 0;
	TRANSFER *trans = 0;

	//obtain the withdrawals and transfers records from the files and store into the structures respectively
	wd = readF(wdOUTF, &wdCount, wd);
	trans = readF(transOUTF, &transCount, trans);

	//add and keep track of the total cash out for all customers and store them into a particular slot that depends on the index no. of the customer
	for (int i = 0; i < wdCount; i++) {
		for (int j = 0; j < custCount; j++) {
			if (strcmp(cust[j].accNo, wd[i].accNo) == 0 && wd[i].t.mth == t.wMonth) {
				amtOut[j] += wd[i].amt;
				break;
			}
		}
	}
	for (int i = 0; i < transCount; i++) {
		for (int j = 0; j < custCount; j++) {
			if (strcmp(cust[j].accNo, trans[i].base.accNo) == 0 && trans[i].base.t.mth == t.wMonth) {
				amtOut[j] += trans[i].base.amt;
				break;
			}
		}
	}
	printHeader("HIGH WITHDRAWAL ALERT SYSTEM", "", 0);
	printf("\n  AMOUNT SET : $ 5000.00\tDATE : %04d-%02d", t.wYear, t.wMonth);
	printf("\n  LIST OF ACCOUNTS THAT HAVE TRIGGERED THE HIGH WITHDRAWAL ALERT :-\n\n  NO. ACCOUNT OWNER\t\t\tNET CASH OUT\tCONTACT NO.\n"
		"  --- -------------\t\t\t------------\t-----------\n");

	//list out the accounts that have triggered the high withdrawals alert
	for (int i = 0; i < custCount; i++) {
		if (amtOut[i] >= 5000)
			printf("  %2d. %-30s\t$ %10.2lf\t%s\n", ++alertCount, cust[i].name, amtOut[i], cust[i].hp);
	}

	//ask for permission to print out the notification letters
	confirm = validIpt("PRINT THE NOTIFICATION LETTERS FOR THE CUSTOMER(S) ABOVE?\n ");
	if (confirm == 'N') return;

	//print the letter for every each of the accounts.
	else {
		for (int i = 0; i < custCount; i++) {
			if (amtOut[i] >= 5000) {
				char fileName[51] = "Notification_";
				strcat(strcat(fileName, cust[i].name), ".txt");
				outf = fopen(fileName, "w");
				fprintf(outf,
					"TARBANK NOTIFICATION\n\n"
					"%s\n%s\n%s\n\n%s\n\n%02d-%02d-%04d\n\n"
					"SIR/MDM.\n\nHIGH WITHDRAWALS ALERT NOTIFICATION\n\n"
					"Please to be noted that your existing account (Acc. no. = %s) has triggered the high withdrawal alert.\n"
					"It means that cash with the amount at least $5000.00 has withdrew/transferred from the bank account.\n"
					"\n\tNET AMOUNT WITHDREW DURING %04d-%02d = $ %.2lf\n"
					"\nIf you have found that any abnormal or unidentified transactions had been occurred, \n"
					"please kindly contact our customer service at 03-4567890 immediately.\n"
					"\nThank you.\n"
					"\n\n-This is a computer generated notice. No signature is required.\n"
					, cust[i].name, cust[i].adds, cust[i].state, cust[i].hp, current.dy, current.mth, current.yr, cust[i].accNo, t.wYear, t.wMonth, amtOut[i]);
				fclose(outf);
			}
		}
	}
	printf("\n  %d DOCUMENT FILES HAVE BEEN CREATED\n", alertCount);
	readKey();
	delete[] amtOut;
}

//additional function--------------------------------------------------------

//unlock account function
void unlockAcc(CUSTOMER *cust, int custCount) {
	CUSTOMER *custPtr;
	char acc[7], confirm;
	do {
		//reset the value
		acc[0] = '\0';
		confirm = 'N';
		custPtr = nullptr;
		printHeader("UNLOCK ACCOUNT FUNCTION", "", 0);
		printBreak();
		printf("\n  PLEASE KEY IN THE ACC. THAT IS GOING TO BE UNLOCKED > ");
		scanf(" %6[^\n]", acc);

		// 0 -> back
		if (strcmp(acc, "0") == 0) return;

		// -1 -> quit
		if (strcmp(acc, "-1") == 0) {
			confirmBreak(&confirm);
			continue;
		}

		//search for the account no. entered by the user
		custPtr = findAcc(cust, acc, custCount, 0);

		//if the account no. is not found, user is prompted to try again
		if (custPtr == nullptr) continue;

		//else print the customer's details and the status of his/her account
		printf("\n  ACCOUNT OWNER\t : %s\n  ACC. NO.\t : %s\n  ADDRESS\t : %s\n  STATE\t\t : %s\n  H/P\t\t : %s\n\n  ACCOUNT STATUS : %s\n",
			custPtr->name, custPtr->accNo, custPtr->adds, custPtr->state, custPtr->hp, (custPtr->lock == 3) ? "LOCKED" : "AVAILABLE");

		//if the account is available, user will be reminded that the account cannot be unlocked
		if (custPtr->lock != 3) {
			printf("\n  THE ACCOUNT ABOVE HAS NOT BEEN LOCKED, IT CANNOT BE UNLOCKED\n");
			readKey();
			continue;
		}

		//confirmation to unlock the selected account
		confirm = validIpt("CONFIRM TO UNLOCK THE ACCOUNT ABOVE?");

		//if yes, reset the lock value of the account back to 0
		if (confirm == 'Y')
			custPtr->lock = 0;
	} while (confirm == 'N');

	printf("\n  THE ACCOUNT HAS BEEN SUCCESSFULLY UNLOCKED AND AVAILABLE FOR USER TO LOG IN\n");
	readKey();
}

//reset the password : manager version
void resetPassw(MANAGER *currMnger) {
	char passw[25];
	do {
		//reset the value
		passw[0] = '\0';
		printHeader("RESET PASSWORD", "", 0);

		//user will be prompted to enter his/her current password
		printf("\n\t0 -> BACK\n\n  PLEASE KEY IN YOUR CURRENT PASSWORD > ");
		scanf(" %24[^\n]", passw);

		// 0 -> back
		if (strcmp("0", passw) == 0) return;

		//if the password is valid and match, proceed to the next section
		if (strcmp(currMnger->passw, passw) == 0) break;
		//else retry
		else {
			printf("\n  INVALID PASSWORD\n  PLEASE TRY AGAIN\n");
			readKey();
		}
	} while (1);
	printHeader("RESET PASSWORD", "", 0);

	//user is being prompted to enter his/her new password
	strcpy(currMnger->passw, setPassw("PLEASE KEY IN YOUR NEW PASSWORD", 23, nullptr, currMnger));
	printf("\n  YOUR PASSWORD HAS BEEN UPDATED\n");
	readKey();
}

//register for new customer or manager
void registerUser(FILE *custINF, CUSTOMER *cust, int custCount) {
	srand(time(NULL));
	char confirm;
	int valid;
	int accNo, pinNo;
	double bal;
	char name1[11], name2[26], name[36] = "", gender, adds[29], state[14], hp[12];
	do {
		/*
		1 - accept the information about the new customer from the user
		2 - if user wished to back, terminate the whole function
		3 - adjust the input data into a neat format
		4 - validate all the input that have been entered by the user
		5 - remind and prompt the user to re-enter the data if there is any invalid input detected
		*/
		confirm = 'N';
		valid = true;
		printHeader("REGISTRATION FOR NEW CUSTOMER", "", 0);
		printf("\n\t0 -> BACK\n\n  PLEASE ENTER DETAILS OF THE NEW CUSTOMER :-\n\n\tSURNAME > ");
		scanf(" %10[^\n]", name1);
		if (confirmBack(name1)) return;
		discard_junk();
		strcpy(name1, _strupr(name1));
		strcat(name1, " ");
		printf("\tNAME > ");
		scanf(" %25[^\n]", name2);
		if (confirmBack(name2)) return;
		discard_junk();
		name2[0] = toupper(name2[0]);
		for (int i = 0; i < 25; i++) {
			if (name2[i] == ' ') name2[i + 1] = toupper(name2[i + 1]);
			else name2[i+1] = tolower(name2[i+1]);
		}
		printf("\tGENDER > ");
		scanf(" %c", &gender);
		if (confirmBack(gender)) return;
		discard_junk();
		gender = toupper(gender);
		printf("\tADDRESS > ");
		scanf(" %28[^\n]", adds);
		if (confirmBack(adds)) return;
		adds[0] = toupper(adds[0]);
		discard_junk();
		printf("\tSTATE > ");
		scanf("  %13[^\n]", state);
		if (confirmBack(state)) return;
		state[0] = toupper(state[0]);
		discard_junk();
		printf("\tCONTACT NO. (###-#######) > ");
		scanf(" %11[^\n]", hp);
		if (confirmBack(hp)) return;
		discard_junk();
		printf("\n\tINITIAL BALANCE (AT LEAST $ 20) > $ ");
		bal = validIpt();
		if (confirmBack(bal)) return;

		//validation of the input data
		strcat(strcpy(name, name1), name2);

		if (gender != 'F'&&gender != 'M') {
			printf("\n  #1 - GENDER CAN ONLY BE 'M' AND'F'\n");
			valid = false;
		}
		for (int i = 0; i < 11; i++) {
			if (i == 3 && hp[i] != '-') {
				printf("\n  #2 - PLEASE FOLLOW THE FORMAT GIVEN WHILE ENTERING THE CONTACT NO.\n");
				valid = false;
			}
			else if (i != 3 && isdigit(hp[i]) == 0) {
				printf("\n  #3 - CONTACT NO. CANNOT CONSIST OF ANY NON-DIGIT CHARACTER\n");
				valid = false;
				break;
			}
			if (hp[i] == '\0') break;
		}
		if (bal < 20) {
			printf("\n  #4 - INITIAL BALANCE MUST AT LEAST BE $ 20\n");
			valid = false;
		}
		if (valid == false) readKey();
		//jump back to the beginning of the loop if the input data have failed the validation process
	} while (valid == false);


	for (int i = 0; i < custCount; i++) {
		if (strcmp(cust[i].name, name) == 0) {
			printf("\n  #5 - SAME NAME IN DATABASE HAS DETECTED : %s\n", cust[i].name);
			confirm = validIpt("CONFIRM TO CREATE ANOTHER ACCOUNT FOR THIS CUSTOEMR?");
			if (confirm == 'N') return;
			break;
		}
	}

	while (1) {
		//generate random account no. and pin. no.
		accNo = rand() % 100000;
		pinNo = rand() % 100000;
		if (accNo < 10000)
			accNo += 10000;

		//if there is a match of the generated account no. when comparing to the customer database, re-generate it
		for (int i = 0; i < custCount; i++) {
			if (strcmp(cust[i].accNo, toString(accNo)) == 0)
				continue;
		}
		break;
	}
	system("cls");

	//print all of the input data that have currently entered by the user and confirmation for registration
	printHeader("REGISTRATION FOR NEW CUSTOMER", "", 0);
	printf("\n  COMPUTER-GENERATED ID NO. \t: %5d\n  COMPUTER-GENERATED PIN. NO. \t: *****\n\n\tNAME \t\t: %s%s\n\tGENDER \t\t: %c"
		"\n\tADDRESS \t: %s\n\tSTATE \t\t: %s\n\tCONTACT NO. \t: %s\n\n\tINITIAL BALANCE : $ %10.2lf\n\n",
		accNo, name1, name2, gender, adds, state, hp, bal);
	confirm = validIpt("CONFIRM TO FINALIZE THE REGISTRATION");
	if (confirm == 'N') return;
	else printf("\n  THE REGISTRATION FOR NEW CUSTOMER HAS BEEN COMPLETED\n\n");

	//append the data into the customer database
	fprintf(custINF, "%05d|%05d|%-36s|%c|%-29s|%-14s|%-11s|%d|%010.2lf %02d-%02d-%04d %02d:%02d:%02d\n",
		accNo, pinNo, strcat(strcat(name1, name2), "_"), gender, strcat(adds, "_"), strcat(state, "_"), hp, 0, bal, 0, 0, 0, 0, 0, 0);
	readKey();

	//terminate the system and ask for rebooting it
	throw 2;
}