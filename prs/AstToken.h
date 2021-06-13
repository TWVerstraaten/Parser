//
// Created by pc on 07-06-21.
//

#ifndef PRS_ASTTOKEN_H
#define PRS_ASTTOKEN_H

#include "CustomFunction.h"
#include "ReservedFunction.h"
#include "StructuralToken.h"
#include "Token.h"

#include <memory>
#include <set>
#include <variant>
#include <vector>

class ParserInfo;

class AstToken {

    friend class TokenWriter;
    friend class Ast;

    struct Vector {
        size_t m_argumentCount;
    };

    struct Empty {};

    enum class OPERATOR_TYPE { PLUS, MINUS, TIMES, DIVIDE, POWER, UNARY_MINUS, EQUALS };

  public:
    explicit AstToken(const std::list<StructuralToken>& structuralTokens, ParserInfo& info);
    AstToken(const StructuralToken::Bracketed& bracketed, Range range, ParserInfo& info);
    AstToken(const StructuralToken::Function& function, Range range, ParserInfo& info);
    AstToken(const std::string& string, Range range, ParserInfo& info);
    AstToken(long long value, Range range, ParserInfo& info);
    AstToken(double value, Range range, ParserInfo& info);
    AstToken(OPERATOR_TYPE type, AstToken astToken, Range range, ParserInfo& info);
    AstToken(OPERATOR_TYPE type, AstToken left, AstToken right, Range range, ParserInfo& info);

    //    void replaceVariables(const std::vector<std::pair<std::string, )

    [[nodiscard]] std::set<CustomFunction> dependsOn() const;
    [[nodiscard]] std::set<std::string>    variablesUsed() const;
    [[nodiscard]] std::set<std::string>    declaredVariables() const;

    [[nodiscard]] static std::string printTree(const AstToken& root);

  private:
    typedef std::variant<AstToken, Token> TempToken;
    typedef std::list<TempToken>          TempTokenList;

    static void replaceUnaryMinuses(TempTokenList& tempTokens, ParserInfo& info);
    static void replacePowers(TempTokenList& tempTokens, ParserInfo& info);
    static void replaceTimesDivide(TempTokenList& tempTokens, ParserInfo& info);
    static void replacePlusMinus(TempTokenList& tempTokens, ParserInfo& info);

    std::variant<OPERATOR_TYPE, Empty, CustomFunction, ReservedFunction, Vector, std::string, double, long long> m_token;
    std::vector<AstToken>                                                                                        m_children;
    Range                                                                                                        m_range;
};

#endif // PRS_ASTTOKEN_H
