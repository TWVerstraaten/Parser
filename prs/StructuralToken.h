//
// Created by pc on 07-06-21.
//

#ifndef PRS_STRUCTURALTOKEN_H
#define PRS_STRUCTURALTOKEN_H

#include "Token.h"

#include <list>
#include <variant>
#include <vector>

class StructuralToken {
    friend class StructuralTokenizer;
    friend class AstToken;
    friend class TokenWriter;

    struct Bracketed {
        std::vector<std::list<StructuralToken>> m_tokenLists;
    };

    struct Function {
        std::string m_name;
        Bracketed   m_arguments;
    };

  public:
    explicit StructuralToken(const Token& token);

    [[nodiscard]] bool        isString() const;
    [[nodiscard]] std::string toString() const;

    [[nodiscard]] static StructuralToken makeFromCommaSeparated(std::list<StructuralToken>&& tokenList);

  private:
    StructuralToken(const std::string& variable, Range range);
    StructuralToken(double number, Range range);
    StructuralToken(long long number, Range range);
    StructuralToken(Bracketed&& multiBracketed, Range range);
    StructuralToken(Function&& function, Range range);

    [[nodiscard]] static Bracketed makeBracketed(std::list<StructuralToken>& tokenList);

    std::variant<Token, Bracketed, Function, std::string, double, long long> m_token;
    Range                                                                    m_range;
};

#endif // PRS_STRUCTURALTOKEN_H
