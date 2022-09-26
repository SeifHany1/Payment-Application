#ifndef app
#define app
#include "server.h"
uint8_t Choice;

void appStart(void);

void MENU(void) {

    printf("\n");
    printf("\n 1) For Reporting and Blocking Your Stolen Card Please Press [ 1 ] then Enter \n");
    printf("\n 2) For Logging Out Please Press [ 0 ] then Enter \n");
    printf("\n 3) To Continue Please Press on any Button then Enter \n");
    printf("\n YOUR CHOICE IS :  ");
    scanf(" %c", &Choice);


};

#endif