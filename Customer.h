#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef struct {
  char customerName[51]; // Mandatory, max 50 chars
  char customerAddress[101]; // Mandatory, max 100 chars
  char customerCity[101]; // Mandatory, max 100 chars
  char customerProvince[3]; // Mandatory, exactly 2 chars (Canada Post abbrev)
  char customerPostalCode[7]; // Mandatory, ANANAN format, 
  char customerPhone[13]; // Mandatory, format ###-###-#### 
  char customerEmail[51]; // Mandatory, max 50 chars
  int customerID; // Mandatory, > 0
  float customerCreditLimit; // Mandatory, > 0.00
  float currentAccountBalance; // Mandatory, >= 0.00
  char lastPaymentMade[11]; // Optional, YYYY-MM-DD format 
  char customerJoinDate[11]; // Mandatory, YYYY-MM-DD format 
} Customer;

#endif 