#ifndef FINDTREE_H
#define FINDTREE_H

#include "headers.h"
#include "tools.h"

// 查找一个域名，返回对应的节点（完整匹配）
Node* findExactNode(Node* node, const char* domain);
bool findVagueNode(Node* node, const char* domain);

#endif // FINDTREE_H
