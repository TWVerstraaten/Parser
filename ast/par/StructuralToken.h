//
// Created by pc on 07-06-21.
//

#ifndef AST_PAR_STRUCTURALTOKEN_H
#define AST_PAR_STRUCTURALTOKEN_H

#include "Token.h"

#include <list>
#include <variant>
#include <vector>

namespace ast::par {

    class StructuralToken {

      public:
        struct Bracketed {
            std::vector<std::list<StructuralToken>> m_tokenLists;
        };

        struct Function {
            std::string m_name;
            Bracketed   m_arguments;
        };

        typedef std::variant<Token, Bracketed, Function, std::string, double, long long int> StructuralTokenVariant;

        explicit StructuralToken(const Token& token);
        StructuralToken(const std::string& variable, Range range);
        StructuralToken(double number, Range range);
        StructuralToken(long long number, Range range);
        StructuralToken(Bracketed&& bracketed, Range range);
        StructuralToken(Function&& function, Range range);

        [[nodiscard]] bool                          holdsString() const;
        [[nodiscard]] std::string                   toString() const;
        [[nodiscard]] const StructuralTokenVariant& token() const;
        [[nodiscard]] const Range&                  range() const;

        [[nodiscard]] static StructuralToken S_MAKE_FROM_COMMA_SEPARATED(std::list<StructuralToken>&& tokenList);

      private:
        [[nodiscard]] static Bracketed S_MAKE_BRACKETED(std::list<StructuralToken>& tokenList);

        StructuralTokenVariant m_token;
        Range                  m_range;
    };
} // namespace ast::par

#endif // AST_PAR_STRUCTURALTOKEN_H
