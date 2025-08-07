// FILE : FileIO.h
// DESCRIPTION : This header file defines functions for loading customers, parts, and orders from files.
#ifndef FILEIO_H
#define FILEIO_H

#include "Customer.h"
#include "Part.h"
#include "Order.h"

int loadCustomers(Customer* customers, const char* fileName);
Customer parseFieldsToCustomer(const char** fields);

int loadParts(Part* parts, const char* fileName);
Part parseFieldsToPart(const char** fields);

int loadOrders(Order* orders, const Part* parts, int partCount, const Customer* customers, int customerCount, const char* fileName);
Order parseFieldsToOrder(const char** fields);

int splitLine(char* line, char** fields, int fieldLimit, char delimiter);

#endif 
