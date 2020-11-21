#include "Parser/Ast.h"

#include <iostream>

int main() {
    std::string eq = "1+4+1-3+2";
    Ast         ast(eq);
    std::cout << "Result = \t" + ast.toString() << '\n';
    return 0;
}
