//
// Created by pc on 17-03-21.
//

#ifndef PARSER_NUMBER_H
#define PARSER_NUMBER_H

#include <string>
#include <variant>

class Number {

  public:
    Number(const Number& other);
    Number(std::variant<long long, double> value);
    Number(const std::string& value);

    [[nodiscard]] Number                                     negate() const;
    [[nodiscard]] const std::variant<long long int, double>& number() const;
    [[nodiscard]] std::string                                toString() const;

    friend Number operator+(const Number& a, const Number& b);
    friend Number operator*(const Number& a, const Number& b);
    friend Number operator/(const Number& a, const Number& b);
    friend bool   operator==(const Number& a, const Number& b);
    friend bool   operator<(const Number& a, const Number& b);

  private:
    std::variant<long long, double> m_number;
};

#endif // PARSER_NUMBER_H
