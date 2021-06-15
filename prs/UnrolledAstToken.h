//
// Created by pc on 14-06-21.
//

#ifndef PRS_UNROLLEDASTTOKEN_H
#define PRS_UNROLLEDASTTOKEN_H

#include "../gen/Number.h"
#include "ReservedFunction.h"
#include "VectorToken.h"

#include <map>
#include <variant>
#include <vector>

class AstToken;

class UnrolledAstToken {

  public:
    struct Plus {};
    struct Minus {};
    struct Times {};
    struct Divide {};
    struct Power {};
    struct UnaryMinus {};

    typedef std::variant<Plus, Minus, Times, Divide, Power, UnaryMinus, ReservedFunction, VectorToken, std::string, double, long long> UnrolledToken;

    explicit UnrolledAstToken(const AstToken& astToken);

    void simplify();

    [[nodiscard]] UnrolledAstToken setVariable(const std::string& variable, const gen::Number& number) const;

    [[nodiscard]] bool                                 isNumeric() const;
    [[nodiscard]] double                               toDouble() const;
    [[nodiscard]] const UnrolledToken&                 token() const;
    [[nodiscard]] const std::vector<UnrolledAstToken>& children() const;
    [[nodiscard]] gen::Number                          toNumber() const;
    [[nodiscard]] std::string                          toString() const;

    [[nodiscard]] static UnrolledToken fromNumber(const gen::Number& number);

  private:
    void simplifyFunction();
    void unWrap1DVectors();
    void setVariableInternal(const std::string& variable, const gen::Number& number);

    UnrolledToken                 m_token;
    std::vector<UnrolledAstToken> m_children;
};

#endif // PRS_UNROLLEDASTTOKEN_H