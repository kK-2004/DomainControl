#include "userdomainGet.h"
#include "tools.h"
#include <iostream>
#include <cstring>

// 计算路径中以分隔符计数的子串数量
int countSubstrings(const char* str, char delimiter) {
    int count = 0;
    while (*str) {
        if (*str == delimiter) {
            count++;
        }
        str++;
    }
    return count + 1;  // 子串数量 = 分隔符数量 + 1
}

// 存储路径的函数
void storeValidPath(char validPaths[MAX_PATHS][MAX_PATH_LEN], int& validPathCount, const char* path) {
    if (validPathCount < MAX_PATHS) {
        strncpy(validPaths[validPathCount], path, MAX_PATH_LEN - 1);
        validPaths[validPathCount][MAX_PATH_LEN - 1] = '\0';  // 确保字符串以 '\0' 结尾
        validPathCount++;
    }
}

void printSubtreeUser(Node* node, char* path, int depth, const char* user, char validPaths[MAX_PATHS][MAX_PATH_LEN], int& validPathCount) {
    // 确保节点存在
    if (!node) return;

    // 保存当前深度，用于恢复 path
    int currentDepth = depth;
    //如果是管理员
    if(strcmp(user,"manager")==0){
        if (node->name && strlen(node->name) > 0) {
            bool shouldAddNode = (depth < 2) || node->owner;

            if (shouldAddNode) {
                int len = strlen(node->name);
                if (depth > 0) {
                    path[depth] = '.';  // 添加分隔符 '.'
                    depth++;
                }
                strncpy(path + depth, node->name, len);  // 拼接当前节点名称
                depth += len;
                path[depth] = '\0';  // 确保字符串以 '\0' 结尾

                // 处理倒序路径
                if (path[0] != '\0') {
                    char* reversedPath = reverseDomainParts(path);
                    int substrCount = countSubstrings(reversedPath, '.');
                    if (substrCount >= 2) {
                        storeValidPath(validPaths, validPathCount, reversedPath);  // 存储满足条件的路径
                    }
                    delete[] reversedPath;
                }
            }
        }
    }else{
        // 检查深度和用户匹配
        if (node->name && strlen(node->name) > 0) {
            bool shouldAddNode = (depth < 2) || (node->owner && strcmp(node->owner, user) == 0);

            if (shouldAddNode) {
                int len = strlen(node->name);
                if (depth > 0) {
                    path[depth] = '.';  // 添加分隔符 '.'
                    depth++;
                }
                strncpy(path + depth, node->name, len);  // 拼接当前节点名称
                depth += len;
                path[depth] = '\0';  // 确保字符串以 '\0' 结尾

                // 处理倒序路径
                if (path[0] != '\0') {
                    char* reversedPath = reverseDomainParts(path);
                    int substrCount = countSubstrings(reversedPath, '.');
                    if (substrCount >= 2) {
                        storeValidPath(validPaths, validPathCount, reversedPath);  // 存储满足条件的路径
                    }
                    delete[] reversedPath;
                }
            }
        }
    }


    // 递归打印子节点
    printSubtreeUser(node->firstChild, path, depth, user, validPaths, validPathCount);

    // 递归打印兄弟节点，恢复深度
    printSubtreeUser(node->nextSibling, path, currentDepth, user, validPaths, validPathCount);
}

// 打印所有有效路径
void printValidPaths(char validPaths[MAX_PATHS][MAX_PATH_LEN], int validPathCount) {
    for (int i = 0; i < validPathCount; i++) {
        std::cout << validPaths[i] << std::endl;
    }
}


// void collectFullDomainPaths(Node* node, char* pathArray[], int& pathIndex,const QString &username) {
//     if (!node) return;

//     // 递归遍历所有子节点
//     Node* child = node->firstChild;
//     while (child) {
//         // 如果当前子节点是叶子节点（没有子节点），则将路径存储到数组
//         if (child->firstChild == nullptr) {
//             char* domainPath = printFullDomainPath(child);
//             if (domainPath && pathIndex < 100 &&(strcmp(child->owner,username.toLocal8Bit().constData())==0 || strcmp(child->owner,""))) {  // 确保路径数组没有溢出
//                 pathArray[pathIndex] = domainPath;  // 存储路径
//                 pathIndex++;
//             }
//         }
//         else {
//             // 否则递归遍历该子树
//             collectFullDomainPaths(child, pathArray, pathIndex,username);
//         }
//         child = child->nextSibling;
//     }
// }
