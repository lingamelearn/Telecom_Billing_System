
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
