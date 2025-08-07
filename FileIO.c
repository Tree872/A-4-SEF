#include "FileIO.h"
#include "Customer.h"
#include "Part.h"
#include "Order.h"
#include "Validation.h"
#include "Logger.h"
#include "Constants.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTION : loadCustomers
// DESCRIPTION : 
//    Reads customer data from a file and populates the customers array.
//    Includes error handling for file operations and calling data validation functions.
// PARAMETERS :
//    Customer* customers: Pointer to an array of Customer structures to be filled.
//    const char* fileName: Name of the file to read customer data from.
// RETURNS :
//    int : The number of customers successfully loaded.
int loadCustomers(Customer* customers, const char* fileName) {
  int customerCount = 0;
  int lineNumber = 0; // For error reporting
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    logGeneric("Failed to open customers database.");
    return 0;
  }
  char errorMessage[256];
  char line[1024];
  customerCount = 0;
  // Read each line from the file
  while (fgets(line, sizeof(line), file) != NULL) {
    // Skip empty lines
    if (line[0] == '\n' || line[0] == '\r') {
      continue; // Read next line
    }
    lineNumber++;
    // Check if over limit 
    if (customerCount >= CUSTOMERS_LIMIT) {
      logGeneric("Customer limit reached when loading from database, cannot load more customers.");
      break;
    }
    // Check if line is too long
    if (strlen(line) == sizeof(line) - 1) {
      snprintf(errorMessage, sizeof(errorMessage), "In customers database line %d: Line is too long.", lineNumber);
      logGeneric(errorMessage);
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
      snprintf(errorMessage, sizeof(errorMessage), "In customers database line %d: Incorrect number of fields (%d expected, found %d)", 
        lineNumber, NUMBER_OF_CUSTOMER_FIELDS, fieldCount);
      logGeneric(errorMessage);
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
// FUNCTION : parseFieldsToCustomer
// DESCRIPTION :
//    Converts an array of strings (fields) into a Customer structure.
//    Assumes that the fields are in the correct order and format as defined in the Customer structure.
//    Assumes that the fields are validated.
// PARAMETERS :
//    const char** fields: Array of strings containing customer data.
// RETURNS :
//    Customer : An instance of a Customer struct populated with the data from the fields.
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
// FUNCTION : loadParts
// DESCRIPTION :
//    Reads part data from a file and populates the parts array.
//    Includes error handling for file operations and calling data validation functions.
// PARAMETERS :
//    Part* parts: Pointer to an array of Part structures to be filled.
//    const char* fileName: Name of the file to read part data from.
// RETURNS :
//    int : The number of parts successfully loaded.
int loadParts(Part* parts, const char* fileName) {
  int partCount = 0;
  int lineNumber = 0; // For error reporting
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    partCount = 0;
    logGeneric("Failed to open parts database.");
    return 0;
  }
  char line[1024];
  char errorMessage[256];
  partCount = 0;
  // Read each line from the file
  while (fgets(line, sizeof(line), file) != NULL) {
    // Skip empty lines
    if (line[0] == '\n' || line[0] == '\r') {
      continue; // Read next line
    }
    lineNumber++;
    // Check if over limit 
    if (partCount >= PARTS_LIMIT) {
      logGeneric("Part limit reached when loading from database, cannot load more parts.");
      break;
    }
    // Check if line is too long
    if (strlen(line) == sizeof(line) - 1) {
      snprintf(errorMessage, sizeof(errorMessage), "In parts database line %d: Line is too long.", lineNumber);
      logGeneric(errorMessage);
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
      snprintf(errorMessage, sizeof(errorMessage), "In parts database line %d: Incorrect number of fields (%d expected, found %d)", 
        lineNumber, NUMBER_OF_PART_FIELDS, fieldCount);
      logGeneric(errorMessage);
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
// FUNCTION : parseFieldsToPart
// DESCRIPTION :
//    Converts an array of strings (fields) into a Part structure.
//    Assumes that the fields are in the correct order and format as defined in the Part structure.
//    Assumes that the fields are validated.
// PARAMETERS :
//    const char** fields: Array of strings containing part data.
// RETURNS :
//    Part : An instance of a Part struct populated with the data from the fields.
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
// FUNCTION : loadOrders
// DESCRIPTION :
//    Reads order data from a file and populates the orders array.
//    It uses the parts and customers arrays to validate their IDs in the order.
//    Includes error handling for file operations and calling data validation functions.
// PARAMETERS :
//    Order* orders: Pointer to an array of Order structures to be filled.
//    const Part* parts: Pointer to an array of Part structures for validation.
//    int partCount: Number of parts in the parts array.
//    const Customer* customers: Pointer to an array of Customer structures for validation.
//    int customerCount: Number of customers in the customers array.
//    const char* fileName: Name of the file to read order data from.
// RETURNS :
//    int : The number of orders successfully loaded.
int loadOrders(Order* orders, const Part* parts, int partCount, const Customer* customers, int customerCount, const char* fileName) {
  int orderCount = 0;
  int lineNumber = 0; // For error reporting
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    orderCount = 0;
    logGeneric("Failed to open orders database.");
    return 0;
  }
  char errorMessage[256];
  char line[2048];
  // Read each line from the file
  while (fgets(line, sizeof(line), file) != NULL) {
    // Skip empty lines
    if (line[0] == '\n' || line[0] == '\r') {
      continue; // Read next line
    }
    lineNumber++;
    // Check if over limit 
    if (orderCount >= ORDERS_LIMIT) {
      logGeneric("Order limit reached when loading from database, cannot load more orders.");
      break;
    }
    // Check if line is too long
    if (strlen(line) == sizeof(line) - 1) {
      snprintf(errorMessage, sizeof(errorMessage), "In orders database line %d: Line is too long.", lineNumber);
      logGeneric(errorMessage);
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
      logGeneric("Incorrect number of fields in orders database.");
      logGeneric("Each order must have at least 9 fields and an odd number of fields to be valid.");
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
// FUNCTION : parseFieldsToOrder
// DESCRIPTION :
//    Converts an array of strings (fields) into an Order structure.
//    Assumes that the fields are in the correct order and format as defined in the Order structure.
//    Assumes that the fields are validated.
// PARAMETERS :
//    const char** fields: Array of strings containing order data.
// RETURNS :
//    Order : An instance of an Order struct populated with the data from the fields.
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
// FUNCTION : splitLine
// DESCRIPTION :
//    Splits a line of text into fields based on a specified delimiter.
//    Can handle up to fieldLimit fields.
//    A field must end with the delimiter character to be considered valid. 
//    (e.g., "1|2|3" results in 2 fields: "1", "2" while "1|2|3| results in 3 fields: "1", "2", "3")
//    Modifies the input line by replacing the delimiter with a null terminator.
// PARAMETERS :
//    char* line: The line of text to be split.
//    char** fields: An array to hold the pointers to the start of each field.
//    int fieldLimit: The maximum number of fields to be stored in the fields array.
//    char delimiter: The character used to split the line into fields.
// RETURNS :
//    int : The number of fields successfully split from the line. Returns -1 if fieldLimit is exceeded.
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