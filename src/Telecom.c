/*
    Program for telecommunication billing system
*/

#include "Telecom.h"

const char *planName[5] = {"Basic Plan", "General Plan", "Regular Plan", "Advanced Plan", "Premium Plan"};
CustInfo *Head = NULL;
unsigned int IDref = 0;

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
