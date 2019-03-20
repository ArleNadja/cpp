#ifndef _C_NODE_H
#define _C_NODE_H

#include <string>
#include <vector>

class CNode {
public:
    CNode(
        const char *id);

    virtual ~CNode();

    ::std::string getID();

    CNode *createChildIfNotExist(
        const char *childID);

    const ::std::vector<CNode *> &getChildren();

private:
    ::std::string fID;
    ::std::vector<CNode *> fChildren;
};

#endif /* _C_NODE_H */

