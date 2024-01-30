
#include "Telecom.h"

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
***********************************************
**********************************
 * CHANGE ADDRESS OF CUSTOMER
*******************************************************/
void changeDetails(CustInfo *node) {
	char *addr = (char *)allocMemory(320);//, *temp = NULL;
    printf("Enter new address: \n\t");
    fscanf(stdin, "%s", addr);
    //temp = node->addr;
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
