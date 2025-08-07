#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Validation.h"
#include "FileIO.h"
#include "Customer.h"
#include "Part.h"
#include "Order.h"
#include "Constants.h"

void printCustomer(const Customer* customer);
void printPart(const Part* p);
void printOrder(const Order* order);
void printCustomers(const Customer* customers, int count);
void printParts(const Part* parts, int count);
void printOrders(const Order* orders, int count);
void printMenu();
void promptInt(const char* prompt, int* input);
void flushInputStream();

int main() {
  Customer *customers = (Customer*)malloc(CUSTOMERS_LIMIT * sizeof(Customer)); 
  Part* parts = (Part*)malloc(PARTS_LIMIT * sizeof(Part));
  Order* orders = (Order*)malloc(ORDERS_LIMIT * sizeof(Order));
  int customerCount = 0;
  int partCount = 0;
  int orderCount = 0;

  while (1) {
    int choice;
    printMenu();
    promptInt("Enter your choice (1-5): ", &choice);
    switch (choice) {
      case 1: {
        customerCount = loadCustomers(customers, CUSTOMERS_FILE);
        partCount = loadParts(parts, PARTS_FILE);
        orderCount = loadOrders(orders, parts, partCount, customers, customerCount, ORDERS_FILE);
        printf("Loaded %d customers, %d parts, and %d orders.\n", customerCount, partCount, orderCount);
        break;
      }
      case 2: {
        printCustomers(customers, customerCount);
        break;
      }
      case 3: {
        printParts(parts, partCount);
        break;
      }
      case 4: {
        printOrders(orders, orderCount);
        break;
      }
      case 5: {
        free(customers);
        free(parts);
        free(orders);
        printf("Memory freed. Exiting program.\n");
        return 0;
      }
      default:
        printf("Invalid choice. Please choose between option 1-5.\n");
    }

  }
  
}
void printCustomer(const Customer* customer) {
  printf("Name         : %s\n", customer->customerName);
  printf("Address      : %s\n", customer->customerAddress);
  printf("Customer ID  : %d,           City : %s\n", customer->customerID, customer->customerCity);
  printf("Province     : %s,           Postal Code : %s\n", customer->customerProvince, customer->customerPostalCode);
  printf("Phone        : %s, Email : %s\n", customer->customerPhone, customer->customerEmail);
  printf("Credit Limit : %.2f,      Account Balance : %.2f\n", customer->customerCreditLimit, customer->currentAccountBalance);
  if (customer->lastPaymentMade[0] != '\0')
    printf("Last Payment Made : %s\n", customer->lastPaymentMade);
  else {
    printf("Last Payment Made : %s\n", "N/A");
  }
  printf("Join Date         : %s\n", customer->customerJoinDate);
  printf("---------------------\n");
}
void printPart(const Part* p) {
  if (p == NULL) {
    printf("Invalid Part\n");
    return;
  }
  printf("Name    : %s\n", p->partName);
  printf("Number  : %s\n", p->partNumber);
  printf("Status  : ");
  if (p->partStatus == 0) {
    printf("In Stock\n");
  }
  else if (p->partStatus == 99) {
    printf("Low Stock\n");
  }
  else if (p->partStatus < 0) {
    printf("Deficit (%d units short)\n", -p->partStatus);
  }
  printf("Part ID : %d,    \tLocation : %s\n", p->partID, p->partLocation);
  printf("Cost    : $%.2f, \tQuantity : %d\n", p->partCost, p->quantityOnHand);

  printf("---------------------\n");
}
void printOrder(const Order* order) {
  if (order == NULL) {
    printf("Invalid Order\n");
    return;
  }
  printf("Order ID      : %lld\n", order->orderID);
  printf("Order Date    : %s\n", order->orderDate);
  printf("Status        : ");
  if (order->orderStatus == 0) {
    printf("Unprocessed\n");
  }
  else if (order->orderStatus == 1) {
    printf("Fulfilled\n");
  }
  else if (order->orderStatus == 99) {
    printf("Insufficient Parts\n");
  }
  else if (order->orderStatus == 500) {
    printf("Credit Exceeded\n");
  }
  printf("Customer ID   : %d,\tDistinct Parts: %d\n", order->customerID, order->distinctParts);
  printf("Total Parts   : %d,\tTotal Amount  : $%.2f\n", order->totalParts, order->orderTotal);
  for (int i = 0; i < order->distinctParts; i++) {
    printf("Part ID: %d, Quantity Ordered: %d\n",
           order->orderedParts[i].partID, order->orderedParts[i].quantityOrdered);
  }
  printf("---------------------\n");
}

void printCustomers(const Customer* customers, int count) {
  if (count == 0) {
    printf("No customers to display. Try loading databases first.\n");
    return;
  }
  for (int i = 0; i < count; i++) {
    printCustomer(&customers[i]);
  }
}
void printParts(const Part* parts, int count) {
  if (count == 0) {
    printf("No parts to display. Try loading databases first.\n");
    return;
  }
  for (int i = 0; i < count; i++) {
    printPart(&parts[i]);
  }
}
void printOrders(const Order* orders, int count) {
  if (count == 0) {
    printf("No orders to display. Try loading databases first.\n");
    return;
  }
  for (int i = 0; i < count; i++) {
    printOrder(&orders[i]);
  }
}
void printMenu() {
  printf("Menu:\n");
  printf("1. Load Database(s)\n");
  printf("2. List Valid Customer(s)\n");
  printf("3. List Valid Part(s)\n");
  printf("4. List Valid Order(s)\n");
  printf("5. Free memory and exit\n");
}
// FUNCTION: promptInt
// DESCRIPTION:
//		Prompts the user for an integer input and validates it.
//		Continues to prompt until a valid integer is entered.
// PARAMETERS:
//		const char* prompt : The prompt message to display to the user.
//		int* input : Pointer to the integer variable where the input will be stored.
// RETURNS:
//		void
void promptInt(const char* prompt, int* input) {
  char inputBuffer[100];
  while (1) {
    printf("%s", prompt);
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    // When user input exceeds buffer amount
    if (!strchr(inputBuffer, '\n')) {
      flushInputStream();
      printf("Input exceeds buffer size. Please try again.\n");
      continue;
    }

    inputBuffer[strlen(inputBuffer) - 1] = '\0'; // Remove the trailing newline 

    if (isInteger(inputBuffer)) {
      sscanf_s(inputBuffer, "%d", input); // Writes directly into input
      return;
    }
    else {
      printf("Input must be a valid integer. Try again\n");
    }
  }
}
// FUNCTION: flushInputStream
// DESCRIPTION:
//		Flushes the input stream to remove any remaining characters.
// PARAMETERS:
//		void
// RETURNS:
//		void
void flushInputStream() {
  char extra = ' ';
  while (extra != '\n' && extra != EOF) {
    extra = getchar();
  }
}