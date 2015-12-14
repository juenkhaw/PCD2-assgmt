#include "struct.h"

//deposit function-----------------------------------------------------------

//deposit amount validation function
int depositCheck(double amt) {
	int valid = true;
	//if user tried to deposit amount of cash and amount with cents 
	if ((int)(ceil(amt)) % 5 != 0 || (int)(floor(amt)) % 5 != 0) {
		printf("\n  AMOUNT TO BE DEPOSIST MUST BE A MULTPLE OF 5 AND CANNOT CONSISTS OF CENTS\n");
		return false;
	}
	//if user tried to deposit with a negative values
	else if (amt < 0 && amt != -1) {
		printf("\n  AMOUNT TO BE DEPOSITED CANNOT BE A NEGATIVE VALUES\n");
		valid = false;
	}
	return valid;
}

//cheque details validation function
int chequeCheck(double amt, char *cheqNo, char *bank) {

	int valid = true;

	//if user tried to enter bank code consist of number or decimal places
	for (int i = 0; i < 10; i++) {
		if (bank[i] >= '0' && bank[i] <= '9' || bank[i] == '.') {
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
	// if user tried to enter amount with negative values
	if (amt < 0) {
		printf("\n  ERROR: CHEQUE AMOUNT --> CANNOT BE A NEGATIVE VALUES");
		valid = false;
	}
	// if user tried to enter cheque amount with zero values
	else if (amt == 0) {
		printf("\n  ERROR: CHEQUE AMOUNT --> CANNOT BE ZERO");
		valid = false;
	}

	return valid;
}

// cash deposit system
void cash_dpFunc(FILE *cashdp, CUSTOMER *currCust, int nATM){
	char confirm;
	int valid;
	double amt;
	do {
		valid = true;
		printHeader("CASH DEPOSITS SYSTEM", "", nATM);
		printBreak();
		printf("\n  NET BALANCE : $ %9.2lf\n  PLEASE KEY IN THE AMOUNT TO BE DEPOSIIED\n\n\t > $ ", currCust->bal);
		amt = validIpt();

		//-1 > terminate the system
		if (amt == -1) {
			confirm = validIpt("TERMINATE THE CURRENT PROCESS AND QUIT?");
			if (confirm == 'Y') throw 0;
			continue;
		}

		//0 -> back
		if (amt == 0) return;

		valid = depositCheck (amt);

		//if the amount failed the validation, user is prompted to enter
		if (valid == false) {
			printf("\n  PLEASE TRY AGAIN\n");
			readKey();
			continue;
		}

		//transaction confirmation
		confirm = validIpt("PLEASE CONFIRM YOUR ON-GOING TRANSACTION");
		if (confirm == 'Y')	currCust->bal += amt;
		else continue;
	} while (valid == false || confirm == 'N');
	printf("\n  DEPOSIT SUCCESSFULLY\n  UPDATED NET BALANCE : $ % 9.2lf\n", currCust->bal);
	readKey();

	//append the data into the appropriate file
	BASEINFO curr_cashDP;
	curr_cashDP.t = setTime();
	strcpy(curr_cashDP.accNo, currCust->accNo);
	curr_cashDP.ATM = nATM; curr_cashDP.txnCode = 1;
	curr_cashDP.amt = amt; curr_cashDP.prevBal = currCust->bal - amt; curr_cashDP.newBal = currCust->bal;
	fwrite(&curr_cashDP, sizeof(BASEINFO), 1, cashdp);
}

//cheque deposit system
void cheque_dpFunc(FILE *chequedp, CUSTOMER *currCust, int nATM) {
	char confirm;
	int valid;
	char cheqNo[7];
	double amt;
	char bank[10];
	int check;
	do {
		valid = true;
		printHeader("CHEQUE DEPOSITS SYSTEM", "", nATM);
		printf("\n  NET BALANCE : $ %9.2lf\n  PLEASE NOTE THAT YOUR BALANCE WILL ONLY BE UPDATED UPON CHEQUE CLEARANCE.\n", currCust->bal);

		printf("\n  NAME OF PAYEE BANK (eg. MBB,CIMB, etc)\n\n\t > ");
		scanf(" %[^\n]", bank);

		printf("\n  CHEQUE NUMBER (6-digit numbers)\n\n\t > ");
		scanf(" %[^\n]", cheqNo);

		printf("\n  AMOUNT STATED ON THE CHEQUE\n\n\t > $ ");
		amt = validIpt();

		valid = chequeCheck(amt, cheqNo, bank);

		//if error(s) are found, prompt and ask user whether wan to continue or terminate the system
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

			// 1 or under 2 choose N > continue the system
			if (check == 1 || confirm == 'N') {
				readKey();
				continue;
			}
		}

		//transaction confirmation
		confirm = validIpt("PLEASE CONFIRM YOUR ON-GOING TRANSACTION");
		if (confirm == 'Y')	currCust->bal;
		else continue;
	} while (confirm == 'N' || check == 1 && valid == false);
	printf("\n  CHEQUE DEPOSIT SUCCESSFULLY\n  PLEASE CHECK YOUR BALANCE IN 3 DAYS' TIME\n");
	readKey();

	//append the data into the appropriate file
	CHEQUE curr_cheqdp;
	curr_cheqdp.base.t = setTime(); curr_cheqdp.base.ATM = nATM;
	curr_cheqdp.check = 0;
	strcpy(curr_cheqdp.base.accNo, currCust->accNo);
	curr_cheqdp.base.txnCode = 2; curr_cheqdp.base.amt = amt;
	strcpy(curr_cheqdp.bankCode, _strupr(bank)); strcpy(curr_cheqdp.cheqNo, cheqNo);
	curr_cheqdp.base.prevBal = currCust->bal; curr_cheqdp.base.newBal = currCust->bal;
	fwrite(&curr_cheqdp, sizeof(CHEQUE), 1, chequedp);
}

//withdrawal function--------------------------------------------------------

//withdrawal amount validation function
bool validAmount(double bal, double amt, int sel) {
	//validate the amount of withdrawal 
	if (sel == 1) {
		//if user tried to withdraw amount which is larger or smaller than the allowed range
		if (amt > bal - 20 || amt < 50) {
			printf("\n  %s\n", (amt < 50) ? "AMOUNT TO BE WITHDRAWN MUST BE MINIMUM AT LEAST $50" : "A MINIMUM BALANCE OF $20 MUST BE REMAINED INSIDE YOUR BANK ACC.");
			return false;
		}

		//if user tried to withdraw invalid amount of cash, amount with cents etc.
		if ((int)(ceil(amt)) % 5 != 0 || (int)(floor(amt)) % 5 != 0) {
			printf("\n  AMOUNT TO BE WITHDRAWN MUST BE A MULTPLE OF 5 AND CANNOT CONSISTS OF CENTS\n");
			return false;
		}
	}
	//validate the transfer amount
	else {
		//if the amount is negative figure
		if (amt < -1) {
			printf("\n  AMOUNT MUST AT LEAST BE $1\n");
			return false;
		}

		//if the user was trying to transfer amount that would cause his balance reduce to less than $20
		if (amt > bal - 20) {
			printf("\n  A MINIMUM BALANCE OF $20 MUST BE REMAINED INSIDE YOUR BANK ACC.\n");
			return false;
		}
	}
	return true;
}

//cash withdrawal system
void wdFunc(FILE *wdOUTF, CUSTOMER *currCust, int nATM) {
	char confirm;
	int valid;
	double amt;
	do {
		valid = true;
		amt = 0;
		confirm = 'N';
		printHeader("CASH WIHDRAWAL SYSTEM", "", nATM);
		printBreak();
		printf("\n  YOUR NET BALANCE : $ %9.2lf\n  PLEASE KEY IN THE AMOUNT TO BE WITHDRAWN\n\n\t > $ ", currCust->bal);
		amt = validIpt();

		//-1 > terminate the system
		if (amt == -1) {
			confirmBreak(&confirm);
			continue;
		}

		//0 -> back
		if (amt == 0) return;

		//validate the amount that is going to be withdrew
		valid = validAmount(currCust->bal, amt, 1);

		//if the amount failed the validation, user is prompted to enter
		if (valid == false) {
			printf("\n  PLEASE TRY AGAIN\n");
			readKey();
			continue;
		}

		//transaction confirmation
		confirm = validIpt("PLEASE CONFIRM YOUR ON-GOING TRANSACTION");

		//update the user's balance
		if (confirm == 'Y')	currCust->bal -= amt;
		else continue;
	} while (valid == false || confirm == 'N');
	printf("\n  CASH HAS BEEN SUCCESSFULLY WITHDRAWN\n  YOUR UPDATED NET BALANCE : $ % 9.2lf\n", currCust->bal);
	readKey();

	//append the data into the appropriate file
	BASEINFO currWd;
	currWd.t = setTime();
	strcpy(currWd.accNo, currCust->accNo);
	currWd.ATM = nATM; currWd.txnCode = 3;
	currWd.amt = amt; currWd.prevBal = currCust->bal + amt; currWd.newBal = currCust->bal;
	fwrite(&currWd, sizeof(BASEINFO), 1, wdOUTF);
}

//transfer system
void transFunc(FILE *transOUTF, CUSTOMER *currCust, CUSTOMER *cust, int custCount, int nATM) {
	CUSTOMER *recipient;
	char recAcc[7], confirm;
	int valid;
	double amt;
	do {
		//reset the variables
		amt = 0;
		recAcc[0] = '\0';
		confirm = 'N';
		valid = true;
		recipient = nullptr;
		printHeader("TRANSFER SYSTEM", "", nATM);
		printBreak();
		printf("\n  YOUR NET BALANCE : $ %9.2lf\n\n  PLEASE KEY IN THE ACC. NO. OF THE RECIPIENT > ", currCust->bal);
		scanf(" %6[^\n]", recAcc);

		//-1 -> quit
		if (strcmp(recAcc, "-1") == 0) {
			confirmBreak(&confirm);
			continue;
		}

		//0 -> back
		if (strcmp(recAcc, "0") == 0) return;

		//check for the existence of the recipient account
		recipient = findAcc(cust, recAcc, custCount, nATM);
		if (recipient == nullptr) continue;

		//if the recipient account no. was matched with the account no. of current user, continue
		if (strcmp(recipient->accNo, currCust->accNo) == 0) {
			printf("\n  CASH CANNOT BE TRANSFFERED WITHIN ONE SAME ACC.\n  PLEASE TRY AGAIN\n");
			readKey();
			valid = false;
			continue;
		}

		//prompt for the amount going to be transferred
		printf("\n\tRECIPIENT NAME : %s\n\n  PLEASE KEY IN THE AMOUNT TO BE TRANSFERRED\n\n\t  > $ ", recipient->name);
		amt = validIpt();

		//-1 > terminate the system
		if (amt == -1) {
			confirmBreak(&confirm);
			continue;
		}

		//0 -> back
		if (amt == 0) return;

		//validating the amount that is going to be transferred
		valid = validAmount(currCust->bal, amt, 2);

		//if the amount has failed the validation, user is prompted to enter again
		if (valid == false) {
			printf("\n  PLEASE TRY AGAIN\n");
			readKey();
			continue;
		}

		//transaction confirmation
		confirm = validIpt("PLEASE CONFIRM YOUR ON-GOING TRANSACTION");

		//update the user's balance
		if (confirm == 'Y') {
			currCust->bal -= amt;
			recipient->bal += amt;
		}
		else continue;
	} while (valid == false || confirm == 'N');
	printf("\n  CASH HAS BEEN SUCCESSFULLY TRANSFERRED\n\n  RECEIVER NAME\t\t : %s\n  RECEIVER ACC. NO.\t : %s\n\n  YOUR UPDATED NET BALANCE\t : $ %9.2lf\n",
		recipient->name, recipient->accNo, currCust->bal);
	readKey();

	//append the data into appropriate file
	TRANSFER currTrans;
	BASEINFO *tmp = &currTrans.base;
	tmp->t = setTime();
	strcpy(tmp->accNo, currCust->accNo);
	tmp->ATM = nATM; tmp->txnCode = 4;
	tmp->amt = amt; tmp->prevBal = currCust->bal + amt; tmp->newBal = currCust->bal;
	strcpy(currTrans.recAccNo, recAcc);
	currTrans.recPrevBal = recipient->bal - amt; currTrans.recNewBal = recipient->bal;
	fwrite(&currTrans, sizeof(TRANSFER), 1, transOUTF);
}

//additional function--------------------------------------------------------

//reset the password : customer version
void resetPassw(CUSTOMER *currCust) {
	char passw[7];
	currCust->lock = 0;
	do {
		//reset the value
		passw[0] = '\0';
		printHeader("RESET PIN. NO.", "", 0);

		//prompt for user to enter his/her current pin. no.
		printf("\n\t0 -> BACK\n\n  WARNING - YOU HAVE ALSO BEEN GIVEN ONLY 3 ATTEMPTS\n  PLEASE KEY IN YOUR CURRENT PIN. NO. > ");
		scanf(" %6[^\n]", passw);
		//0 -> back
		if (strcmp(passw, "0") == 0) return;
		//if the pin. no. is matched and correct
		if (strcmp(currCust->PIN, passw) == 0) break;
		//else the user will be warned with the attempts left for him/her to try again
		else {
			currCust->lock++;
			//if the user has been failed to log into the account for 3 times, the system will lock the account and terminate
			if (currCust->lock == 3)
				throw - 1;
			printf("\n  INVALID PIN. NO.\n  WARNING - YOU HAVE ONLY 3 ATTEMPTS TO LOG INTO YOUR account\n"
				"  YOU HAVE ONLY %d ATTEMPT(S) LEFT\n", 3 - currCust->lock);
			readKey();
		}
	} while (1);
	printHeader("RESET PIN. NO.", "", 0);
	//user is being prompted to reset his/her pin. no.
	strcpy(currCust->PIN, setPassw("PLEASE NOTED THAT YOU SHOULD REMEMBER YOUR NEW PIN. NO.\n  THE PREVIOUS PIN. NO. WILL NO LONGER BE VALID \n  ONCE YOU HAVE UPDATED YOUR PIN. NO.", 5, currCust, nullptr));
	printf("\n  YOUR PIN. NO. HAS BEEN SUCCESSFULLY UPDATED\n");
	readKey();
}

//for customer to check the account balance and also provide transactions that are realized
void accDetails(CUSTOMER *currCust, FILE *wdOUTF, FILE *transOUTF, FILE *cashdp, FILE *chequedp) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	char confirm, filename[50] = "transReport_", c;
	int yr, mth;
	int wdCount = 0, transCount = 0, dpCount = 0, chqdpCount = 0;
	double wdTotal = 0, transTotal = 0, dpTotal = 0, chqdpTotal = 0, transinTotal = 0;
	double totalIn = 0, totalOut = 0;
	BASEINFO *wd = 0, *dp = 0;
	TRANSFER *trans = 0;
	CHEQUE *chqdp = 0;
	strcat(strcat(filename, currCust->name), ".txt");
	FILE *outf = fopen(filename, "w+");

	//read the data from appropriate files to the dynamic structure arrays respectively
	wd = readF(wdOUTF, &wdCount, wd);
	trans = readF(transOUTF, &transCount, trans);
	dp = readF(cashdp, &dpCount, dp);
	chqdp = readF(chequedp, &chqdpCount, chqdp);

	//print the user's details and his/he current balance
	printHeader("ACCOUNT DETAILS", currCust->name, 0);
	printf("\n\tACC. NO. \t: %s\n\tOWNER NAME \t: %s\n\tGENDER \t\t: %c\n\tADDRESS \t: %s\n\tSTATE \t\t: %s\n\tCONTACT NO. \t: %s\n",
		currCust->accNo, currCust->name, currCust->gender, currCust->adds, currCust->state, currCust->hp);
	printf("\n\tCURRENT BALANCE : $ %10.2lf\n", currCust->bal);

	//prompt the user whether he/she want to review his/her transactions history or not
	confirm = validIpt("\n  DO YOU WANT TO REVIEW YOUR TRANSACTIONS HISTORY?\n  N -> BACK");
	//if no, return to main menu
	if (confirm == 'N') return;
	//if yes, user is prompted to specify a date that transactions was taken place
	else {
		printHeader("CUSTOMER TRANSACTIONS DRILL DOWN REPORT", currCust->name, 0);
		printf("\n  PLEASE SPECIFY A DATE THAT YOU WANT TO REVIEW YOUR TRANSACTIONS HISTORY\n\n");
		yr = validIpt("\t YEAR", 2015, t.wYear);
		mth = validIpt("\tMONTH", 1, 12);
		system("cls");

		//print a neat drill down report for the user's transactions and classify them into 5 main categories
		printHeader("CUSTOMER TRANSACTIONS DRILL DOWN REPORT", currCust->name, 0);
		fprintf(outf, "  TARBANK\n  TRANSACTIONS DRILL DOWN REPORT\n  %s - %s\n\n  GENERATED ON : ", currCust->name, currCust->accNo);
		printTime(outf);
		fprintf(outf, "\n  SPECIFIED DATE : %02d-%04d\n", mth, yr);
		for (int i = 0; i < 79; i++) fprintf(outf, "-");

		/*
		Each of the categories will only print the transactions that are took place in the date specified by the user:-
		1 -> match the user account no. and the account no. that are involved to the transactions
		2 -> match the date and time specified
		3 -> add the amount involved to the accumulated total amounts respectively
		4 -> print the transaction entries in a text file
		*/

		//deposit transactions
		fprintf(outf, "\n  DEPOSIT HISTORY\n  ===============\n\n  DATE & TIME\t\tAMOUNT DEPOSITED\n  -----------\t\t----------------\n  ");
		for (int i = 0; i < dpCount; i++) {
			if (strcmp(dp[i].accNo, currCust->accNo) == 0 && dp[i].t.yr == yr&&dp[i].t.mth == mth) {
				dpTotal += dp[i].amt;
				printTime(outf, dp[i].t);
				fprintf(outf, "   $ %10.2lf\n  ", dp[i].amt);
			}
		}
		fprintf(outf, "\n\t>>> NET AMOUNT DEPOSITED : $ %10.2lf\n\n", dpTotal);
		for (int i = 0; i < 79; i++) fprintf(outf, "-");

		//cheque deposit transactions
		fprintf(outf, "\n  CHEQUE DEPOSIT HISTORY\n  ======================\n\n  DATE & TIME\t\tCHEQUE AMOUNT\tCHEQUE NO.\tPAYEE BANK PROCESSED\n"
			"  -----------\t\t-------------\t----------\t---------- ---------\n  ");
		for (int i = 0; i < chqdpCount; i++) {
			if (strcmp(chqdp[i].base.accNo, currCust->accNo) == 0 && chqdp[i].base.t.yr == yr&&chqdp[i].base.t.mth == mth) {
				chqdpTotal += chqdp[i].base.amt;
				printTime(outf, chqdp[i].base.t);
				fprintf(outf, "   $ %10.2lf\t%6s\t\t%-10s\t%c\n  ", chqdp[i].base.amt, chqdp[i].cheqNo, chqdp[i].bankCode, (chqdp[i].check==0)?'N':'Y');
			}
		}
		fprintf(outf, "\n\t>>> NET CHEQUE DEPOSITED : $ %10.2lf\n\n", chqdpTotal);
		for (int i = 0; i < 79; i++) fprintf(outf, "-");

		//list of the amount transferred to the user's account
		fprintf(outf, "\n  LIST OF CASH TRANSFERRED IN\n  ===========================\n\n  DATE & TIME\t\tAMOUNT TRANSFERRED\tPAYEE ACC.\n  -----------\t\t------------------\t----------\n  ");
		for (int i = 0; i < transCount; i++) {
			if (strcmp(trans[i].recAccNo, currCust->accNo) == 0 && trans[i].base.t.yr == yr&&trans[i].base.t.mth == mth) {
				transinTotal += trans[i].base.amt;
				printTime(outf, trans[i].base.t);
				fprintf(outf, "   $ %10.2lf\t\t%s\n  ", trans[i].base.amt, trans[i].base.accNo);
			}
		}
		fprintf(outf, "\n\t>>> NET AMOUNT TRANSFERRED IN : $ %10.2lf\n\n", transinTotal);
		for (int i = 0; i < 79; i++) fprintf(outf, "-");

		//withdrawal transactions
		fprintf(outf, "\n  WITHDRAWAL HISTORY\n  ==================\n\n  DATE & TIME\t\tAMOUNT WITHDREW\n  -----------\t\t---------------\n  ");
		for (int i = 0; i < wdCount; i++) {
			if (strcmp(wd[i].accNo, currCust->accNo) == 0 && wd[i].t.yr == yr&&wd[i].t.mth == mth) {
				wdTotal += wd[i].amt;
				printTime(outf, wd[i].t);
				fprintf(outf, "   $ %10.2lf\n  ", wd[i].amt);
			}
		}
		fprintf(outf, "\n\t>>> NET AMOUNT WITHDREW : $ %10.2lf\n\n", wdTotal);
		for (int i = 0; i < 79; i++) fprintf(outf, "-");

		//fund transfers transactions
		fprintf(outf, "\n  FUND TRANSFERS HISTORY\n  ======================\n\n  DATE & TIME\t\tAMOUNT TRANSFERRED\tRECIPIENT ACC.\n  -----------\t\t------------------\t--------------\n  ");
		for (int i = 0; i < transCount; i++) {
			if (strcmp(trans[i].base.accNo, currCust->accNo) == 0 && trans[i].base.t.yr == yr&&trans[i].base.t.mth == mth) {
				transTotal += trans[i].base.amt;
				printTime(outf, trans[i].base.t);
				fprintf(outf, "   $ %10.2lf\t\t%s\n  ", trans[i].base.amt, trans[i].recAccNo);
			}
		}
		fprintf(outf, "\n\t>>> NET AMOUNT TRANSFERRED : $ %10.2lf\n\n", transTotal);
		for (int i = 0; i < 79; i++) fprintf(outf, "-");
		totalIn = dpTotal + chqdpTotal + transinTotal;
		totalOut = wdTotal + transTotal;
		fprintf(outf,
			"\n\tNET AMOUNT DEPOSITED \t: $ %10.2lf\n\tNET CHEQUE DEPOSITED \t: $ %10.2lf\n\tNET AMOUNT TRANSF. IN \t: $ %10.2lf\n"
			"\n\tNET AMOUNT WITHDREW \t: $ %10.2lf\n\tNET AMOUNT TRANSFERRED \t: $ %10.2lf\n"
			"\n\tNET CASH IN\t\t: $ %10.2lf\n\tNET CASH OUT\t\t: $ %10.2lf\n"
			"\n\tNET %s FOR THIS MONTH\t: $ %10.2lf\n\n",
			dpTotal, chqdpTotal, transinTotal, wdTotal, transTotal, totalIn, totalOut, ((totalIn - totalOut) < 0) ? "OUT" : "IN", abs(totalIn - totalOut));

		//print the content in the entire file onto the console
		rewind(outf);
		while (fscanf(outf, "%c", &c) != EOF) printf("%c", c);
		confirm = validIpt("PRINT A COPY OF THE REPORT?");
		fclose(outf);

		//prompt for user to print the report or not : Y -> retain the file / N -> delete the file
		if (confirm == 'Y') printf("\n  \"%s\" HAS BEEN CREATED\n  PLEASE RETRIEVE YOUR HARDCOPY AT THE COUNTER\n\n", filename);
		else remove(filename);
		readKey();
	}
}