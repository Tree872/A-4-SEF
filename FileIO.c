#include "FileIO.h"
#include "Customer.h"
#include "Part.h"
#include "Order.h"
#include "Validation.h"
#include "Constants.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int loadCustomers(Customer* customers, const char* fileName) {
  int customerCount = 0;
  int lineNumber = 0; // For error reporting
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    customerCount = 0;
    // TODO Log failure to open file
    return 0;
  }
  char line[1024];
  customerCount = 0;
  // Read each line from the file
  while (fgets(line, sizeof(line), file) != NULL) {
    // Check if over limit 
    if (customerCount >= CUSTOMERS_LIMIT) {
    // TODO Log customer limit full
      break;
    }
    // Check if line is too long
    if (strchr(line, '\n') == NULL) {
      // TODO Log line too long
      // Discard rest of the line
      while (1) {
        char extra = fgetc(file);
        if (extra == '\n' || extra == EOF) {
          break;
        }
      }
      continue; // Read next line
    }
    // Split line into fields
    char* fields[NUMBER_OF_CUSTOMER_FIELDS];
    int fieldCount = splitLine(line, fields, NUMBER_OF_CUSTOMER_FIELDS, '|');
    if (fieldCount != NUMBER_OF_CUSTOMER_FIELDS) {
      // TODO Log incorrect format
      continue; // Read next line
    }
    if (!validateCustomerFields(fields, lineNumber)) {
      // No need to log here since validate already did
      continue;
    }
    // Fields should be all valid at this point
    customers[customerCount] = parseFieldsToCustomer(fields);
    customerCount++;
  }
  fclose(file);
  return customerCount;
}

Customer parseFieldsToCustomer(const char** fields) {
  Customer newCustomer;
  strcpy_s(newCustomer.customerName, sizeof(newCustomer.customerName), fields[0]);
  strcpy_s(newCustomer.customerAddress, sizeof(newCustomer.customerAddress), fields[1]);
  strcpy_s(newCustomer.customerCity, sizeof(newCustomer.customerCity), fields[2]);
  strcpy_s(newCustomer.customerProvince, sizeof(newCustomer.customerProvince), fields[3]);
  strcpy_s(newCustomer.customerPostalCode, sizeof(newCustomer.customerPostalCode), fields[4]);
  strcpy_s(newCustomer.customerPhone, sizeof(newCustomer.customerPhone), fields[5]);
  strcpy_s(newCustomer.customerEmail, sizeof(newCustomer.customerEmail), fields[6]);
  sscanf_s(fields[7], "%d", &newCustomer.customerID);
  sscanf_s(fields[8], "%f", &newCustomer.customerCreditLimit);
  sscanf_s(fields[9], "%f", &newCustomer.currentAccountBalance);
  strcpy_s(newCustomer.lastPaymentMade, sizeof(newCustomer.lastPaymentMade), fields[10]);
  strcpy_s(newCustomer.customerJoinDate, sizeof(newCustomer.customerJoinDate), fields[11]);
  return newCustomer;
}

int loadParts(Part* parts, const char* fileName) {
  int partCount = 0;
  int lineNumber = 0; // For error reporting
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    partCount = 0;
    // TODO Log failure to open file
    return 0;
  }
  char line[1024];
  partCount = 0;
  // Read each line from the file
  while (fgets(line, sizeof(line), file) != NULL) {
    lineNumber++;
    // Check if over limit 
    if (partCount >= PARTS_LIMIT) {
      // TODO Log part limit full
      break;
    }
    // Check if line is too long
    if (strchr(line, '\n') == NULL) {
      // TODO Log line too long
      // Discard rest of the line
      while (1) {
        char extra = fgetc(file);
        if (extra == '\n' || extra == EOF) {
          break;
        }
      }
      continue; // Read next line
    }
    // Split line into fields
    char* fields[NUMBER_OF_PART_FIELDS];
    int fieldCount = splitLine(line, fields, NUMBER_OF_PART_FIELDS, '|');
    if (fieldCount != NUMBER_OF_PART_FIELDS) {
      // TODO Log incorrect format
      continue; // Read next line
    }
    if (!validatePartFields(fields, lineNumber)) {
      // No need to log here since validate already did
      continue;
    }
    // Fields should be all valid at this point
    parts[partCount] = parseFieldsToPart(fields);
    partCount++;
  }

  fclose(file);
  return partCount;
}

