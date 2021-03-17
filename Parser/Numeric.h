//
// Created by pc on 25-11-20.
//

#ifndef PARSER_NUMERIC_H
#define PARSER_NUMERIC_H

#include <memory>
#include <ostream>

class AstNode;

class Numeric {

    enum class DATA_TYPE { INTEGER, DOUBLE };

  public:
    Numeric(double value);
    Numeric(long long value);
    Numeric(long value);
    Numeric(int value);
    Numeric(const Numeric& other) = default;

    [[nodiscard]] double                   doubleValue() const;
    [[nodiscard]] std::unique_ptr<AstNode> toNode() const;

    Numeric& operator+=(const Numeric& other);
    Numeric& operator-=(const Numeric& other);
    Numeric& operator*=(const Numeric& other);
    Numeric& operator/=(const Numeric& other);
    Numeric& operator^=(const Numeric& other);

    friend std::ostream& operator<<(std::ostream& os, const Numeric& numeric);
    friend long long     gcd(const Numeric& lhs, const Numeric& rhs);
    friend Numeric       operator+(const Numeric& lhs, const Numeric& rhs);
    friend Numeric       operator-(const Numeric& lhs, const Numeric& rhs);
    friend Numeric       operator*(const Numeric& lhs, const Numeric& rhs);
    friend Numeric       operator/(const Numeric& lhs, const Numeric& rhs);
    friend Numeric       operator^(const Numeric& lhs, const Numeric& rhs);

  private:
    DATA_TYPE m_dataType     = DATA_TYPE::INTEGER;
    double    m_doubleValue  = 0.0;
    long long m_integerValue = 0ll;
};

#endif // PARSER_NUMERIC_H
