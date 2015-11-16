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
	char accNo[6];
	int ATM, txnCode;
	double amt, prevBal, newBal;
}BASEINFO;

typedef struct {
	char accNo[6], PIN[6];
	char name[36], gender, adds[29], state[14], hp[12];
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

