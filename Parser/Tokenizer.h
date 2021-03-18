//
// Created by pc on 19-11-20.
//

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

#include <list>
#include <string>

class Tokenizer {
  public:
    enum class TOKEN_TYPE {
        REFERENCE,
        IDENTIFIER,
        INTEGER,
        BIN_OP_TERM,
        BIN_OP_FAC,
        BIN_OP_EXPR,
        UNARY_OP,
        LEFT_BR,
        RIGHT_BR,
        FUN_OPEN_BR,
        FUN_CLOSE_BR,
        FUNCTION
    };

    struct Token {
        TOKEN_TYPE  m_type;
        std::string m_string;
    };

    typedef std::list<Token> TokenList;
    explicit Tokenizer(const std::string& trimmedString);
    [[nodiscard]] TokenList&  tokenList();
    [[nodiscard]] std::string toString(bool readable = false) const;

    static std::string toString(const TokenList& tokens, bool readable = false);

  private:
    std::string::const_iterator parseValueType(std::string::const_iterator it, std::string::const_iterator end);
    void                        findFunctionIdentifiers();

    TokenList m_tokens;
};

inline bool operator==(const Tokenizer::Token& left, const Tokenizer::Token& right) {
    return left.m_type == right.m_type && left.m_string == right.m_string;
}

inline bool operator!=(const Tokenizer::Token& left, const Tokenizer::Token& right) {
    return !(left == right);
}

#endif // PARSER_TOKENIZER_H
