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
    friend class AstToken;

    struct Bracketed {
        std::vector<std::list<StructuralToken>> m_tokenLists;
    };

    struct Function {
        std::string m_name;
        Bracketed   m_arguments;
    };

  public:
    explicit StructuralToken(const Token& token);

    StructuralToken(const std::string& variable, Range range);
    StructuralToken(double number, Range range);
    StructuralToken(long long number, Range range);

    [[nodiscard]] static StructuralToken makeFromCommaSeparated(std::list<StructuralToken>&& tokenList);

    [[nodiscard]] bool        isRawTokenOfType(Token::TYPE type) const;
    [[nodiscard]] bool        isString() const;
    [[nodiscard]] std::string toString() const;

  private:
    explicit StructuralToken(Bracketed&& multiBracketed, Range range);
    explicit StructuralToken(Function&& function, Range range);


    [[nodiscard]] std::string toString(const Token& token) const;
    [[nodiscard]] std::string toString(const Bracketed& token) const;
    [[nodiscard]] std::string toString(const Function& token) const;
    [[nodiscard]] std::string toString(const std::string& token) const;
    [[nodiscard]] std::string toString(double token) const;
    [[nodiscard]] std::string toString(long long token) const;

    [[nodiscard]] static Bracketed makeBracketed(std::list<StructuralToken>& tokenList);

    std::variant<Token, Bracketed, Function, std::string, double, long long> m_token;
    Range                                                                    m_range;
};

#endif // PRS_STRUCTURALTOKEN_H
