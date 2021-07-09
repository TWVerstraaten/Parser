//
// Created by pc on 17-03-21.
//

#ifndef PARSER_NUMBER_H
#define PARSER_NUMBER_H

#include <string>
#include <variant>

namespace gen {
    class Number {
      public:
        Number(const Number& other);
        explicit Number(std::variant<long long, double> value);
        explicit Number(const std::string& value);
        Number(long long value);
        Number(int value);
        Number(double value);

        [[nodiscard]] double                                     toDouble() const;
        [[nodiscard]] Number                                     negate() const;
        [[nodiscard]] const std::variant<long long int, double>& number() const;
        [[nodiscard]] std::string                                toString() const;
        [[nodiscard]] bool                                       holdsLongLong() const;
        [[nodiscard]] long long                                  toLongLong() const;

        friend bool   operator==(const Number& a, const Number& b);
        friend bool   operator<(const Number& a, const Number& b);
        friend Number operator+(const Number& a, const Number& b);
        friend Number operator-(const Number& a, const Number& b);
        friend Number operator*(const Number& a, const Number& b);
        friend Number operator/(const Number& a, const Number& b);
        friend Number operator^(const Number& a, const Number& b);
        friend Number operator-(const Number& a);

      private:
        std::variant<long long, double> m_number{0ll};
    };
} // namespace gen

#endif // PARSER_NUMBER_H
