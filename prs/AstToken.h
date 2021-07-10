//
// Created by pc on 07-06-21.
//

#ifndef PRS_ASTTOKEN_H
#define PRS_ASTTOKEN_H

#include "CustomFunctionToken.h"
#include "Header.h"
#include "ReservedFunction.h"
#include "StructuralToken.h"
#include "VectorToken.h"

#include <map>
#include <memory>
#include <set>
#include <variant>
#include <vector>

namespace err {
    class ParserInfo;
}

class AstToken {

  public:
    struct Empty {};
    struct Error {};
    enum class OPERATOR_TYPE { PLUS, MINUS, TIMES, DIVIDE, POWER, UNARY_MINUS, EQUALS };

    typedef std::variant<Error, OPERATOR_TYPE, Empty, CustomFunctionToken, rsrvd::Reserved, VectorToken, std::string, double, long long int> AstTokenVariant;
    typedef std::variant<AstToken, Token>                                                                                                    TempToken;
    typedef std::list<TempToken>                                                                                                             TempTokenList;

    AstToken() = default;
    explicit AstToken(const std::list<StructuralToken>& structuralTokens, err::ParserInfo& info);
    AstToken(const StructuralToken::Bracketed& bracketed, Range range, err::ParserInfo& info);
    AstToken(const StructuralToken::Function& function, Range range, err::ParserInfo& info);
    AstToken(const std::string& string, Range range, err::ParserInfo& info);
    AstToken(long long value, Range range, err::ParserInfo& info);
    AstToken(double value, Range range, err::ParserInfo& info);
    AstToken(OPERATOR_TYPE type, AstToken astToken, Range range, err::ParserInfo& info);
    AstToken(OPERATOR_TYPE type, AstToken left, AstToken right, Range range, err::ParserInfo& info);

    void replaceVariable(const std::string& variable, const AstToken& token);
    void replaceVariables(const std::map<std::string, AstToken>& variableMap);
    void replaceVariables(const std::vector<std::string>& variables, const std::vector<AstToken>& tokens);
    void replaceFunction(const Header::FullHeader& header, const AstToken& functionToken);

    [[nodiscard]] std::set<CustomFunctionToken> getCustomFunctionDependencies() const;
    [[nodiscard]] std::set<std::string>         getUndeclaredVariables(const std::set<std::string>& declared) const;
    [[nodiscard]] std::set<std::string>         variablesUsed() const;
    [[nodiscard]] const AstTokenVariant&        token() const;
    [[nodiscard]] const std::vector<AstToken>&  children() const;
    [[nodiscard]] const Range&                  range() const;
    [[nodiscard]] std::string                   toStringAsTree() const;
    [[nodiscard]] std::string                   toStringFlat() const;

  private:
    void maybeCastToReservedFunction(err::ParserInfo& info);

    AstTokenVariant       m_token = {Error{}};
    std::vector<AstToken> m_children;
    Range                 m_range;
};

#endif // PRS_ASTTOKEN_H
