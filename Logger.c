// FILE : Logger.c
// DESCRIPTION : Implements logging functionality for the system.
#include "Logger.h"
#include "Constants.h"
#include <stdio.h>
#include <time.h>

#  // Name of the runtime log file

// FUNCTION     : getTimestamp
// DESCRIPTION  : Gets the current date and time and formats it into a string.
// PARAMETERS   :
//   buffer     : A char array to store the formatted timestamp
//   size       : The size of the buffer
// RETURNS      : void
static void getTimestamp(char* buffer, int size) {
  time_t now = time(NULL);                 // Get current system time
  struct tm* t = localtime(&now);          // Convert to local time
  strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);  // Format the time string
}

// FUNCTION     : logError
// DESCRIPTION  : Writes a detailed error to the log file with timestamp.
//                Used when a customer, part, or order has invalid fields.
// PARAMETERS   :
//   sourceType : Type of the object (e.g., "Customer", "Part", "Order")
//   id         : The ID of the object (used to identify the record)
//   fieldName  : The name of the field where the error occurred
//   message    : The error message to be logged
// RETURNS      : void
void logError(const char* sourceType, int id, const char* fieldName, const char* message) {
  FILE* file = fopen(LOG_FILE, "a");  // Open the log file in append mode
  if (!file) return;  // If file can't be opened, exit silently

  char timestamp[20];
  getTimestamp(timestamp, sizeof(timestamp));  // Get current timestamp

  // Write the error message to the log file
  fprintf(file, "%s | %s ID: %d | Field: %s | Error: %s\n", timestamp, sourceType, id, fieldName, message);

  fclose(file);  // Close the file
}

// FUNCTION     : logGeneric
// DESCRIPTION  : Logs a general system message with timestamp.
//                Used for events like file not found, limits reached, etc.
// PARAMETERS   :
//   message    : The general message to be logged
// RETURNS      : void
void logGeneric(const char* message) {
  FILE* file = fopen(LOG_FILE, "a");  // Open the log file in append mode
  if (!file) return;  // Exit silently if file can't be opened

  char timestamp[20];
  getTimestamp(timestamp, sizeof(timestamp));  // Get current timestamp

  // Write the general message to the log
  fprintf(file, "%s | %s\n", timestamp, message);

  fclose(file);  // Close the file
}

// FUNCTION     : clearLog
// DESCRIPTION  : Clears the contents of the log file (empties it).
// PARAMETERS   : None
// RETURNS      : void
void clearLog() {
  FILE* file = fopen(LOG_FILE, "w");  // Open the file in write mode (truncates it)
  if (file) {
    fclose(file);  // Close immediately to save empty file
  }
}
