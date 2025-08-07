// FILE: Logger.h
// DESCRIPTION: This header file defines the logging functions used in the system.
#ifndef LOGGER_H
#define LOGGER_H

// Logs an error for a specific record like Customer, Part, or Order
void logError(const char* sourceType, int id, const char* fieldName, const char* message);

// Logs a general message e.g., file open errors, limits exceeded
void logGeneric(const char* message);

// Clears the content of the log file empties runtime_log.txt
void clearLog();

#endif
