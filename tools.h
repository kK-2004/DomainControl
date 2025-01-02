#ifndef TOOLS_H
#define TOOLS_H

#include "headers.h"

char* removeSubdomain(const char* subdomain, const char* fullDomain);
char* reverseDomainParts(const char* input);
void printSubtreeReversed(Node* node, char* path, int depth);
// void printNodesAtDepth(Node* node, int targetDepth);
Node* findNodeAtDepth(Node* node, const char* name, int targetDepth);
char* printFullDomainPath(Node* node);
void collectFullDomainPaths(Node* node, char* pathArray[], int& pathIndex);
void loadTreeFromFile(Node* root, const char* filename);
Node* findOrCreateNode(Node* root, const char* name, int depth);
// void stringTotree(Node* node, char* domain, int depth,const char*id,const category Category);
// bool fileinitial();

#endif // TOOLS_H
