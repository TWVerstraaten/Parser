//
// Created by pc on 14-06-21.
//

#ifndef PRS_UNROLLEDASTTOKEN_H
#define PRS_UNROLLEDASTTOKEN_H

#include "../gen/Number.h"
#include "ReservedFunction.h"
#include "VectorToken.h"

#include <variant>
#include <vector>

class AstToken;

class UnrolledAstToken {

    struct Plus {};
    struct Minus {};
    struct Times {};
    struct Divide {};
    struct Power {};
    struct UnaryMinus {};

    typedef std::variant<Plus, Minus, Times, Divide, Power, UnaryMinus, ReservedFunction, VectorToken, std::string, double, long long> UnrolledToken;

  public:
    explicit UnrolledAstToken(const AstToken& astToken);

    void simplify();

    [[nodiscard]] std::string          toString() const;
    [[nodiscard]] bool                 isNumeric() const;
    [[nodiscard]] bool                 is1DNumericVector() const;
    [[nodiscard]] double               toDouble() const;
    [[nodiscard]] gen::Number          toNumber() const;
    [[nodiscard]] static UnrolledToken fromNumber(const gen::Number& number);

  private:
    [[nodiscard]] std::string commaSeparatedChildren() const;

    void simplifyFunction();
    void unWrap1DVectors();

    UnrolledToken                 m_token;
    std::vector<UnrolledAstToken> m_children;
};

#endif // PRS_UNROLLEDASTTOKEN_H
