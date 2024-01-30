
#include "Telecom.h"

/***********************************
 * CREATE RECORD FOR NEW CUSTOMER
************************************/
CustInfo *createRecord(CustInfo *Head, char entity) {
    CustInfo *node;
    Login user;

    node = (CustInfo *)allocMemory(sizeof(CustInfo));
    node->addr = (char *)allocMemory(320);
    /*********************
     * Request User Info
    **********************/
   if(entity == CUSTOMER) {
        printf("First Name: ");
        fscanf(stdin, "%s", node->first_name);
        printf("Last Name: ");
        fscanf(stdin, "%s", node->last_name);
        printf("Age: ");
        fscanf(stdin, "%hd", &node->age);
        printf("Address: ");
        fscanf(stdin, "%s", node->addr);
        node->phoneNum   = createPhoneNum(Head, node);
        selectPlan(node);
        if(makePayment(node)) {
            return node;
        }
   }
   printf("New Password[Max 10 characters]: ");
   fscanf(stdin, "%s", user.passwd);

   user.customerID = customerID();
   node->customerID = user.customerID;
   user.entity     = entity;
   createLogin(user);
   /***********************************************
    * Add node to the linked list
   ************************************************/
  if(entity == CUSTOMER) {
    Head = newRecord(Head, node);
  }
  printf("--------------------------------------------------------------------------------------------------------\n");
  printf("Name: %s %s, PhoneNumber: %ld, CustomerID: %ud\n", node->first_name, node->last_name, node->phoneNum, node->customerID);
  printf("--------------------------------------------------------------------------------------------------------\n");
  return Head;
}

void createLogin(Login user) {
    FILE *fptr;
    
    fptr = open_file(LOGIN_FILE, APPEND);
    fwrite(&user, sizeof(user), 1, fptr);
    fclose(fptr);
}
/********************************
 * LOGIN FUNCTIONALITY FOR 
 * BOTH EMPLOYEES AND CUSTOMERS
*********************************/
unsigned int loginPage(char entity) {
    FILE *fptr;
    Login user;
    unsigned int customerID;
    unsigned char passwd[32] = {'\0'};

    fptr = open_file(LOGIN_FILE, READ);
    printf("Customer ID: ");
    fscanf(stdin, "%ud", &customerID);flushBuffer();
    do {
        if(fread(&user, sizeof(user), 1, fptr) != 1) {
            printf("Customer ID does not exist\n");
            return FAIL;
        }
    } while(user.customerID != customerID || user.entity != entity);
    for(int i = 0; i < 3; i++) {
        printf("Password[Max 10 characters]: ");
        fscanf(stdin, "%s", passwd);
        if(!strcmp(user.passwd, passwd)) {
            fclose(fptr);
            return user.customerID;
        }
    }
    fclose(fptr);
    return FAIL;
}

unsigned long int customerID(void) {
    if(IDref == 0) {
        IDref = 40001;
        return IDref;
    }
    IDref += 1;
    return IDref;
}

unsigned long int createPhoneNum(CustInfo *Head, CustInfo *node) {
    unsigned long int phno;

    while(1) {
        phno = createRandomNum(10);
        if(!searchRecord(Head, phno, 0)) {
            return phno;
        }
    }
}
