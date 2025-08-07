#ifndef VALIDATION_H
#define VALIDATION_H

int validateCustomerFields(char** fields, int lineNumber);
int validateProvince(const char* province);
int validatePostalCode(const char* postalCode);
int validatePhoneNumber(const char* phoneNumber);
int validateEmail(const char* email);
int validateDate(const char* date);

int validatePartFields(char** fields, int lineNumber);
int validatePartLocation(const char* partLocation);
int validatePartStatus(char* quantityOnHand, char* partStatus);


int isInteger(const char* str);
int isNumber(const char* str);
int isValidDate(int year, int month, int day);

#endif 
