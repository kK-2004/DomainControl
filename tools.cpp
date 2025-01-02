#include "tools.h"
#include <QDebug>
#include <sstream>
#include "createTree.h"
#include "saveDomain.h"

extern Node* root;
// 从完整域名中去掉子域名部分并返回剩余部分
char* removeSubdomain(const char* subdomain, const char* fullDomain) {
    size_t subdomainLength = strlen(subdomain);

    // 检查前缀是否匹配且匹配部分后紧接着一个'.'
    if (strncmp(subdomain, fullDomain, subdomainLength) == 0 &&
        fullDomain[subdomainLength] == '.') {
        // 提取剩余部分
        const char* remaining = fullDomain + subdomainLength + 1; // 跳过'.'
        char* result = new char[strlen(remaining) + 1];
        strcpy(result, remaining);
        return result;
    }

    // 如果不匹配，返回空字符串
    char* emptyResult = new char[1];
    emptyResult[0] = '\0';
    return emptyResult;
}

// 反转域名部分并返回反转后的字符串
char* reverseDomainParts(const char* input) {
    size_t len = strlen(input);
    char* mutableInput = new char[len + 1];
    strcpy(mutableInput, input);

    char* result = new char[len + 1];
    char* tempResult = result;

    char* token = strtok(mutableInput, ".");
    char* parts[100];
    int partCount = 0;

    while (token != nullptr) {
        parts[partCount++] = token;
        token = strtok(nullptr, ".");
    }

    for (int i = partCount - 1; i >= 0; --i) {
        if (i != partCount - 1) {
            *tempResult++ = '.';
        }
        strcpy(tempResult, parts[i]);
        tempResult += strlen(parts[i]);
    }

    delete[] mutableInput;

    return result;
}


void printSubtreeReversed(Node* node, char* path, int depth) {
    // 确保节点存在
    if (!node) return;

    // 保存当前深度，用于恢复 path
    int currentDepth = depth;

    // 确保节点名称有效
    if (node->name && strlen(node->name) > 0) {
        int len = strlen(node->name);
        if (depth > 0) {
            path[depth] = '.';  // 添加分隔符 '.'
            depth++;
        }
        strncpy(path + depth, node->name, len);  // 拼接当前节点名称
        depth += len;
        path[depth] = '\0';  // 确保字符串以 '\0' 结尾
    }

    // 打印当前路径（倒序）
    if (path[0] != '\0') {  // 确保路径非空
        char* reversedPath = reverseDomainParts(path);
        qDebug()<<reversedPath;
        delete[] reversedPath;
    }

    // 递归打印子节点
    printSubtreeReversed(node->firstChild, path, depth);

    // 递归打印兄弟节点，恢复深度
    printSubtreeReversed(node->nextSibling, path, currentDepth);
}


// void printNodesAtDepth(Node* node, int targetDepth) {
//     if (!node) return;

//     if (node->depth == targetDepth) {
//         std::cout << node->name << "\n";
//     }

//     printNodesAtDepth(node->firstChild, targetDepth);
//     printNodesAtDepth(node->nextSibling, targetDepth);
// }

Node* findNodeAtDepth(Node* node, const char* name, int targetDepth) {
    if (!node) return nullptr;

    if (node->depth == targetDepth && strcmp(node->name, name) == 0) {
        return node;
    }

    Node* result = findNodeAtDepth(node->firstChild, name, targetDepth);
    if (result) return result;

    return findNodeAtDepth(node->nextSibling, name, targetDepth);
}

char* printFullDomainPath(Node* node) {
    if (!node) return nullptr;

    char path[1024] = { 0 }; // 用于存储域名路径
    size_t pathLen = 0;
    Node* current = node;

    // 从下往上拼接域名
    while (current) {
        size_t nameLen = strlen(current->name);

        // 检查是否超出路径缓冲区
        if (pathLen + nameLen + 1 > sizeof(path)) {
            return nullptr;
        }

        // 拼接当前域名部分
        memmove(path + nameLen + 1, path, pathLen + 1); // 向后移动现有路径
        memcpy(path, current->name, nameLen);
        path[nameLen] = (pathLen > 0) ? '.' : '\0'; // 添加分隔符或结束符
        pathLen += nameLen + 1;

        current = current->parent;
    }

    // 使用 reverseDomainParts 反转域名
    char* reversedPath = reverseDomainParts(path);
    //std::cout << "完整域名路径: " << reversedPath << std::endl;
    return reversedPath;

    delete[] reversedPath; // 释放动态分配的内存
}



void loadTreeFromFile(Node* root, const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string owner, domain, categoryStr;

        if (!(iss >> owner >> domain >> categoryStr)) {
            continue;
        }

        // 解析域名
        std::vector<std::string> parts;
        size_t pos = 0;
        std::string token;
        while ((pos = domain.find('.')) != std::string::npos) {
            token = domain.substr(0, pos);
            parts.push_back(token);
            domain.erase(0, pos + 1);
        }
        parts.push_back(domain); // 添加最后一个部分

        // 确保至少有一个部分（即顶级域名）
        if (parts.empty()) {
            continue;
        }

        // 反转域名部分的顺序，因为在文件中它们是按从叶子节点到根节点的顺序存储的
        std::reverse(parts.begin(), parts.end());

        Node* current = root;
        for (int depth = 0; depth < parts.size(); ++depth) {
            Node* child = findOrCreateNode(current, parts[depth].c_str(), depth + 1);
            if (!child) {
                break; // 如果无法创建或找到节点，则停止构建
            }
            if (depth == parts.size() - 1) { // 最后一层节点
                strcpy(child->owner, owner.c_str());
                child->nodeCategory = stringToCategory(categoryStr);
            }
            current = child;
        }
    }

    file.close();
}

// 辅助函数：查找或创建节点
Node* findOrCreateNode(Node* root, const char* name, int depth) {
    Node* current = root->firstChild;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->nextSibling;
    }

    // 如果未找到，创建新节点
    Node* newNode = new Node(name, depth, root);
    newNode->nextSibling = root->firstChild;
    root->firstChild = newNode;
    return newNode;
}

void collectFullDomainPaths(Node* node, char* pathArray[], int& pathIndex) {
    if (!node) return;

    // 递归遍历所有子节点
    Node* child = node->firstChild;
    while (child) {
        // 如果当前子节点是叶子节点（没有子节点），则将路径存储到数组
        if (child->firstChild == nullptr) {
            char* domainPath = printFullDomainPath(child);
            if (domainPath && pathIndex < 100) {  // 确保路径数组没有溢出
                pathArray[pathIndex] = domainPath;  // 存储路径
                pathIndex++;
            }
        }
        else {
            // 否则递归遍历该子树
            collectFullDomainPaths(child, pathArray, pathIndex);
        }
        child = child->nextSibling;
    }
}
