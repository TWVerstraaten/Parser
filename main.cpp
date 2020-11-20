#include "Parser/Ast.h"
#include "Parser/Tokenizer.h"

#include <iostream>

int main() {
    std::string eq = "cos(1 + sin(x + 4))";
    Ast         ast(eq);
    std::cout << "Result = \t" + ast.toString() << '\n';
    return 0;
}
