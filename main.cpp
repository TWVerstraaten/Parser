#include "Parser/Ast.h"
#include "Parser/AstNode/AstNodeAdd.h"
#include "Parser/AstNode/AstNodeInteger.h"
#include "Parser/AstNode/AstNodeMul.h"
#include "Parser/AstNode/AstNodeUnaryMinus.h"
#include "Parser/AstNode/AstNodeVar.h"
#include "Parser/AstNode/Pattern/Matcher.h"
#include "Parser/AstNode/Pattern/Pattern.h"

#include <iostream>

int main() {
    //    auto* node = new AstNodeUnaryMinus(std::unique_ptr<AstNode>(new AstNodeUnaryMinus(
    //        std::unique_ptr<AstNode>(new AstNodeUnaryMinus(std::unique_ptr<AstNode>(new AstNodeInteger(2)))))));
    //
    //    using M = Matcher::MATCH_TOKEN;
    //    Matcher m{{M::ANY, M::ANY, M::MINUS, M::NAME0, M::CLOSE, M::CLOSE, M::CLOSE}};
    //
    //    if (m.match(node)) {
    //        std::cout << "found\t" << m.node(0)->toString() << '\n';
    //    }

    std::string eq = "  -  1 - 2 + x *z - z*x +3 +y + sin(t) * y";
    Ast         ast(eq);
    auto*       rootNode = ast.m_rootNode.get();
    auto*       copyNode = ast.m_copy.get();
    std::cout << rootNode->toString() << '\n';
    std::cout << copyNode->toString() << '\n';

    return 0;
}
