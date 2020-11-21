#include "Parser/Ast.h"

#include <iostream>

int main() {
    std::string eq = "x * x + x * x";
    Ast         ast(eq);
    std::cout << ast.toString() << '\n';
    return 0;
}
