#include "Validation.h"
#include "Customer.h"
#include "Part.h"
#include "Order.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char* CANADA_PROVINCES[] = {
    "AB", "BC", "MB", "NB", "NL", "NS", "NT", "NU", "ON", "PE", "QC", "SK", "YT"
};

int validateCustomerFields(char** fields, int lineNumber) {
  char errorMessage[1024];
  snprintf(errorMessage, sizeof(errorMessage), "Error when loading  customers database: Line %d: ", lineNumber);
  // Validate name
  if (strlen(fields[0]) == 0 || strlen(fields[0]) > 50) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #1: Customer name must neither be blank or over 50 characters");
  }
  // Validate address
  if (strlen(fields[1]) == 0 || strlen(fields[1]) > 100) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #2: Customer address must neither be blank or over 100 characters.");
  }
  // Validate city
  if (strlen(fields[2]) == 0 || strlen(fields[2]) > 100) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #3: Customer city must neither be blank or over 100 characters. ");
  }
  // Validate province
  if (!validateProvince(fields[3])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #4: Customer province must be a valid canadian province abbreviations");
  }
  // Validate postal code
  if (!validatePostalCode(fields[4])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #5: Customer postal code must be in ANANAN format (where A is an alphabetic letter and N is a numeric digit)");
  }
  // Validate phone number
  if (!validatePhoneNumber(fields[5])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #6: Customer phone number must be in ###-###-#### format");
  }
  // Validate email
  if (!validateEmail(fields[6])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #7: Customer email address must a valid email address");
  }
  // Validate customer ID
  int customerID = 0;
  sscanf_s(fields[7], "%d", &customerID);
  if (!isInteger(fields[7]) || customerID <= 0) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #8: Customer ID must be a positive int.");
  }
  // Validate credit limit
  float creditLimit = 0.0;
  sscanf_s(fields[8], "%f", &creditLimit);
  if (!isNumber(fields[8]) || creditLimit <= 0.0) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #9: Customer credit limit must be greater than 0. ");
  }
  // Validate account balance
  if (!isNumber(fields[9]) || fields[9][0] == '-') {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #10: Customer account balance greater than or equal to 0. ");
  }
  // Validate last payment date (optional)
  if (strlen(fields[10]) > 0 && !validateDate(fields[10])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #11: Customer last payment date must be either blank or in YYYY-MM-DD format and is a valid date.");
  }
  // Validate join date
  if (fields[11] == NULL || !validateDate(fields[11])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #12: Customer join date must be in YYYY-MM-DD format and is a valid date.");
  }
  if (strlen(errorMessage) > 60) { // When there is at least one error
    printf("%s\n", errorMessage);
    // TODO Log the error message
    return 0; 
  }
  return 1;
}

int validatePartFields(char** fields, int lineNumber) {
  char errorMessage[1024];
  snprintf(errorMessage, sizeof(errorMessage), "Error when loading part database: Line %d: ", lineNumber);
  // Validate part name
  if (strlen(fields[0]) == 0 || strlen(fields[0]) > 50) {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #1: Part name must neither be blank or over 50 characters");
  }
  // Validate part number
  if (strlen(fields[1]) == 0 || strlen(fields[1]) > 50) {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #2: Part number must neither be blank or over 50 characters");
  }
  // Validate part location
  if (!validatePartLocation(fields[2])) {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #3: Part location must be in A###-S###-L##-B## format");
  }
  // Validate part cost
  float partCost = 0.0;
  sscanf_s(fields[3], "%f", &partCost);
  if (!isNumber(fields[3]) || partCost <= 0.0) {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #4: Part cost must be a positive number.");
  }
  // Validate quantity on hand
  if (!isInteger(fields[4]) || fields[4][0] == '-') {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #5: Quantity on hand must be a integer greater than or equal 0.");
  }
  // Validate part status
  if (!validatePartStatus(fields[4], fields[5])) {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #6: Part status or quantity on hand is invalid.");
  }
  // Validate part ID
  int partID = 0;
  sscanf_s(fields[6], "%d", &partID);
  if (!isInteger(fields[6]) || partID <= 0) {
    strcat_s(errorMessage, sizeof(errorMessage),
      "\nField #7: Part ID must be a positive integer.");
  }

  if (strlen(errorMessage) > 60) { // When there is at least one error
    printf("%s\n", errorMessage);
    // TODO Log the error message
    return 0; 
  }
  return 1;
}

