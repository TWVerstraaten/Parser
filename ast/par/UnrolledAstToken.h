//
// Created by pc on 14-06-21.
//

#ifndef PRS_UNROLLEDASTTOKEN_H
#define PRS_UNROLLEDASTTOKEN_H

#include "../../gen/Number.h"
#include "ReservedToken.h"
#include "VectorToken.h"

#include <map>
#include <variant>
#include <vector>

namespace ast::par {

    class AstToken;

    class UnrolledAstToken {

      public:
        struct Plus {};
        struct Minus {};
        struct Times {};
        struct Divide {};
        struct Power {};
        struct UnaryMinus {};

        typedef std::variant<Plus, Minus, Times, Divide, Power, UnaryMinus, ReservedToken, VectorToken, std::string, double, long long> UnrolledToken;

        explicit UnrolledAstToken(const AstToken& astToken);

        void simplify();

        void setVariableInPlace(const std::string& variable, const gen::Number& number);

        [[nodiscard]] bool                                 isNumeric() const;
        [[nodiscard]] double                               toDouble() const;
        [[nodiscard]] gen::Number                          toNumber() const;
        [[nodiscard]] std::string                          toString() const;
        [[nodiscard]] const UnrolledToken&                 token() const;
        [[nodiscard]] const std::vector<UnrolledAstToken>& children() const;

      private:
        void simplifyFunction();
        void unWrap1DVectors();
        void setVariableInternal(const std::string& variable, const gen::Number& number);

        UnrolledToken                 m_token;
        std::vector<UnrolledAstToken> m_children;
    };
} // namespace ast::par

#endif // PRS_UNROLLEDASTTOKEN_H
