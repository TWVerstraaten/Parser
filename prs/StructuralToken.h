//
// Created by pc on 07-06-21.
//

#ifndef PARSER_STRUCTURALTOKEN_H
#define PARSER_STRUCTURALTOKEN_H

#include "Token.h"

#include <list>
#include <variant>
#include <vector>

class StructuralToken {

    friend class Tokenizer;

    struct MultiBracketed {
        std::vector<std::list<StructuralToken>> m_tokenLists;
    };

    struct Function {
        std::string    m_name;
        MultiBracketed m_arguments;
    };

  public:
    explicit StructuralToken(const Token& token);

    StructuralToken(const std::string& variable, size_t startIndex, size_t endIndex);
    StructuralToken(double number, size_t startIndex, size_t endIndex);
    StructuralToken(long long number, size_t startIndex, size_t endIndex);

    [[nodiscard]] static StructuralToken makeFromCommaSeparated(std::list<StructuralToken>&& tokenList);

    [[nodiscard]] bool        isRawTokenOfType(Token::TOKEN_TYPE type) const;
    [[nodiscard]] bool        isString() const;
    [[nodiscard]] std::string toString() const;

  private:
    explicit StructuralToken(MultiBracketed&& multiBracketed, size_t startIndex, size_t endIndex);
    explicit StructuralToken(Function&& function, size_t startIndex, size_t endIndex);

    [[nodiscard]] std::string toString(const Token& token) const;
    [[nodiscard]] std::string toString(const MultiBracketed& token) const;
    [[nodiscard]] std::string toString(const Function& token) const;
    [[nodiscard]] std::string toString(const std::string& token) const;
    [[nodiscard]] std::string toString(double token) const;
    [[nodiscard]] std::string toString(long long token) const;

    [[nodiscard]] static MultiBracketed makeBracketed(std::list<StructuralToken>& tokenList);

    std::variant<Token, MultiBracketed, Function, std::string, double, long long> m_token;
    size_t                                                                        m_startIndex;
    size_t                                                                        m_endIndex;
};

#endif // PARSER_STRUCTURALTOKEN_H