int validateOrderFields(char** fields, int numOfReadFields, int lineNumber, const Part* parts, int partCount, const Customer* customers, int customerCount) {
  char errorMessage[4096];
  snprintf(errorMessage, sizeof(errorMessage), "Error when loading orders database: Line %d: ", lineNumber);
  // Validate order ID
  if (!validateOrderID(fields[0])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #1: Order ID must be in YYYYMMDDSSS format and is a valid date.");
  }
  // Validate order date
  if (!validateDate(fields[1])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #2: Order date must be in YYYY-MM-DD format and is a valid date.");
  }
  // Validate order status
  if (!validateOrderStatus(fields[2])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #3: Order status must be a valid integer (0, 1, 99, or 500).");
  }
  // Validate customer ID
  if (!validateCustomerIDInOrder(fields[3], customers, customerCount)) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #4: Customer ID must be a positive integer and must link to an existing customer.");
  }
  // Validate order total (just a format check, actual calculation is done later)
  float orderTotal = 0.0;
  sscanf_s(fields[4], "%f", &orderTotal);
  if (!isNumber(fields[4]) || orderTotal <= 0.0) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #5: Order total must be a positive number.");
  }
  // Validate distinct parts
  int distinctParts = 0;
  sscanf_s(fields[5], "%d", &distinctParts);
  int expectedDistinctParts = (numOfReadFields - 7) / 2; // Each part has two fields
  if (!isInteger(fields[5])) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #6: Distinct parts must be a positive integer.");
  }
  else if (distinctParts != expectedDistinctParts) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #6: Distinct parts count does not match the number of parts provided.");
  }
  // Validate total parts (just a format check, actual calculation is done later)
  int totalParts = 0;
  sscanf_s(fields[6], "%d", &totalParts);
  if (!isInteger(fields[6]) || totalParts < 1) {
    strcat_s(errorMessage, sizeof(errorMessage), 
      "\nField #7: Total parts must be a positive integer greater than or equal to 1.");
  }
  // Validate ordered parts
  int numOfParsedOrderedParts = (numOfReadFields - 7) / 2;
  char localErrorMessage[200];
  int isAllPartsValid = 1;
  for (int i = 0; i < numOfParsedOrderedParts; i++) {
    int partID = 0;
    int quantityOrdered = 0;
    sscanf_s(fields[7 + i * 2], "%d", &partID);
    sscanf_s(fields[8 + i * 2], "%d", &quantityOrdered);
    if (!isInteger(fields[7 + i * 2]) || partID <= 0) {
      isAllPartsValid = 0;
      snprintf(localErrorMessage, sizeof(localErrorMessage), 
        "\nField #%d: Part ID must be a positive integer.", 8 + i * 2);
      strcat_s(errorMessage, sizeof(errorMessage), localErrorMessage);
    }
    else if (!validatePartIDInOrder(partID, parts, partCount)) {
      isAllPartsValid = 0;
      snprintf(localErrorMessage, sizeof(localErrorMessage),
        "\nField #%d: Part ID %d does not exist in the parts database.", 7 + i * 2, partID);
      strcat_s(errorMessage, sizeof(errorMessage), localErrorMessage);
    }
    if (!isInteger(fields[8 + i * 2]) || quantityOrdered <= 0) {
      isAllPartsValid = 0;
      snprintf(localErrorMessage, sizeof(localErrorMessage), 
        "\nField #%d: Quantity ordered must be a positive integer.", 9 + i * 2);
      strcat_s(errorMessage, sizeof(errorMessage), localErrorMessage);
    }
  }
  // Validate order total and total parts
  if (isAllPartsValid) {
    int calculatedTotalParts = 0;
    float calculatedOrderTotal = 0.0;
    for (int i = 0; i < numOfParsedOrderedParts; i++) {
      int partID = 0;
      int quantityOrdered = 0;
      sscanf_s(fields[7 + i * 2], "%d", &partID);
      sscanf_s(fields[8 + i * 2], "%d", &quantityOrdered);
      for (int j = 0; j < partCount; j++) {
        if (parts[j].partID == partID) {
          calculatedTotalParts += quantityOrdered;
          calculatedOrderTotal += parts[j].partCost * quantityOrdered;
          break;
        }
      }
    }
    if (calculatedTotalParts != totalParts) {
      strcat_s(errorMessage, sizeof(errorMessage), 
        "\nField #5: Calculated total parts does not match the provided total parts.");
    }
    if (calculatedOrderTotal != orderTotal) {
      strcat_s(errorMessage, sizeof(errorMessage), 
        "\nField #7: Calculated order total does not match the provided order total.");
    }
  }


  if (strlen(errorMessage) > 60) { // When there is at least one error
    printf("%s\n", errorMessage);
    // TODO Log the error message
    return 0; 
  }
  return 1;
}

