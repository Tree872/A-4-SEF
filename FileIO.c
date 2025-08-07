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
  FILE* file = NULL;
  errno_t err = fopen_s(&file, fileName, "r");
  if (err != 0 || file == NULL) {
    customerCount = 0;
    // TODO Log failure to open file
    printf("Failure to open file\n");
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
    if (!validateCustomerFields(fields, customerCount + 1)) {
      // No need to log here since validate already did
      continue;
    }
    // Fields should be all valid at this point
    customers[customerCount] = parseFieldsToCustomers(fields);
    customerCount++;
  }
  fclose(file);
  return customerCount;
}

Customer parseFieldsToCustomers(const char** fields) {
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
    if (!validatePartFields(fields, partCount + 1)) {
      // No need to log here since validate already did
      continue;
    }
    // Fields should be all valid at this point
    parts[partCount] = parseFieldsToParts(fields);
    partCount++;
  }

  fclose(file);
  return partCount;
}

Part parseFieldsToParts(const char** fields) {
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