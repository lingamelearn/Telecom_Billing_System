
#include <time.h>
#include "Telecom.h"

unsigned long int createRandomNum(unsigned int size) {
    time_t t;
    unsigned long int phno;

    srand((unsigned)time(&t));
    for(int i = 0; i < size; i++) {
        phno += rand()%10;
        phno *= 10;
    }
    return phno;
}

void setDate(unsigned int *dueDate, unsigned short int mode) {
    time_t t;
    struct tm *dtTm;

    time(&t);
	dtTm = localtime(&t);
	dueDate[0] = dtTm->tm_mday;
	dueDate[1] = dtTm->tm_mon + 1;
	dueDate[2]  = dtTm->tm_year + 1900;
    if(mode == FULLPAY) {
        dueDate[1] += 1;
    } else if(mode == MINPAY) {
        dueDate[0] += 15;
        if(dueDate[0] > 30) {
            dueDate[0] = dueDate[0] - 30;
            dueDate[1] += 1;
        }
    }

    if(dueDate[1] > 12) {
            dueDate[1] = 1;
            dueDate[2] += 1;
    }
}

void *allocMemory(size_t sz) {
	return calloc(1, sz);
}

FILE *open_file(char *fileName, char *mode) {
    FILE *fptr;

    if((fptr = fopen(fileName, mode)) == NULL) {
        printf("Unable to access file\n");
        return NULL;
    }
    return fptr;
}

void flushBuffer(void) {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}
