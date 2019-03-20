#include <iostream>
#include "CNode.h"

static const char *const kCompany[] = 
{"4,3,6"
,"2,7"
,"0,1"
,"2,5"
,"0,8"
};
static const int kNumOfCompany(sizeof(kCompany) / sizeof(kCompany[0]));

::std::vector< ::std::string > split(
    const ::std::string &line)
{
    ::std::vector< ::std::string > ret;
    ::std::string::size_type startPos = 0;
    ::std::string::size_type endPos = line.find(",");
    while (true) {
        ::std::string part = (endPos == ::std::string::npos) ? 
            line.substr(startPos) : line.substr(startPos, endPos - startPos);
        ret.push_back(part);
        if (endPos == ::std::string::npos) {
            break;
        }
        startPos = endPos + 1;
        endPos = line.find(",", endPos + 1);
    }
    return ret;
}

void printRec(
    const ::std::string &indent,
    CNode *node)
{
    ::std::cout << indent << node->getID() << ::std::endl;
    const ::std::vector<CNode *> children = node->getChildren();
    for (::std::vector<CNode *>::const_iterator i = children.begin(); i != children.end(); i++) {
        ::std::string nextIndent = indent + "  ";
        printRec(nextIndent, *i);
    }
}

void printChildren(
    const ::std::string &id,
    const ::std::vector<CNode *> &nodes)
{
    for (::std::vector<CNode *>::const_iterator i = nodes.begin(); i != nodes.end(); i++) {
        CNode *node = *i;
        if (node->getID() == id) {
            printRec("", node);
            return;
        }
    }
}

int main()
{
    CNode *root = new CNode("root");
    for (int i = 0; i < kNumOfCompany; i++) {
        ::std::vector< ::std::string > ids = split(kCompany[i]);
        CNode *cur = root;
        for (::std::vector< ::std::string >::const_iterator i = ids.begin(); i != ids.end(); i++) {
            cur = cur->createChildIfNotExist(i->c_str());
        }
    }

    printChildren("4", root->getChildren());
    printChildren("2", root->getChildren());
    printChildren("0", root->getChildren());

    delete root;

    return 0;
}

