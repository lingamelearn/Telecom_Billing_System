/*
    Program for telecommunication billing system
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include "Telecom.h"
/*****************************************************************************************************/
#define     SUCCESS                         0
#define     FAIL                            1
#define     QUIT                            3

#define     INSTALL_CHARGE                  1200
#define     LOGIN_FILE                      "loginfile.dat"
#define     DATA_FILE                       "custdata.dat"
#define		TEMP_FILE						"tempfile.dat"

#define     FULLPAY                         1
#define     MINPAY                          2

#define     READ                            "rb"
#define     WRITE                           "wb"
#define     APPEND                          "ab"
#define     UPDATE                          "rb+"

#define     EMPLOYEE                        'E'
#define     CUSTOMER                        'C'

struct LoginData {
    unsigned int customerID;
    unsigned char passwd[32];
    unsigned char entity;
};
typedef struct LoginData Login;

struct CustomerInfo {
	struct CustomerInfo *prev;
    unsigned int customerID;
    char first_name[32];
    char last_name[32];
    unsigned short int age;
    char *addr;
    float monthCharge;
    unsigned int dueDate[3];
    float due;
    unsigned long int phoneNum;
    unsigned short int plan;
    struct CustomerInfo *next;
};
typedef struct CustomerInfo CustInfo;
/*
struct Node {
    struct Node *prev;
    CustInfo user;
    struct Node *next;
};
typedef struct Node Node;
*/


CustInfo *readRecords(char *fileName);
void writeRecords(char *fileName);
CustInfo *newRecord(CustInfo *Head, CustInfo *data);
CustInfo *searchRecord(CustInfo *Head, unsigned long int phoneNum, unsigned int customerID);
void listRecords(CustInfo *Head);
CustInfo *deleteRecord(CustInfo *Head, unsigned int customerID);
void deleteLogin(unsigned int customerID);
CustInfo *createRecord(CustInfo *Head, char entity);
void createLogin(Login user);
unsigned int loginPage(char entity);
unsigned long int customerID(void);
unsigned long int createPhoneNum(CustInfo *Head, CustInfo *node);
unsigned long int createRandomNum(unsigned int size);
unsigned int selectPlan(CustInfo *node);
void changeDetails(CustInfo *node);
int makePayment(CustInfo *node);
void viewPayment(CustInfo *node);
void setDate(unsigned int *dueDate, unsigned short int mode);
void *allocMemory(size_t sz);
FILE *open_file(char *fileName, char *mode);
void homePage(CustInfo *node);
void employeePage(void);
void flushBuffer(void);
unsigned int newacc_login(char entity);
void coreInit(void);

const char *planName[5] = {"Basic Plan", "General Plan", "Regular Plan", "Advanced Plan", "Premium Plan"};
CustInfo *Head = NULL;
unsigned int IDref = 0;
/*************************************************************************************************************/
int main(void) {
    char entity;
    int choice;
    unsigned int customerID = 0;
    CustInfo *node = {0};

    coreInit();
    while(1) {
    	printf("1.Customer\n");
		printf("2.Employee\n");
		printf("3.Quit\n");
		printf("Choose from above: ");
		fscanf(stdin, "%d", &choice);flushBuffer();
		if(choice == 1) {
			entity = CUSTOMER;
		} else if(choice == 2) {
			entity = EMPLOYEE;
		} else if(choice == 3) {
			writeRecords(DATA_FILE);
			break;
		} else {
			printf("Wrong Option ... Try Again\n");
			continue;
		}
		customerID = newacc_login(entity);
        if(customerID) {
			if(entity == CUSTOMER) {
				node = searchRecord(Head, 0, customerID);
				homePage(node);
			} else if(entity == EMPLOYEE) {
				employeePage();
			}
        }
    }
    return 0;
}

void coreInit(void) {
	FILE *fptr;
	Login user;

	Head = readRecords(DATA_FILE);
	fptr = open_file(LOGIN_FILE, READ);
	if(fptr) {
		fseek(fptr, -(long)sizeof(Login), SEEK_END);
		fread(&user, sizeof(user), 1, fptr);
		IDref = user.customerID;
	}
}

unsigned int newacc_login(char entity) {
	int choice = 0;
	unsigned int customerID = 1;

	while(1) {
		printf("1.Create New Account\n");
		printf("2.Login\n");
		printf("3.Main menu\n");
		printf("Choose from above: ");
		fscanf(stdin, "%d", &choice);

		if(choice == 1) {
			Head = createRecord(Head, entity);
			return 0;
		} else if(choice == 2) {
			if((customerID = loginPage(entity)) != FAIL) {
				printf("Login Successful\n");
				return customerID;
			}
		} else if(choice == 3) {
			break;
		}
		printf("Wrong Credentials...Try Again\n");
	}
	return 0;
}

void employeePage(void) {
    int choice = 0;
    unsigned int customerID;
    unsigned long int phoneNum;

    while(1) {
        printf("1.List all Records\n");
        printf("2.View Payment Details\n");
        printf("3.Delete Records\n");
        printf("4.LogOut\n");
        printf("Choose from above: ");
        fscanf(stdin, "%d", &choice);
        switch(choice) {
            case 1:
                    listRecords(Head);
                    break;
            case 2:
            		printf("Enter a Phone Number: ");
            		fscanf(stdin, "%ld", &phoneNum);
                    viewPayment(searchRecord(Head, phoneNum, 0));
                    break;
            case 3:
                    printf("Enter Customer ID to delete record: ");
                    fscanf(stdin, "%d", &customerID);
                    deleteRecord(Head, customerID);
                    break;
            case 4:
            		return;
            default:
            	printf("Wrong Choice...Choose again\n");
        }
    }
}

