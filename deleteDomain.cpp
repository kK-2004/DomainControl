#include "deleteDomain.h"
#include "saveDomain.h"
#include <QDebug>
void deleteNodeAndSubtree(Node* root, Node* target) {
    if (root == nullptr) {
        return;
    }
    if (root == target) {
        // 释放目标节点及其子树
        root = nullptr;
        return;
    }
    if (root->firstChild == target) {
        root->firstChild = target->nextSibling;
    }
    else if (root->nextSibling == target) {
        root->nextSibling = target->nextSibling;
    }
    deleteNodeAndSubtree(root->firstChild, target);
    deleteNodeAndSubtree(root->nextSibling, target);
}
bool deleteDomain(Node* root, const char* domain)
{
    char* reversedDomain = reverseDomainParts(domain);
    Node* node = findExactNode(root, reversedDomain);
    if (node) {
        // 删除目标节点及其子树
        deleteNodeAndSubtree(root, node);
        return true;
    }
    else {
        return false;
    }
}

