//
// Created by pc on 09-06-21.
//

#ifndef PRS_TOKENWRITER_H
#define PRS_TOKENWRITER_H

#include "par/AstToken.h"
#include "par/StructuralToken.h"
#include "par/Token.h"

namespace ast {
    class UnrolledAstToken;

    class TokenWriter {

      public:
        [[nodiscard]] static std::string toString(const Token& token, const Range& range);
        [[nodiscard]] static std::string toString(const StructuralToken::Bracketed& token, const Range& range);
        [[nodiscard]] static std::string toString(const StructuralToken::Function& token, const Range& range);
        [[nodiscard]] static std::string toString(const std::string& string, const Range& range);
        [[nodiscard]] static std::string toString(double val, const Range& range);
        [[nodiscard]] static std::string toString(long long val, const Range& range);
        [[nodiscard]] static std::string toString(AstToken::OPERATOR_TYPE type);
        [[nodiscard]] static std::string toString(const UnrolledAstToken& unrolledAstToken);
        [[nodiscard]] static std::string toStringAsTree(const std::string& prefix, const AstToken& node, bool isLeft);
    };
} // namespace ast

#endif // PRS_TOKENWRITER_H
