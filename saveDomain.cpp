#include"saveDomain.h"

#include <QDebug>

void saveTreeToFile(Node* root, const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {

        return;
    }

    std::vector<Node*> nodesToVisit;
    nodesToVisit.push_back(root);

    while (!nodesToVisit.empty()) {
        Node* node = nodesToVisit.back();
        nodesToVisit.pop_back();

        // 只保存深度 >= 2 的节点
        if (node->depth >= 2 && node->owner[0] != '\0') {
            if (node->depth==2)
            {
                file << node->owner << " " << node->name << "." << node->parent->name << " "
                     << categoryToString(node->nodeCategory) << std::endl;
            }
            if (node->depth==3)
            {
                file << node->owner << " " << node->name << "." << node->parent->name
                     <<"." << node->parent->parent->name << " "
                     << categoryToString(node->parent->nodeCategory) << std::endl;
            }
            if (node->depth==4)
            {
                file << node->owner << " " << node->name << "." << node->parent->name
                     <<"." << node->parent->parent->name <<"." << node->parent->parent->parent->name << " "
                     << categoryToString(node->parent->nodeCategory) << std::endl;
            }
        }

        // 添加子节点到待访问列表
        Node* child = node->firstChild;
        while (child) {
            nodesToVisit.push_back(child);
            child = child->nextSibling;
        }
    }

    file.close();
}
