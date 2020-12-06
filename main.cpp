#include "Parser/Ast.h"

int main() {
    std::string eq1 = " x^1+x^3 + y - k - y ";
    Ast         ast1(eq1);
    auto*       copyNode1 = ast1.m_copy.get();

    std::string eq2 = "x*x";
    Ast         ast2(eq2);
    auto*       copyNode2 = ast2.m_copy.get();

    std::cout << "Simplified:\t" << *copyNode1 << '\n';
    std::cout << "Simplified:\t" << *copyNode2 << '\n';

    //    const auto i = AstNode::intersect(copyNode1, copyNode2);
    //    std::cout << *i.m_common << '\n';
    //    std::cout << *i.m_firstRemainder << '\n';
    //    std::cout << *i.m_secondRemainder << '\n';
    return 0;
}