int validateProvince(const char* province) {
  if (strlen(province) != 2) {
    return 0; // Invalid province format
  }
  int numOfProvinces = sizeof(CANADA_PROVINCES) / sizeof(CANADA_PROVINCES[0]);
  for (int i = 0; i < numOfProvinces; i++) {
    if (strcmp(province, CANADA_PROVINCES[i]) == 0) {
      return 1; 
    }
  }
  return 0; 
}

int validatePostalCode(const char* postalCode) {
  if (strlen(postalCode) != 6) {
    return 0; // Invalid postal code length
  }
  for (int i = 0; i < 6; i++) {
    if (i % 2 == 0 && !isalpha(postalCode[i])) {
      return 0; // Even index is a letter
    }
    if (i % 2 == 1 && !isdigit(postalCode[i])) {
      return 0; // Odd index is a digit
    }
  }
  return 1;
}

int validatePhoneNumber(const char* phoneNumber) {
  if (strlen(phoneNumber) != 12) {
    return 0; // Invalid phone number length
  }
  for (int i = 0; i < 12; i++) {
    if (i == 3 || i == 7) {
      if (phoneNumber[i] != '-') {
        return 0; // Hyphen is not in the correct position
      }
    }
    else if (!isdigit(phoneNumber[i])) {
      return 0;
    }
  }
  return 1; 
}

int validateEmail(const char* email) {
  //  The email address must start and end with a letter (no numbers or symbols).
  //  There must be exactly one @ somewhere in the string.
  //  There must be at least one dot after @ but not right after.
  //  There must be no consecutive dots.
  //  The only valid characters before the @ are letters, numbers.
  //  The only valid characters after the @ are letters, numbers, and dots (non-consecutive).
  if (strlen(email) == 0 || strlen(email) > 50) {
    return 0; // Invalid email length
  }
  int atPosition = -1;
  int lastDotPosition = -1;
  int emailLen = strlen(email);
  
  if (!isalpha(email[0]) || !isalpha(email[emailLen - 1])) {
    return 0; // Check if the first and last characters are letters
  }
  for (int i = 0; i < emailLen; i++) {
    if (email[i] == '@') {
      if (atPosition != -1) {
        return 0; // More than one @ found
      }
      else if (email[i + 1] == '.') {
        return 0; // Dot right after @
      }
      atPosition = i;

    }
    else if (email[i] == '.') {
      if (lastDotPosition == i - 1) {
        return 0; // Consecutive dots 
      }
      else if (atPosition == -1) {
        return 0; // Dot before @
      }
      lastDotPosition = i;
    }
    else if (!isalnum(email[i])) {
      return 0; 
    }
  }

  if (atPosition == -1 || lastDotPosition == -1 || lastDotPosition < atPosition) {
    return 0; // No @ or no dot after @
  }

  return 1;
}

int validateDate(const char* date) {
  // Format check (YYYY-MM-DD)
  if (strlen(date) != 10) {
    return 0; // Invalid date length
  }
  for (int i = 0; i < 10; i++) {
    if (i == 4 || i == 7) {
      if (date[i] != '-') {
        return 0; // Hyphen is not in the correct position
      }
    }
    else if (!isdigit(date[i])) {
      return 0;
    }
  }
  // Range check
  int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + (date[3] - '0');
  int month = (date[5] - '0') * 10 + (date[6] - '0');
  int day = (date[8] - '0') * 10 + (date[9] - '0');

  return isValidDate(year, month, day); 
}

int validatePartLocation(const char* partLocation) {
  if (strlen(partLocation) != 17 || partLocation[4] != '-' || partLocation[9] != '-' || partLocation[13] != '-') {
    printf("Invalid part location format: %s\n", partLocation);
    return 0; // Invalid format
  }
  // Check A###-S###-L##-B##
  if (partLocation[0] != 'A' || !isdigit(partLocation[1]) || !isdigit(partLocation[2]) || !isdigit(partLocation[3])) {
    return 0; // Invalid aisle
  }
  if (partLocation[5] != 'S' || !isdigit(partLocation[6]) || !isdigit(partLocation[7]) || !isdigit(partLocation[8])) {
    return 0; // Invalid shelf
  }
  if (partLocation[10] != 'L' || !isdigit(partLocation[11]) || !isdigit(partLocation[12])) {
    return 0; // Invalid level
  }
  if (partLocation[14] != 'B' || !isdigit(partLocation[15]) || !isdigit(partLocation[16])) {
    return 0; // Invalid bin
  }
  return 1; 
}

