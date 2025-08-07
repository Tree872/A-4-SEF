#ifndef VALIDATION_H
#define VALIDATION_H
#include "Customer.h"
#include "Part.h"
#include "Order.h"

int validateCustomerFields(char** fields, int lineNumber);
int validateProvince(const char* province);
int validatePostalCode(const char* postalCode);
int validatePhoneNumber(const char* phoneNumber);
int validateEmail(const char* email);
int validateDate(const char* date);

int validatePartFields(char** fields, int lineNumber);
int validatePartLocation(const char* partLocation);
int validatePartStatus(char* quantityOnHand, char* partStatus);

int validateOrderFields(char** fields, int numOfReadFields, int lineNumber, const Part* parts, int partCount, const Customer* customers, int customerCount);
int validateOrderID(const char* orderID);
int validateOrderStatus(char* orderStatus);
int validateCustomerIDInOrder(const char* customerID, const Customer* customers, int customerCount);
int validatePartIDInOrder(int partID, const Part* parts, int partCount);

int isInteger(const char* str);
int isNumber(const char* str);
int isValidDate(int year, int month, int day);

#endif 
