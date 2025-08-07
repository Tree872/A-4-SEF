// FILE : Part.h
// DESCRIPTION : This header file defines the structure for Part.
#ifndef PART_H
#define PART_H

typedef struct {
  char partName[51]; // Mandatory, max 50 chars
  char partNumber[51]; // Mandatory, max 50 chars, includes numbers and letters
  char partLocation[21]; // Mandatory, 4 coded components (aisle, shelf, level and bin) separated by the dashes (A###-S###-L##-B##) (e.g., A001-S002-L01-B01)
  float partCost; // Mandatory, > 0.00
  int quantityOnHand; // Mandatory, >= 0
  int partStatus; // Mandatory
  /*    partStatus meanings:
      - 0: QuantityOnHand > 100
      - 99: QuantityOnHand < 100
      - Negative: deficit (e.g., -180 if short 180 parts for customer orders)
  */
  int partID; // Mandatory, > 0
} Part;

#endif 

