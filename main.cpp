#include "Parser/Ast.h"
#include "Parser/AstNode/AstNodeInteger.h"
#include "Parser/AstNode/AstNodeUnaryMinus.h"
#include "Parser/AstNode/Pattern/Pattern.h"

#include <iostream>

int main() {

    AstNode* node = new AstNodeUnaryMinus(
        std::unique_ptr<AstNode>(new AstNodeUnaryMinus(std::unique_ptr<AstNode>(new AstNodeInteger(0)))));

    using TOKEN = Pattern::PATTERN_TOKEN;
    Pattern pattern({TOKEN::MINUS, TOKEN::CH0, TOKEN::MINUS, TOKEN ::CH0, TOKEN ::NAME_A, TOKEN::CLOSE, TOKEN::CLOSE,
                     TOKEN::CLOSE});
    assert(pattern.match(node));
    if (pattern.node("A") != nullptr) {
        std::cout << pattern.node("A")->toString() << '\n';
    }

    std::string eq = "- (- (-4))";
    Ast         ast(eq);
    std::cout << ast.toString() << '\n';
    return 0;
}
