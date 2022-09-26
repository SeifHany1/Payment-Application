#include "app.h"
void appStart(void)
{
    uint8_t flag = 0;
    ST_cardData_t CardData;
    ST_cardData_t* CardDataPtr;
    CardDataPtr = &CardData;
    ST_terminalData_t TermData;
    ST_terminalData_t* TermDataPtr;
    TermDataPtr = &TermData;
    ST_transaction_t transData;
    ST_transaction_t* transDataPtr;
    transDataPtr = &transData;
    transData.transactionSequenceNumber=0;
    int MAX;

    getAccountData(); // To get the Client's PAN before App. Start
    getBalanceData(); // To get the Client's Balance before APP. Start

    while (1)
    {

        printf("\n\n                    --------------------------------- Welcome  --------------------------------- \n\n", CardDataPtr->cardHolderName);

        printf("\n");
        transData.transState = 0;
        // Get Card Data
        //Name
        flag = getCardHolderName(CardDataPtr);
        if (flag != 0)
        {
            printf("\n                                        ERROR : Wrong Name Format . PLEASE TRY AGAIN... \n");
            transData.transState = 2;
            continue;
        }
        flag = 0;
        //Expiry Date
        flag = getCardExpiryDate(CardDataPtr);
        if (flag != 0)
        {
            printf("\n                                  ERROR : Wrong Expiration Date format . PLEASE TRY AGAIN... \n");
            transData.transState = 2;
            continue;
        }
        printf("\n");
        flag = 0;
        //PAN and Check its validity
        flag = isValidCardPAN(CardDataPtr);
        if (flag != 0)
        {
            printf("\n                                      ERROR : Wrong PAN Format . PLEASE TRY AGAIN... \n");
            transData.transState = 2;
            continue;
        }
        flag = 0;
        system("cls");    // A NEW CLEAN WINDOW    
        Client = 0;
        MAX = 0;
        while (Client == 0) {



            //Check Account Validity
            flag = isValidAccount(CardDataPtr);
            if (flag != 0)
            {
                transData.transState = 2;
                continue;
            }
            flag = 0;

            //get Transaction Date
            getTransactionDate(TermDataPtr);
            //Printing Transaction Date 
            printf("                                                Today's Date ");

            printf("%s\n\n", TermData.transactionDate);

            //check expiration date
            flag = isCardExpired(CardData, TermData);
            // if card is expired -> exit
            if (flag != 0)
            {
                printf("                                       ERROR : Your Card is Expired , PLEASE TRY ANOTHER CARD... \n");
                Client = 1;
                continue;

            }
            else
            {
                printf("           ------------------------ Welcome %s , Your Card is Verefied ------------------------ \n\n", CardDataPtr->cardHolderName);
                flag = 0;
            }

            // MENU

            MENU();

            if (Choice == '1') {

                transDataPtr->transState = 2;
                printf("\n\n                                        Your Card Has Been Blocked Successfully !! \n\n");
                continue;
            }
            else if (Choice == '0') {

                Client = 1;
                continue;
            }
            else
            {
                //Set Max Amount
                if (MAX == 0) {
                    flag = setMaxAmount(TermDataPtr);
                    MAX++;
                    if (flag != 0)
                    {

                        continue;
                    }
                }
                // Get transaction amount 
                flag = getTransactionAmount(TermDataPtr);
                if (flag != 0)
                {
                    continue;
                }
                transDataPtr->terminalData.transAmount = TermDataPtr->transAmount;
                transDataPtr->terminalData.maxTransAmount = TermDataPtr->maxTransAmount;

                // Check Below Max
                flag = isBelowMaxAmount(TermDataPtr);
                if (flag != 0)
                {
                    transData.transState = 2;
                    continue;
                }
                //Check Amount Availability
                flag = isAmountAvailable(TermDataPtr);
                if (flag != 0)
                {
                    transData.transState = 1;
                    continue;
                }
                //Check if card is stolen or not
                flag = recieveTransactionData(transDataPtr);
                if (flag != 0)
                {
                    continue;
                }
                flag = 0;
                // Save Transaction
                if (Choice == '1') {
                    transDataPtr->transState = 2;
                }
                    flag = saveTransaction(transDataPtr);
                    transData.transactionSequenceNumber++;
                

                printf("\n\n\n            ---------------------   PLEASE WAIT WHILE YOU REDIRECTED TO YOUR HOME SCREEN   --------------------- \n\n\n\n\n");
                for (int delay = 0; delay < 2000000000; delay++);
            
            }
        }
    }
    };
void main()
{
    appStart();
}
