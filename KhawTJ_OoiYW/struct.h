#pragma once
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
	int lock;
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

