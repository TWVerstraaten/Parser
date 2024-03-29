//
// Created by pc on 19-11-20.
//

#ifndef PRS_TOKENIZER_H
#define PRS_TOKENIZER_H

#include <list>
#include <string>

namespace fml::prs {
    class Tokenizer {
      public:
        enum class TOKEN_TYPE {
            REFERENCE,
            IDENTIFIER,
            INTEGER,
            COMMA,
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

        typedef std::__cxx11::list<Token> TokenList;
        explicit Tokenizer(const std::string& trimmedString);
        [[nodiscard]] TokenList&  tokenList();
        [[nodiscard]] std::string toString(bool readable = false) const;

        static std::string toString(const TokenList& tokens, bool readable = false);

      private:
        std::string::const_iterator parseValueType(std::string::const_iterator it, std::string::const_iterator end);
        void                        findFunctionIdentifiers();

        TokenList m_tokens;
    };

    inline bool operator==(const prs::Tokenizer::Token& left, const prs::Tokenizer::Token& right) {
        return left.m_type == right.m_type && left.m_string == right.m_string;
    }

    inline bool operator!=(const prs::Tokenizer::Token& left, const prs::Tokenizer::Token& right) {
        return !(left == right);
    }
} // namespace fml::prs

#endif // PRS_TOKENIZER_H
