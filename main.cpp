#include "Parser/Ast.h"

int main() {
    std::string eq1 = "x*x^3/(x*y)";
    Ast         ast1(eq1);
    auto*       copyNode1 = ast1.m_copy.get();

    //    std::string eq2 = "x*x";
    //    Ast         ast2(eq2);
    //    auto*       copyNode2 = ast2.m_copy.get();

    std::cout << "Simplified:\t" << *copyNode1 << '\n';
    //    std::cout << "Simplified:\t" << *copyNode2 << '\n';

    return 0;
}
