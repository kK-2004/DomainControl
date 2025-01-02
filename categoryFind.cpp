#include"categoryfind.h"
#include <Qvector>
void categoryfind(Node* node, int choice,QVector<QString> &temdomain)
{
    // 检查当前节点是否有效
    if (node == nullptr) return;
    // 遍历当前节点的所有子节点
    // 设置分类，默认值是 other
    category Category = other;
    Category = static_cast<category>(choice - 1);
    if (node->depth == 2)
    {
        if (node->nodeCategory == Category) {
            temdomain.append(node->name);
        }
    }
    // 递归遍历子节点
    categoryfind(node->firstChild, choice,temdomain); // 递归到子节点
    // 遍历兄弟节点
    categoryfind(node->nextSibling, choice,temdomain);
}

