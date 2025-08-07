#ifndef FILEIO_H
#define FILEIO_H

#include "Customer.h"
#include "Part.h"
#include "Order.h"

int loadCustomers(Customer* customers, const char* fileName);
Customer parseFieldsToCustomers(const char** fields);

int loadParts(Part* parts, const char* fileName);
Part parseFieldsToParts(const char** fields);

int splitLine(char* line, char** fields, int fieldLimit, char delimiter);

#endif 