Part parseFieldsToPart(const char** fields) {
  Part newPart;
  strcpy_s(newPart.partName, sizeof(newPart.partName), fields[0]);
  strcpy_s(newPart.partNumber, sizeof(newPart.partNumber), fields[1]);
  strcpy_s(newPart.partLocation, sizeof(newPart.partLocation), fields[2]);
  sscanf_s(fields[3], "%f", &newPart.partCost);
  sscanf_s(fields[4], "%d", &newPart.quantityOnHand);
  sscanf_s(fields[5], "%d", &newPart.partStatus);
  sscanf_s(fields[6], "%d", &newPart.partID);
  return newPart;
}

int loadOrders(Order* orders, const Part* parts, int partCount, const Customer* customers, int customerCount, const char* fileName) {
  int orderCount = 0;
  int lineNumber = 0; // For error reporting
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    orderCount = 0;
    // TODO Log failure to open file
    return 0;
  }
  char line[2048];
  // Read each line from the file
  while (fgets(line, sizeof(line), file) != NULL) {
    lineNumber++;
    // Check if over limit 
    if (orderCount >= ORDERS_LIMIT) {
      // TODO Log order limit full
      break;
    }
    // Check if line is too long
    if (strchr(line, '\n') == NULL) {
      // TODO Log line too long
      // Discard rest of the line
      while (1) {
        char extra = fgetc(file);
        if (extra == '\n' || extra == EOF) {
          break;
        }
      }
      continue; // Read next line
    }
    // Split line into fields
    char* fields[NUMBER_OF_ORDER_FIELDS + PARTS_LIMIT * 2];
    int fieldCount = splitLine(line, fields, NUMBER_OF_ORDER_FIELDS + PARTS_LIMIT * 2, '|');
    
    // Check if the number of fields is valid
    if (fieldCount < NUMBER_OF_ORDER_FIELDS + 2 || fieldCount % 2 == 0) { 
      // TODO Log incorrect format
      continue; // Read next line
    }
    if (!validateOrderFields(fields, fieldCount, lineNumber, parts, partCount, customers, customerCount)) {
      // No need to log here since validate already did
      continue;
    }
    // Fields should be all valid at this point
    orders[orderCount] = parseFieldsToOrder(fields);
    orderCount++;
  }
  
  return orderCount;
}

Order parseFieldsToOrder(const char** fields) {
  Order newOrder;
  sscanf_s(fields[0], "%lld", &newOrder.orderID);
  strcpy_s(newOrder.orderDate, sizeof(newOrder.orderDate), fields[1]);
  sscanf_s(fields[2], "%d", &newOrder.orderStatus);
  sscanf_s(fields[3], "%d", &newOrder.customerID);
  sscanf_s(fields[4], "%f", &newOrder.orderTotal);
  sscanf_s(fields[5], "%d", &newOrder.distinctParts);
  sscanf_s(fields[6], "%d", &newOrder.totalParts);
  // Parse ordered parts
  for (int i = 0; i < newOrder.distinctParts; i++) {
    sscanf_s(fields[7 + i * 2], "%d", &newOrder.orderedParts[i].partID);
    sscanf_s(fields[8 + i * 2], "%d", &newOrder.orderedParts[i].quantityOrdered);
  }
  
  return newOrder;
}

int splitLine(char* line, char** fields, int fieldLimit, char delimiter) {
  int fieldCount = 0;
  char* tokenStartPointer = line;
  char *cursor = line;

  while (*cursor != '\0') {
    if (*cursor == delimiter) {
      *cursor = '\0';     
      fields[fieldCount] = tokenStartPointer;
      fieldCount++;
      cursor++;               
      tokenStartPointer = cursor;
      if (fieldCount > fieldLimit) {
        return -1;
      }
    }
    else {
      cursor++;
    }
  }
  return fieldCount;
}