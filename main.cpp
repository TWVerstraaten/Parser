#include "Parser/Ast.h"

#include <iostream>

int main() {

    std::string eq = " (b*c*a) + b*a  - a * b";
    Ast         ast(eq);
    auto*       rootNode = ast.m_rootNode.get();
    auto*       copyNode = ast.m_copy.get();
    std::cout << rootNode->toString() << '\n';
    std::cout << copyNode->toString() << '\n';

    return 0;
}
