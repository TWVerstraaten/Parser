#include "Parser/Ast.h"

int main() {
    std::string eq1 = "x*(f(2.+ (2*g(a))) +2)";
    Ast         ast1(eq1);
    auto*       copyNode1 = ast1.m_copy.get();

    std::cout << "Simplified:\t" << *copyNode1 << '\n';
    return 0;
}
