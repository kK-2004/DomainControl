#include "createTree.h"

bool createHelper(Node* node, const char* domain, int depth,const QString &username,int choice)
{
    const char* remainingDomain = strchr(domain, '.');
    size_t partLength = (remainingDomain == nullptr) ? strlen(domain) : remainingDomain - domain;

    // 分配内存，确保 '\0' 结尾
    char* part = new char[partLength + 1];
    strncpy(part, domain, partLength);
    part[partLength] = '\0';

    // 查找子节点，查看是否已存在
    Node* child = node->firstChild;
    while (child) {
        if (strcmp(child->name, part) == 0) {
            delete[] part;  // 释放临时内存
            if (remainingDomain == nullptr) return false;
            return createHelper(child, remainingDomain + 1, depth + 1,username,choice);
        }
        child = child->nextSibling;
    }

    // 创建新节点
    Node* newNode = new Node(part, depth + 1, node);  // 假设 Node 构造函数正确处理 name
    delete[] part; // 释放 part
    if (depth >= 1) {
        strcpy(newNode->owner, username.toLocal8Bit().constData());
    }
    if (depth == 1) {
        int temchoice=choice;
        category Category = other;  // 默认分类
        if (temchoice >= 1 && temchoice <= 15) {
            Category = static_cast<category>(temchoice - 1);
            newNode->nodeCategory = Category;
        }
    }


    // 将新节点加入到父节点的子节点链表中
    newNode->nextSibling = node->firstChild;
    node->firstChild = newNode;

    // 递归处理剩余的域名部分
    if (remainingDomain != nullptr) {
        createHelper(newNode, remainingDomain + 1, depth + 1,username,choice);
    }

    return true;
}
// bool createHelper(Node* node, const char* domain, int depth,const QString &username,int choice)
// {
//     const char* remainingDomain = strchr(domain, '.');
//     size_t partLength = (remainingDomain == nullptr) ? strlen(domain) : remainingDomain - domain;

//     // 分配内存，确保 '\0' 结尾
//     char* part = new char[partLength + 1];
//     strncpy(part, domain, partLength);
//     part[partLength] = '\0';

//     // 查找子节点，查看是否已存在
//     Node* child = node->firstChild;
//     while (child) {
//         if (strcmp(child->name, part) == 0) {
//             delete[] part;  // 释放临时内存
//             if (remainingDomain == nullptr) return false;
//             return createHelper(child, remainingDomain + 1, depth + 1,username,choice);
//         }
//         child = child->nextSibling;
//     }

//     // 创建新节点
//     Node* newNode = new Node(part, depth + 1, node);  // 假设 Node 构造函数正确处理 name
//     delete[] part; // 释放 part
//     if (depth >= 1) {
//         strcpy(newNode->id, username.toLocal8Bit().constData());
//     }
//     if (depth == 1) {
//         int temchoice=choice;
//         category Category = other;  // 默认分类
//         if (temchoice >= 1 && temchoice <= 15) {
//             Category = static_cast<category>(temchoice - 1);
//             newNode->nodeCategory = Category;
//         }
//     }

//     // 将新节点加入到父节点的子节点链表中
//     newNode->nextSibling = node->firstChild;
//     node->firstChild = newNode;

//     // 递归处理剩余的域名部分
//     if (remainingDomain != nullptr) {
//         createHelper(newNode, remainingDomain + 1, depth + 1,username,choice);
//     }
//     return true;
//     // const char* remainingDomain = strchr(domain, '.');

//     // size_t partLength = (remainingDomain == nullptr) ? strlen(domain) : remainingDomain - domain;
//     // char* part = new char[partLength + 1];
//     // strncpy(part, domain, partLength);
//     // part[partLength] = '\0';

//     // Node* child = node->firstChild;
//     // while (child) {
//     //     if (strcmp(child->name, part) == 0) {
//     //         delete[] part;
//     //         if (remainingDomain == nullptr) return false;
//     //         createHelper(child, remainingDomain + 1, depth + 1,username,choice);
//     //         return false;
//     //     }
//     //     child = child->nextSibling;
//     // }

//     // Node* newNode = new Node(part, depth + 1, node);  // 设置新节点的深度和父节点
//     // if (depth >= 1) {
//     //     strcpy(newNode->id, username.toLocal8Bit().constData());
//     // }
//     // if (depth == 1)
//     // {
//     //     char temid[100];
//     //     strncpy(temid,username.toLocal8Bit().constData(),sizeof(username)-1);
//     //     newNode->id = temid;
//     //     int temchoice=choice;
//     //     category Category = other;
//     //     if (temchoice - 1 >= 0 && temchoice - 1 < 15) { // 枚举的有效范围是 0 到 14
//     //         Category = static_cast<category>(temchoice - 1);
//     //         newNode->nodeCategory = Category;
//     //     }
//     // }
//     // newNode->nextSibling = node->firstChild;
//     // node->firstChild = newNode;
//     // delete[] part;
//     // if (remainingDomain != nullptr) {
//     //     createHelper(newNode, remainingDomain + 1, depth + 1,username,choice);
//     // }
//     // return true;
// }
