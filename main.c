#include <stdio.h>
#include <stdlib.h>
#include "Validation.h"
#include "FileIO.h"
#include "Customer.h"
#include "Part.h"
#include "Order.h"
#include "Constants.h"

void printCustomer(const Customer* customer);
void printPart(const Part* p);
void printOrder(const Order* order);

int main() {
  Customer *customers = (Customer*)malloc(CUSTOMERS_LIMIT * sizeof(Customer)); 
  Part* parts = (Part*)malloc(PARTS_LIMIT * sizeof(Part));
  Order* orders = (Order*)malloc(ORDERS_LIMIT * sizeof(Order));
  int customerCount = loadCustomers(customers, CUSTOMERS_FILE);
  int partCount = loadParts(parts, PARTS_FILE);
  // Order must be loaded after customers and parts
  int orderCount = loadOrders(orders, parts, partCount, customers, customerCount, ORDERS_FILE);
  printOrder(&orders[0]);
  
  free(customers);
  free(parts);
  free(orders);
}
void printCustomer(const Customer* customer) {
  printf("---------------------\n");
  printf("Name             : %s\n", customer->customerName);
  printf("Address          : %s\n", customer->customerAddress);
  printf("City             : %s\n", customer->customerCity);
  printf("Province         : %s\n", customer->customerProvince);
  printf("Postal Code      : %s\n", customer->customerPostalCode);
  printf("Phone            : %s\n", customer->customerPhone);
  printf("Email            : %s\n", customer->customerEmail);
  printf("Customer ID      : %d\n", customer->customerID);
  printf("Credit Limit     : %.2f\n", customer->customerCreditLimit);
  printf("Account Balance  : %.2f\n", customer->currentAccountBalance);
  if (customer->lastPaymentMade[0] != '\0')
    printf("Last Payment Made: %s\n", customer->lastPaymentMade);
  else {
    printf("Last Payment Made: %s\n", "N/A");
  }
  printf("Join Date        : %s\n", customer->customerJoinDate);
  printf("---------------------\n");
}
void printPart(const Part* p) {
  if (p == NULL) {
    printf("Invalid Part\n");
    return;
  }
  printf("---------------------\n");
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

  printf("---------------------\n");
}

void printOrder(const Order* order) {
  if (order == NULL) {
    printf("Invalid Order\n");
    return;
  }
  printf("---------------------\n");
  printf("Order ID      : %lld\n", order->orderID);
  printf("Order Date    : %s\n", order->orderDate);
  printf("Status        : %d\n", order->orderStatus);
  printf("Customer ID   : %d\n", order->customerID);
  printf("Total Amount  : $%.2f\n", order->orderTotal);
  printf("Distinct Parts: %d\n", order->distinctParts);
  printf("Total Parts   : %d\n", order->totalParts);
  for (int i = 0; i < order->distinctParts; i++) {
    printf("Part ID: %d, Quantity Ordered: %d\n",
           order->orderedParts[i].partID, order->orderedParts[i].quantityOrdered);
  }
  printf("---------------------\n");
}