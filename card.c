#include "card.h"
#include <stdio.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	printf("Please Enter The Card Holder Name : ");
	gets(cardData->cardHolderName);
	printf("\n");

	if (strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) == NULL) {
	//	printf("wrong");
		return WRONG_NAME;
	}
	else
	//	printf("correct");
		return OK;

};

 EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	printf("Please Enter The Card Expiry Date in ( MM/YY ) Format : ");

	gets(cardData->cardExpirationDate);

	if (strlen(cardData->cardExpirationDate) != 5 || strlen(cardData->cardExpirationDate) == NULL) {         // No. of digits check
		return WRONG_EXP_DATE;
	}

	else if (((cardData->cardExpirationDate[0] == '0' && cardData->cardExpirationDate[1] > '0' && cardData->cardExpirationDate[1] < '10') || (cardData->cardExpirationDate[0] == '1' && cardData->cardExpirationDate[1] >= '0' && cardData->cardExpirationDate[1] < '3')) && (cardData->cardExpirationDate[2] == '/')) {      // Month Format Check
		return OK;
	}
	else
	return WRONG_EXP_DATE;
};



 EN_cardError_t getCardPAN(ST_cardData_t* cardData) {

	 printf("Please Insert The PAN Number  : ");

	 gets(cardData->primaryAccountNumber);

	 if ( strlen(cardData->primaryAccountNumber) > 19 || strlen(cardData->primaryAccountNumber) <16 )
	 {
		// printf("ERROR PAN less than 16 or greater than 19 numbers");

		 return WRONG_PAN;
	 }
	 for (int i = 0; i < strlen(cardData->primaryAccountNumber); i++)
	 {
		 if ((cardData->primaryAccountNumber[i]) >= '0' && (cardData->primaryAccountNumber[i]) <= '9') {

			 continue;

		 }

		 else {
			// printf("Character found");

			 return WRONG_PAN;
		 }
	 }
	// printf("DONE");

		 return OK;
};


