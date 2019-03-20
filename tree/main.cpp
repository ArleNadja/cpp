/**
 * Tree構造のNodeのテスト実装。
 *
 $ a.out
 4
   3
     6
 2
   7
   5
 0
   1
   8
 */

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
    const ::std::vector<CNode *> &roots)
{
    CNode *root = CNode::getNode(id, roots);
    if (root != 0) {
        printRec("", root);
    }
}

int main()
{
    ::std::vector<CNode *> roots;
    for (int i = 0; i < kNumOfCompany; i++) {
        ::std::vector< ::std::string > ids = split(kCompany[i]);
        CNode *cur = 0;
        for (::std::vector< ::std::string >::const_iterator i = ids.begin(); i != ids.end(); i++) {
            if (cur == 0) {
                cur = CNode::getNode(*i, roots);
                if (cur == 0) {
                    cur = new CNode(i->c_str());
                    roots.push_back(cur);
                }
            } else {
                CNode *child = CNode::getNode(*i, cur->getChildren());
                if (child == 0) {
                    child = cur->addChild(i->c_str());
                }
                cur = child;
            }
        }
    }

    printChildren("4", roots);
    printChildren("2", roots);
    printChildren("0", roots);

    for (::std::vector<CNode *>::iterator i = roots.begin(); i != roots.end(); i++) {
        delete *i;
    }
}