int validatePartStatus(char* quantityOnHand, char* partStatus) {
  if (!isInteger(quantityOnHand) || !isInteger(partStatus)) {
    return 0; // Invalid input
  }
  int quantityOnHandValue = 0;
  int partStatusValue = 0;
  sscanf_s(quantityOnHand, "%d", &quantityOnHandValue);
  sscanf_s(partStatus, "%d", &partStatusValue);
  if (quantityOnHandValue > 100 && partStatusValue != 0) {
    return 0; 
  }
  else if (quantityOnHandValue <= 100 && quantityOnHandValue >= 0 && partStatusValue != 99) {
    return 0; 
  }
  else if (quantityOnHandValue < 0 && partStatusValue >= 0) {
    return 0; 
  }
  return 1; 
}

int validateOrderID(const char* orderID) {
  if (strlen(orderID) != 11) {
    return 0; 
  }
  if (!isInteger(orderID)) {
    return 0; 
  }
  int year = (orderID[0] - '0') * 1000 + (orderID[1] - '0') * 100 + (orderID[2] - '0') * 10 + (orderID[3] - '0');
  int month = (orderID[4] - '0') * 10 + (orderID[5] - '0');
  int day = (orderID[6] - '0') * 10 + (orderID[7] - '0');
  if (!isValidDate(year, month, day)) {
    return 0; 
  }
  return 1;
}

int validateOrderStatus(char* orderStatus) {
  if (!isInteger(orderStatus)) {
    return 0;
  }
  int orderStatusValue = 0;
  sscanf_s(orderStatus, "%d", &orderStatusValue);
  if (orderStatusValue != 0 && orderStatusValue != 1 && orderStatusValue != 99 && orderStatusValue != 500) {
    return 0;
  }
  return 1; 
}

int validateCustomerIDInOrder(char* customerID, const Customer* customers, int customerCount) {
  if (!isInteger(customerID)) {
    return 0; 
  }
  int customerIDValue = 0;
  sscanf_s(customerID, "%d", &customerIDValue);
  if (customerIDValue <= 0) {
    return 0; 
  }
  for (int i = 0; i < customerCount; i++) {
    if (customers[i].customerID == customerIDValue) {
      return 1; // Valid customer ID found
    }
  }
  return 0; 
}

int validatePartIDInOrder(int partID, const Part* parts, int partCount) {
  if (partID <= 0) {
    return 0; // Invalid part ID
  }
  for (int i = 0; i < partCount; i++) {
    if (parts[i].partID == partID) {
      return 1; // Valid part ID found
    }
  }
  return 0; // Part ID not found
}

int isInteger(const char* str) {
  if (str == NULL || *str == '\0') {
    return 0;
  }
  int len = strlen(str);

  for (int i = 0; i < len; i++) {
    if (i == 0) {
      if ((str[i] == '-' || str[i] == '+') && len > 1) {
        continue; 
      }
    }
    if (!isdigit(str[i])) {
      return 0;
    }
  }
  return 1;
}

int isNumber(const char* str) {
  if (str == NULL || *str == '\0') {
    return 0;
  }
  int len = strlen(str);
  if (str[0] == '.' || str[len - 1] == '.') {
    return 0;
  }
  int dotCount = 0;
  int i = 0;
  for (i; i < len; i++) {
    if (i == 0) {
      if ((str[i] == '-' || str[i] == '+') && len > 1) {
        continue; 
      }
    }
    else if (str[i] == '.') {
      dotCount++;
      if (dotCount > 1) {
        return 0; 
      }
    } 
    else if (!isdigit(str[i])) {
      return 0; 
    }
  }

  return 1;
}

int isValidDate(int year, int month, int day) {
  if (year < 2000 || year > 2100 || month < 1 || month > 12 || day < 1) {
    return 0; 
  }

  if (month == 4 || month == 6 || month == 9 || month == 11) {
    if (day > 30) {
      return 0;
    }
  }
  else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
    if (day > 31) {
      return 0;
    }
  }
  else if (month == 2) {
    // Check for leap year
    int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeapYear && day > 29) {
      return 0;
    }
    else if (!isLeapYear && day > 28) {
      return 0;
    }
  }
  return 1; 
}