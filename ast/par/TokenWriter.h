//
// Created by pc on 09-06-21.
//

#ifndef AST_PAR_TOKENWRITER_H
#define AST_PAR_TOKENWRITER_H

#include "AstToken.h"
#include "StructuralToken.h"
#include "Token.h"

namespace ast::par {
    class UnrolledAstToken;

    class TokenWriter {

      public:
        [[nodiscard]] static std::string S_TO_STRING(const Token& token, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(const StructuralToken::Bracketed& token, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(const StructuralToken::Function& token, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(const std::string& string, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(double val, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(long long val, const Range& range);
        [[nodiscard]] static std::string S_TO_STRING(AstToken::OPERATOR_TYPE type);
        [[nodiscard]] static std::string S_TO_STRING(const UnrolledAstToken& unrolledAstToken);
        [[nodiscard]] static std::string S_TO_STRING_AS_TREE(const std::string& prefix, const AstToken& node, bool isLeft);
    };
} // namespace ast::par

#endif // AST_PAR_TOKENWRITER_H
