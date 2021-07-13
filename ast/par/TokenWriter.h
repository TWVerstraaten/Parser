//
// Created by pc on 09-06-21.
//

#ifndef AST_PAR_TOKENWRITER_H
#define AST_PAR_TOKENWRITER_H

#include <string>

namespace ast::par {

    class Token;
    class StructuralToken;
    class AstToken;
    class Range;
    class UnrolledAstToken;

    class TokenWriter {

      public:
        [[nodiscard]] static std::string S_TO_STRING(const Token& token, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(const StructuralToken& token, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(const UnrolledAstToken& unrolledAstToken);
        [[nodiscard]] static std::string S_TO_STRING_AS_TREE(const AstToken& node);
        [[nodiscard]] static std::string S_TO_STRING_FLAT(const AstToken& node);
    };
} // namespace ast::par

#endif // AST_PAR_TOKENWRITER_H
