#include <iostream>
#include "CNode.h"

CNode::CNode(
    const char *id)
: fID(id)
, fChildren()
{
    // Nothig to do.
}

CNode::~CNode()
{
    for (::std::vector<CNode *>::iterator i = fChildren.begin(); i != fChildren.end(); i++) {
        delete *i;
    }
}

::std::string CNode::getID()
{
    return fID;
}

CNode *CNode::createChildIfNotExist(
    const char *childID)
{
    for (::std::vector<CNode *>::const_iterator i = fChildren.begin(); i != fChildren.end(); i++) {
        CNode *child = *i;
        if (child->getID() == childID) {
            return child;
        }
    }
    CNode *child = new CNode(childID);
    fChildren.push_back(child);
    return child;
}

const ::std::vector<CNode *> &CNode::getChildren()
{
    return fChildren;
}