void homePage(CustInfo *node) {
    int choice = 0;

    while(choice != 4) {
    	printf("1.Change Address\n");
		printf("2.Change Plan\n");
		printf("3.Make Payment\n");
		printf("4.LogOut\n");
		printf("Choose from above: ");
		fscanf(stdin, "%d", &choice);
		switch(choice) {
			case 1:
					changeDetails(node);
					break;
			case 2:
					if(selectPlan(node) == FAIL) {
						break;
					} // @suppress("No break at end of case")
			case 3:
					makePayment(node);
					break;
			case 4:
					return;
			default:
					printf("Wrong Choice...Choose again\n");
					break;
		}
    }
}

/*****************************************************************************************************/
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
/***********************************************************************************************************************/
unsigned int selectPlan(CustInfo *node) {
    unsigned short int newPlan;
    unsigned int plans[5][3] = {
                                {300, 28, 350},
                                {450, 28, 500},
                                {700, 28, 870},
                                {1150, 30, 1200},
                                {1500, 30, 1560}
                            };

    while(1) {
        for(int i = 0; i < 5; i++) {
            printf("%d.%s     -  %d min, %d days, Rs.%d/mo\n", i+1, planName[i], plans[i][0], plans[i][1], plans[i][2]);
        }
        printf("Choose from above Plans[Enter 0 to cancel]: ");
        fscanf(stdin, "%hd", &newPlan);
        if(!newPlan) {
            return FAIL;
        }
        /*** FOR NEW CUSTOMER - INSTALLATION COSTS ***/
        if(!node->plan) {
            node->due = INSTALL_CHARGE;
        }
        printf("Entered Select Plan\n");
        if(node->plan != newPlan) {
            node->plan = newPlan;
            node->monthCharge = plans[newPlan][2];
            node->due  += node->monthCharge;
            return SUCCESS;
        }
        printf("Plan already active...Choose a different Plan\n");
    }
}
/******************************************************
 **********************************************
 *      MODIFY DATA IN EXISTING RECORD
*******************************************************
**********************************
 * CHANGE ADDRESS OF CUSTOMER
***********************************/
void changeDetails(CustInfo *node) {
	char *addr = (char *)allocMemory(320), *temp = NULL;
    printf("Enter new address: \n\t");
    fscanf(stdin, "%s", addr);
    temp = node->addr;
    //free(temp);
    node->addr = addr;
    printf("New Address updated\n");
}
/****************************************
 * VIEW & MAKE MONTHLY PAYMENT FOR PLAN
*****************************************/
void viewPayment(CustInfo *node) {
	if(node == NULL) {
		printf("Wrong Input...Try again\n");
		return;
	}
    unsigned int *dueDate = node->dueDate;

    printf("View Payment details below -->\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("| Name: %s %s | Plan: %s | Due: %.2f | Due Date: %d-%d-%d |\n",
            node->first_name, node->last_name, planName[node->plan-1], node->due, dueDate[0], dueDate[1], dueDate[2]);
    printf("--------------------------------------------------------------------------------------------------------\n");
}

int makePayment(CustInfo *node) {
    int pay = 0;

    viewPayment(node);
    printf("1.Full Payment\n");
    printf("2.Minimum Payment\n");  //  Payment --> 30% of total amount to pay
    printf("Choose an Option from above: ");
    fscanf(stdin, "%d", &pay);
    if(pay == FULLPAY) {
        node->due = node->monthCharge;
        printf("Full Payment made\n");
    } else if(pay == MINPAY) {
        node->due += (node->monthCharge > node->due) ? node->monthCharge - (node->due/30) * 100 : (node->due/30) * 100 - node->monthCharge;
        printf("Minimum payment of %.2f is Paid\n", (node->monthCharge/30) * 100);
    } else {
        printf("Payment Cancelled...Redirecting to Home Page\n");
        return FAIL;
    }
    setDate(node->dueDate, pay);
    return SUCCESS;
}
/**************************
 * DELETE SELECTED RECORD
***************************/
CustInfo *deleteRecord(CustInfo *Head, unsigned int customerID) {
    CustInfo *node, *temp;

    node = Head;
    while(node != NULL) {
    	if(node->customerID == customerID) {
    		temp = node;
    		node = node->next;
    		if(temp == Head) {
    			Head = node;
    			Head->prev = NULL;
    		} else {
    			node->prev = temp->prev;
    			temp->prev->next = temp->next;
    		}
    		free(temp);
    		deleteLogin(customerID);
    		break;
    	} else {
    		node = node->next;
    	}
    }
    return Head;
}

void deleteLogin(unsigned int customerID) {
	FILE *fptr, *tmp;
	Login user;

	/**************************************
	 * DELETE RELATED DATA FROM LOGIN FILE
	 **************************************/
	fptr = open_file(LOGIN_FILE, READ);
	tmp  = open_file(TEMP_FILE, WRITE);
	while(fread(&user, sizeof(user), 1, fptr) != 1) {
		if(user.customerID == customerID) {
			continue;
		}
		fwrite(&user, sizeof(user), 1, fptr);
	}
	fclose(fptr);
	fclose(tmp);
	remove(LOGIN_FILE);
	rename(TEMP_FILE, LOGIN_FILE);
}
/***************************************************************************************************************************************/
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
/***************************************************************************************************************************************/

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
/************************************************************************************************/
void flushBuffer(void) {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}











