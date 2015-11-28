#pragma once
//structure definition

//structure for time
typedef struct {
	int yr, mth, dy, hr, min, sec;
}TIME;

//structure for managers' details
typedef struct {
	char ID[7], passw[25], name[31];
}MANAGER;

//structure for both deposits/withdrawals details
typedef struct {
	TIME t;
	char accNo[6];
	int ATM, txnCode;
	double amt, prevBal, newBal;
}BASEINFO;

//structure for customers' details
typedef struct {
	char accNo[6], PIN[6];
	char name[36], gender, adds[29], state[14], hp[12];
	int lock;
	double bal;
	TIME lastTrans;
}CUSTOMER;

//structure for cheque deposits' details
typedef struct {
	BASEINFO base;
	char bankCode[10];
	char cheqNo[7];
	int check;
}CHEQUE;

//structure for transfer's details
typedef struct {
	BASEINFO base;
	char recAccNo[6];
	double recPrevBal, recNewBal;
}TRANSFER;

