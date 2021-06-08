//
// Created by pc on 07-06-21.
//

#ifndef PARSER_ASTTOKEN_H
#define PARSER_ASTTOKEN_H

#include "StructuralToken.h"
#include "Token.h"

#include <memory>
#include <set>
#include <variant>
#include <vector>

class AstToken {

    struct Function {
        std::string m_name;
        size_t      m_argumentCount;
    };

    struct Vector {
        size_t m_argumentCount;
    };

    enum class TYPE { PLUS, MINUS, TIMES, DIVIDE, POWER, UNARY_MINUS, EQUALS };

  public:
    explicit AstToken(const std::list<StructuralToken>& structuralTokens);
    AstToken(const StructuralToken::Bracketed& bracketed, Range range);
    AstToken(const StructuralToken::Function& function, Range range);
    AstToken(const std::string& string, Range range);
    AstToken(long long value, Range range);
    AstToken(double value, Range range);
    AstToken(TYPE type, AstToken astToken, Range range);
    AstToken(TYPE type, AstToken left, AstToken right, Range range);

    void printTree() const;

  private:
    static void replaceUnaryMinuses(std::list<std::variant<AstToken, Token>>& tempTokens);
    static void        replacePowers(std::list<std::variant<AstToken, Token>>& tempTokens);
    static void        replaceTimesDivide(std::list<std::variant<AstToken, Token>>& tempTokens);
    static void        replacePlusMinus(std::list<std::variant<AstToken, Token>>& tempTokens);
    void        printTree(const std::string& prefix, const AstToken& node, bool isLeft) const;
    [[nodiscard]] static std::list<std::variant<AstToken, Token>>::iterator tokenIt(std::list<std::variant<AstToken, Token>>& tempTokens,
                                                                                    std::list<std::variant<AstToken, Token>>::iterator it,
                                                                                    const std::set<Token::TOKEN_TYPE>& types);

    std::variant<TYPE, Function, Vector, std::string, double, long long> m_token;
    std::vector<AstToken>                                                m_children;
    Range                                                                m_range;
};

#endif // PARSER_ASTTOKEN_H
