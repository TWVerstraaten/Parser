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

    struct Bracketed {
        std::list<StructuralToken> m_tokenList;
    };

    struct MultiBracketed {
        std::vector<std::list<StructuralToken>> m_tokenLists;
    };

    struct Function {
        std::string                             m_name;
        std::variant<Bracketed, MultiBracketed> m_arguments;
    };

  public:
    explicit StructuralToken(const Token& token);
    explicit StructuralToken(const MultiBracketed& multiBracketed);
    explicit StructuralToken(const Bracketed& bracketed);
    explicit StructuralToken(const Function& function);

    [[nodiscard]] bool        isRawTokenOfType(Token::TOKEN_TYPE type) const;
    [[nodiscard]] std::string toString() const;

  private:
    [[nodiscard]] std::string toString(const Token& token) const;
    [[nodiscard]] std::string toString(const Bracketed& token) const;
    [[nodiscard]] std::string toString(const MultiBracketed& token) const;
    [[nodiscard]] std::string toString(const Function& token) const;

    std::variant<Token, Bracketed, MultiBracketed, Function> m_token;
    size_t                                                   m_startIndexInString;
    size_t                                                   m_endIndexInString;
};

#endif // PARSER_STRUCTURALTOKEN_H
