#include <stdio.h>
#include "Validation.h"
#include "FileIO.h"
#include "Customer.h"
#include "Part.h"
#include "Constants.h"

void printCustomer(const Customer* customer);
void printPart(const Part* p);

int main() {
  Part parts[10]; 

  int partCount = loadParts(parts, PARTS_FILE); 
  printf("Loaded %d parts from %s\n", partCount, PARTS_FILE);
  printPart(&parts[0]); 
  printPart(&parts[1]); 


}
void printCustomer(const Customer* customer) {
  printf("Customer Information:\n");
  printf("---------------------\n");
  printf("Name: %s\n", customer->customerName);
  printf("Address: %s\n", customer->customerAddress);
  printf("City: %s\n", customer->customerCity);
  printf("Province: %s\n", customer->customerProvince);
  printf("Postal Code: %s\n", customer->customerPostalCode);
  printf("Phone: %s\n", customer->customerPhone);
  printf("Email: %s\n", customer->customerEmail);
  printf("Customer ID: %d\n", customer->customerID);
  printf("Credit Limit: %.2f\n", customer->customerCreditLimit);
  printf("Account Balance: %.2f\n", customer->currentAccountBalance);
  printf("Last Payment Made: %s\n",
    (customer->lastPaymentMade[0] != '\0') ? customer->lastPaymentMade : "N/A");
  printf("Join Date: %s\n", customer->customerJoinDate);
  printf("\n");
}
void printPart(const Part* p) {
  if (p == NULL) {
    printf("Invalid Part\n");
    return;
  }

  printf("----- Part Information -----\n");
  printf("Part ID        : %d\n", p->partID);
  printf("Name           : %s\n", p->partName);
  printf("Number         : %s\n", p->partNumber);
  printf("Location       : %s\n", p->partLocation);
  printf("Cost           : $%.2f\n", p->partCost);
  printf("Quantity       : %d\n", p->quantityOnHand);

  printf("Status         : ");
  if (p->partStatus == 0) {
    printf("In Stock (Quantity > 100)\n");
  }
  else if (p->partStatus == 99) {
    printf("Low Stock (Quantity < 100)\n");
  }
  else if (p->partStatus < 0) {
    printf("Deficit (%d units short)\n", -p->partStatus);
  }
  else {
    printf("Unknown Status (%d)\n", p->partStatus);
  }

  printf("-----------------------------\n");
}