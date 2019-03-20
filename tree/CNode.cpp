#include "CNode.h"

CNode *CNode::getNode(
    const ::std::string &id,
    const ::std::vector<CNode *> &nodes)
{
    for (::std::vector<CNode *>::const_iterator i = nodes.begin(); i != nodes.end(); i++) {
        CNode *node = *i;
        if (node->getID() == id) {
            return node;
        }
    }
    return 0;
}

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

CNode *CNode::addChild(
    const char *childID)
{
    CNode *child = new CNode(childID);
    fChildren.push_back(child);
    return child;
}

const ::std::vector<CNode *> &CNode::getChildren()
{
    return fChildren;
}

