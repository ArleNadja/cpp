/**
 * Tree構造のNodeのサンプル。
 * 実行結果は以下。

 $ ./a.out 
 root
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

static const char *const kIDs[] = 
{"4,3,6"
,"2,7"
,"0,1"
,"2,5"
,"0,8"
};
static const int kNumOfIDs(sizeof(kIDs) / sizeof(kIDs[0]));

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

int main()
{
    CNode *root = new CNode("root");
    for (int i = 0; i < kNumOfIDs; i++) {
        ::std::vector< ::std::string > ids = split(kIDs[i]);
        CNode *cur = root;
        for (::std::vector< ::std::string >::const_iterator i = ids.begin(); i != ids.end(); i++) {
            cur = cur->createChildIfNotExist(i->c_str());
        }
    }

    printRec("", root);

    delete root;

    return 0;
}


