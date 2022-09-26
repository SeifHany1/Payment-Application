#include "server.h"
#include <stdio.h>
#include<string.h>
#define LSIZ 21   
#define RSIZ 256  // no. of lines

struct ST_transaction_t transactions[255] = { 0 };
uint32_t Transaction_Number = 0;
int PAN_FOUND_LINE = 0;
void getAccountData(void) {

    char line[RSIZ][LSIZ];
    int i = 0, tot = 0 ;
    char* fname = "PAN_DATA.txt";
    FILE* fptr = fopen(fname, "r");

    while (fgets(line[i], LSIZ, fptr))
    {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;

    printf("\nChecking PAN DATA in Progress , Please wait ... \n", fname);

    for (i = 0; i < tot; ++i)
    {
        strcpy(accounts[i].primaryAccountNumber , line[i]);

    }
};
void getBalanceData(void) {

    float line[RSIZ][LSIZ];
    int i = 0, tot = 0;
    float* fname = "BALANCE_DATA.txt";
    FILE* fptr = fopen(fname, "r");

    while (fscanf(fptr,"%f", line[i]))
    {
        line[i][LSIZ] ;
        i++;
    }
    tot = i;
    printf("\nChecking BALANCE DATA in Progress , Please wait ... \n\n", fname);
	for (i = 0; i < tot; ++i)
	{
		accounts[i].balance = *line[i];
	}

};


EN_transState_t recieveTransactionData(ST_transaction_t* transData) {

	if ((transData->transState) == 1)
	{
		printf("DECLINED INSUFFECIENT FUND \n");
		return DECLINED_INSUFFECIENT_FUND;
	}
	else if ((transData->transState) == 2)
	{
		printf("     *******************************************  DECLINED STOLEN CARD  ******************************************* \n");
		return DECLINED_STOLEN_CARD;
	}
	else if ((transData->transState) == 3)
	{
		printf("INTERNAL_SERVER_ERROR \n");
		return INTERNAL_SERVER_ERROR;
	}
    return APPROVED;
};

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	int i = 0, j = 0;

	for (i = 0; i < 256; i++)
	{
		if ((strcmp((cardData->primaryAccountNumber), (accounts[i].primaryAccountNumber))) == 0)
		{
			PAN_FOUND_LINE = i;
			printf("\n                          [[ Your PAN was found in %d th line in ( PAN_DATA.txt ) Data Base ]] \n\n", i+1);

			return OK;
		}
		
		else if ((strcmp((cardData->primaryAccountNumber), (accounts[i].primaryAccountNumber))) != 0)
		{
			j++;
			if (j > 255)
			{
				printf("\n                         Your PAN was not found in ( PAN_DATA.txt ) Data Base , Please try again... \n\n");
				Client = 1;

				return ACCOUNT_NOT_FOUND;
			}
		}
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{

	if (termData->transAmount > accounts[PAN_FOUND_LINE].balance)
	{
		printf("                         Your Balance is : %f LE  , You have Insufficient Balance !! . PLEASE TRY AGAIN... \n\n" , accounts[PAN_FOUND_LINE].balance);
		return LOW_BALANCE;
	}
	else
	{
		printf("                         Your Balance is : %f LE  , You have Sufficient Balance \n\n" , accounts[PAN_FOUND_LINE].balance);
		return OK;
	}
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData){
{

	printf("                                    This is The Transaction Number (%d) \n\n", transactionSequenceNumber + 1);
	if ((transactionSequenceNumber >= 255) || (transactionSequenceNumber < 0))
	{
		printf("Save Failed");
		return SAVING_FAILED;
	}
	else if (transData->transactionSequenceNumber == transactionSequenceNumber )
	{		
		transData->terminalData.maxTransAmount = accounts[PAN_FOUND_LINE].balance;

		transData->terminalData.maxTransAmount = ((transData->terminalData.maxTransAmount) - (transData->terminalData.transAmount)); // Update Max Amount

		accounts[PAN_FOUND_LINE].balance = accounts[PAN_FOUND_LINE].balance - (transData->terminalData.transAmount) ; 		// Update Client's Balance

		printf("                          Transaction Done , Your Balance has been Updated... \n\n\n");

		printf("---------------------------> Your New Balance is %f \n\n", accounts[PAN_FOUND_LINE].balance);

		printf("\n                          The New Max. Transaction Amount is %f \n\n", transData->terminalData.maxTransAmount );

		Transaction_Number++;

		return OK;
	}
	return SAVING_FAILED;  
}}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	uint8_t flag = 0;
	
	if ((transData->transState) == 1)
	{
		printf("\n                                     DECLINED INSUFFECIENT FUND \n");
		return SAVING_FAILED;
	}
	else if ((transData->transState) == 2)
	{
		printf("\n                                     DECLINED STOLEN CARD \n");
		return SAVING_FAILED;
	}
	else if ((transData->transState) == 3)
	{
		printf("\n                                     INTERNAL_SERVER_ERROR \n");
		return SAVING_FAILED;
	}
	flag = getTransaction(Transaction_Number, transData);

	if (flag == 1)
	{
		printf("\n                                ERROR : Reached Maximum Transactions . PLEASE TRY AGAIN... \n");
		return SAVING_FAILED;
	}
	else {
		transactions[Transaction_Number] = *transData;  // Update Transaction
		return OK;
	}
}
