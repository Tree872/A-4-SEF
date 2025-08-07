// FILE : Order.h
// DESCRIPTION : This header file defines the structure for Order and supporting types.
#ifndef ORDER_H
#define ORDER_H
#include "Constants.h"

typedef struct {
  int partID; // Mandatory, > 0 (must link to existing part)
  int quantityOrdered; // Mandatory, >= 1
} OrderedPart;

typedef struct {
  long long orderID; // Mandatory, format YYYYMMDDSSS as int (e.g., 20241113034)
  char orderDate[11]; // Mandatory, YYYY-MM-DD format 
  int orderStatus; // Mandatory, 0 (unprocessed), 1 (fulfilled), 99 (insufficient parts), 500 (credit exceeded)
  int customerID; // Mandatory, > 0 (must link to existing customer)
  float orderTotal; // Mandatory, > 0.00
  int distinctParts; // Mandatory, >= 1
  int totalParts; // Mandatory, >= 1
  OrderedPart orderedParts[PARTS_LIMIT];  
} Order;

#endif