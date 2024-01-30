
#include "Telecom.h"

/******************************
 * READ RECORDS FROM FILE 
 *         AND 
 *   CREATE LINKED LIST
*******************************/
CustInfo *readRecords(char *fileName) {
    FILE *fptr;
    CustInfo *node, *temp = NULL;

    fptr = open_file(fileName, READ);
    while(1) {
    	node = (CustInfo *)allocMemory(sizeof(CustInfo));
    	if(fread(node, sizeof(*node), 1, fptr) != 1) {
    		free(node);
    		break;
    	}
        node->next = NULL;
        if(Head == NULL) {
            temp = Head = node;
            continue;
        }
        temp->next = node;
        node->prev = temp;
        temp = node;
    }
    fclose(fptr);
    return Head;
}
/***********************************
 * READ EACH NODE FROM LINKED LIST
 * WRITE TO FILE AND FREE MEMORY
*************************************/
void writeRecords(char *fileName) {
    FILE *fptr;
    CustInfo *node, *temp = NULL;

    fptr = open_file(DATA_FILE, WRITE);
    node = Head;
    while(node != NULL) {
        fwrite(node, sizeof(*node), 1, fptr);
        temp = node;
        node = node->next;
        Head = node;
        free(temp);
    }
    fclose(fptr);
}

CustInfo *newRecord(CustInfo *Head, CustInfo *data) {
    CustInfo *node;

    if(Head == NULL) {
        data->prev = NULL;
        data->next = NULL;
        Head = data;
        return Head;
    }
    node = Head;
    while(node->next != NULL) {
        node = node->next;
    }
    data->prev = node;
    data->next = NULL;
    node->next = data;
    return Head;
}

CustInfo *searchRecord(CustInfo *Head, unsigned long int phoneNum, unsigned int customerID) {
    CustInfo *node = Head;

    if(phoneNum) {
        while(node != NULL) {
            if(node->phoneNum == phoneNum) {
                return node;
            }
            node = node->next;
        }
    } else if(customerID) {
        while(node != NULL) {
            if(node->customerID == customerID) {
                return node;
            }
            node = node->next;
        }
    }
    return NULL;
}

void listRecords(CustInfo *Head) {
    CustInfo *node = Head;

    printf("Inside list records\n");
    while(node != NULL) {
        unsigned int *dueDate = node->dueDate;
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("| CustomerID: %d | Phone: %ld | Plan: %s | Due: %.2f | Due Date: %d-%d-%d |\n",
                node->customerID, node->phoneNum, planName[node->plan-1], node->due, dueDate[0], dueDate[1], dueDate[2]);
        //printf("--------------------------------------------------------------------------------------------------------\n");
        node = node->next;
    }
}
