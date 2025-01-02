#include "categoryCount.h"

// 递归遍历所有叶子节点，并进行类别统计
void categoryCount(Node* node, int* cateCount) {
    // 如果当前节点为空，则直接返回
    if (!node) {
        return;
    }
    // 递归遍历所有叶子节点
    // 如果当前节点是叶子节点（没有子节点）
    if (node->firstChild == nullptr) {
        Node* temnode = node;
        // 向上查找节点，直到找到深度为 2 的节点
        // while (temnode != nullptr && temnode->depth != 2) {
        //     temnode = temnode->parent;
        // }

        // // 如果找到深度为 2 的父节点并且该节点有有效的类别

        // if (temnode && temnode->depth == 2) {
        //     // 假设类别是一个整数且范围在 0 到 14 之间
        //     int category = static_cast<int>(temnode->nodeCategory);
        //     if (category >= 0 && category < 15) {
        //         cateCount[category]++;
        //     }
        // }
        int temp=temnode->depth;
        while (temnode != nullptr && temnode->depth != 2) {
             temnode = temnode->parent;
        }
        if (temnode && temnode->depth == 2) {
                // 假设类别是一个整数且范围在 0 到 14 之间
                int category = static_cast<int>(temnode->nodeCategory);
                if (category >= 0 && category < 15) {
                    cateCount[category]+=temp-2;
                }
        }
    }
    // 继续递归处理当前节点的子节点和兄弟节点
    if (node->firstChild) {
        categoryCount(node->firstChild, cateCount); // 递归处理子节点
    }
    if (node->nextSibling) {
        categoryCount(node->nextSibling, cateCount); // 递归处理兄弟节点
    }
}
void categoryCount2(Node* node, int* cateCount)
{
    if(!node)
    {
        return;
    }
    if(node->depth==2)
    {
        cateCount[static_cast<int>(node->nodeCategory)]++;
    }
    categoryCount2(node->firstChild,cateCount);
    categoryCount2(node->nextSibling,cateCount);
}
