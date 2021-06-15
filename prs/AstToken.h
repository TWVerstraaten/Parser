//
// Created by pc on 07-06-21.
//

#ifndef PRS_ASTTOKEN_H
#define PRS_ASTTOKEN_H

#include "CustomFunctionToken.h"
#include "ReservedFunction.h"
#include "StructuralToken.h"
#include "VectorToken.h"

#include <set>
#include <variant>
#include <vector>

class ParserInfo;

class AstToken {

  public:
    struct Empty {};
    enum class OPERATOR_TYPE { PLUS, MINUS, TIMES, DIVIDE, POWER, UNARY_MINUS, EQUALS };

    typedef std::variant<OPERATOR_TYPE, Empty, CustomFunctionToken, ReservedFunction, VectorToken, std::string, double, long long int> AstTokenVariant;

    explicit AstToken(const std::list<StructuralToken>& structuralTokens, ParserInfo& info);
    AstToken(const StructuralToken::Bracketed& bracketed, Range range, ParserInfo& info);
    AstToken(const StructuralToken::Function& function, Range range, ParserInfo& info);
    AstToken(const std::string& string, Range range, ParserInfo& info);
    AstToken(long long value, Range range, ParserInfo& info);
    AstToken(double value, Range range, ParserInfo& info);
    AstToken(OPERATOR_TYPE type, AstToken astToken, Range range, ParserInfo& info);
    AstToken(OPERATOR_TYPE type, AstToken left, AstToken right, Range range, ParserInfo& info);

    [[nodiscard]] std::set<CustomFunctionToken> dependsOn() const;
    [[nodiscard]] std::set<std::string>         variablesUsed() const;
    [[nodiscard]] const AstTokenVariant&        token() const;
    [[nodiscard]] const std::vector<AstToken>&  children() const;
    [[nodiscard]] const Range&                  range() const;

    [[nodiscard]] static std::string toStringAsTree(const AstToken& root);

  private:
    void maybeCastToReservedFunction(ParserInfo& info);

    typedef std::variant<AstToken, Token> TempToken;
    typedef std::list<TempToken>          TempTokenList;

    static void replaceUnaryMinuses(TempTokenList& tempTokens, ParserInfo& info);
    static void replacePowers(TempTokenList& tempTokens, ParserInfo& info);
    static void replaceTimesDivide(TempTokenList& tempTokens, ParserInfo& info);
    static void replacePlusMinus(TempTokenList& tempTokens, ParserInfo& info);

    AstTokenVariant       m_token;
    std::vector<AstToken> m_children;
    Range                 m_range;
};

#endif // PRS_ASTTOKEN_H
