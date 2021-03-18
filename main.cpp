#include "Parser/Ast.h"

int main() {
    std::string eq1 = "-2 * 2 * 2 / 2";
    Ast         ast1(eq1);
    auto        copyNode1 = ast1.simplify();

    std::cout << "Simplified:\t" << copyNode1.toString() << '\n';
    return 0;
}
