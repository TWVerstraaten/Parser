#include "Parser/Ast.h"

int main() {
    std::string eq1 = "0/0";
    Ast         ast1(eq1);
    auto*       copyNode1 = ast1.m_copy.get();

    std::cout << "Simplified:\t" << *copyNode1 << '\n';
    return 0;
}
