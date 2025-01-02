#ifndef USERDOMAINGET_H
#define USERDOMAINGET_H

#include "headers.h"


int countSubstrings(const char* str, char delimiter);
void storeValidPath(char validPaths[MAX_PATHS][MAX_PATH_LEN], int& validPathCount, const char* path);
void printSubtreeUser(Node* node, char* path, int depth, const char* user, char validPaths[MAX_PATHS][MAX_PATH_LEN], int& validPathCount);
void printValidPaths(char validPaths[MAX_PATHS][MAX_PATH_LEN], int validPathCount);

#endif // USERDOMAINGET_H
