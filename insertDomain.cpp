#include "insertDomain.h"

bool insertNode(Node* root, int targetDepth,const QString &parentDomain,const QString &insertDomain ,const QString &username) {
    Node* parentNode = findNodeAtDepth(root, parentDomain.toLocal8Bit().constData(), targetDepth);
    if (parentNode) {
        createHelper(parentNode, insertDomain.toLocal8Bit().constData(), parentNode->depth,username,-1);
        return true;
    }else{
        return false;
    }
}

