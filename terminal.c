#include"terminal.h"
#include <stdio.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    //Get today's Date from PC / LAPTOP in ( DD/MM/YY ) Format

    time_t Date;
    Date = time(NULL);
    struct tm time = *localtime(&Date);

    // Get Days in Correct Format ( Left )

    if (time.tm_mday > 9)
    {

        termData->transactionDate[0] = (time.tm_mday / 10) + '0';  

        termData->transactionDate[1] = (time.tm_mday % 10) + '0';
    }
    else
    {
        termData->transactionDate[0] = 0 + '0';

        termData->transactionDate[1] = time.tm_mday + '0';
    }

    // Add Slash

    termData->transactionDate[2] = '/';

    // Get Months in Correct Format ( Middle )

    if ((time.tm_mon + 1) > 9)
    {
        termData->transactionDate[3] = ((time.tm_mon + 1) / 10) + '0';

        termData->transactionDate[4] = ((time.tm_mon + 1) % 10) + '0';
    }
    else
    {
        termData->transactionDate[3] = 0 + '0';

        termData->transactionDate[4] = (time.tm_mon + 1) + '0';
    }
    // Add Slash

    termData->transactionDate[5] = '/';

    // Get Years in Correct Format ( Right )
    {
        termData->transactionDate[6] = ((time.tm_year + 1900) / 1000) + '0';

        termData->transactionDate[7] = (((time.tm_year + 1900) - 2000) / 100) + '0';

        termData->transactionDate[8] = (((time.tm_year + 1900) - 2000) / 10) + '0';

        termData->transactionDate[9] = ((time.tm_year + 1900) % 10) + '0';

        termData->transactionDate[10] = '\0';
    }


    return OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    // [ YEAR CHECK ] 1st y in ( mm/yy ) must be greater than 3rd y in  ( dd/mm/yyyy ) 
    if (cardData.cardExpirationDate[3] < termData.transactionDate[8]) {
        return EXPIRED_CARD;
    }

    // [ YEAR CHECK ] 2nd y in ( mm/yy ) must be greater than 4th y in  ( dd/mm/yyyy ) 
    else if ((cardData.cardExpirationDate[3] == termData.transactionDate[8]) && (cardData.cardExpirationDate[4] < termData.transactionDate[9])) {
        return EXPIRED_CARD;
    }

    // [ MONTH CHECK ] 1st m in ( mm/yy ) must be greater than 1st m in  ( dd/mm/yyyy ) 

    else if ((cardData.cardExpirationDate[4] == termData.transactionDate[9]) && (cardData.cardExpirationDate[0] < termData.transactionDate[3])) {
        return EXPIRED_CARD;
    }

    // [ MONTH CHECK ] 2nd m in ( mm/yy ) must be greater than 2nd m in  ( dd/mm/yyyy ) 

    else if ((cardData.cardExpirationDate[4] == termData.transactionDate[9]) && (cardData.cardExpirationDate[0] == termData.transactionDate[3]) && (cardData.cardExpirationDate[1] < termData.transactionDate[4])) {
        return EXPIRED_CARD;
    }

    else
    {
        return OK;
    }
}
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    if (getCardPAN(cardData) == OK)
    {
        return OK;
    }
    else
    {
        return INVALID_CARD;
    }
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    printf("Please Enter The Transaction Amount : ");

    scanf("%f", &termData->transAmount);

    if ((termData->transAmount) <= 0)
    {
        printf("\n                                      ERROR : Wrong Transaction Amount . PLEASE TRY AGAIN  \n");

        return INVALID_AMOUNT;
    }
    else
    {
        return OK;
    }
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    if ((termData->transAmount) > (termData->maxTransAmount))
    {



        printf("\n                                      ERROR : Transaction Exceeded Max. Amount . PLEASE TRY AGAIN \n");
        return EXCEED_MAX_AMOUNT;
    }
    else
    {
        printf("\n             ------------------------> Transaction Amount Accepted , Checking Your Balance $ <------------------------- \n\n");

        return OK;
    }
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    printf("\n\nPlease Enter The Maximum Transaction Amount : ");

    scanf("%f", &termData->maxTransAmount);

    if ((termData->maxTransAmount) <= 0)
    {
        printf("ERROR , Maximum Amount Can't be 0 \n");
        return INVALID_MAX_AMOUNT;
    }
    else
    {
        printf("\n             ---------------------------> Maximum Amount Has Been Set Successfully <--------------------------- \n\n");

        return OK;
    }
}