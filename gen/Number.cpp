//
// Created by pc on 17-03-21.
//

#include "Number.h"

#include "defines.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>

namespace gen {
    template <typename T>
    T READ_FROM_STRING(const std::string& string) {
        std::istringstream stringStream(string);
        T                  val;
        stringStream >> val;
        return val;
    }

    Number::Number(const Number& other) : m_number(other.m_number) {
    }

    Number::Number(std::variant<long long int, double> value) : m_number(value) {
    }

    Number::Number(const std::string& value) {
        if (value.find('.') == std::string::npos) {
            m_number = READ_FROM_STRING<long long>(value);
        } else {
            assert(std::count_if(TT_IT(value), [](char c) { return c == '.'; }) == 1);
            m_number = READ_FROM_STRING<double>(value);
        }
    }

    Number::Number(long long int value) {
        m_number = value;
    }

    Number::Number(double value) {
        m_number = value;
    }

    const std::variant<long long int, double>& Number::number() const {
        return m_number;
    }

    std::string Number::toString() const {
        return std::visit([](const auto& a) { return std::to_string(a); }, m_number);
    }

    Number Number::negate() const {
        return std::visit([](const auto& a) { return Number(-a); }, m_number);
    }

    Number operator+(const Number& a, const Number& b) {
        return std::visit([](const auto& a, const auto& b) { return Number(a + b); }, a.m_number, b.m_number);
    }

    Number operator-(const Number& a, const Number& b) {
        return std::visit([](const auto& a, const auto& b) { return Number(a - b); }, a.m_number, b.m_number);
    }

    Number operator*(const Number& a, const Number& b) {
        return std::visit([](const auto& a, const auto& b) { return Number(a * b); }, a.m_number, b.m_number);
    }

    bool operator==(const Number& a, const Number& b) {
        return std::visit([](const auto& a, const auto& b) { return a == b; }, a.m_number, b.m_number);
    }

    bool operator<(const Number& a, const Number& b) {
        return std::visit([](const auto& a, const auto& b) { return a < b; }, a.m_number, b.m_number);
    }

    Number operator/(const Number& a, const Number& b) {
        if (std::holds_alternative<long long>(a.number()) && std::holds_alternative<long long>(b.number())) {
            const auto num = std::get<long long>(a.number());
            const auto den = std::get<long long>(b.number());
            assert(den != 0ll);
            if (std::abs(num) % std::abs(den) == 0ll) {
                return Number(num / den);
            }
        }
        return std::visit([](const auto& a, const auto& b) { return Number(a / static_cast<double>(b)); }, a.m_number, b.m_number);
    }

    [[nodiscard]] long long POWER(long long base, long long exponent) {
        long long result = 1;
        while (exponent) {
            if (exponent & 1) {
                result = result * base;
            }
            exponent >>= 1;
            base *= base;
        }
        return result;
    }

    Number operator^(const Number& a, const Number& b) {
        if (std::holds_alternative<long long>(a.number()) && std::holds_alternative<long long>(b.number())) {
            long long base     = std::get<long long>(a.number());
            long long exponent = std::get<long long>(b.number());
            if ((exponent < std::numeric_limits<long long>::digits) && (exponent * std::log2(base) - 1 < std::numeric_limits<long long>::digits)) {
                return Number{POWER(base, exponent)};
            }
        }
        return std::visit([](const auto& a, const auto& b) { return Number(std::pow(static_cast<double>(a), static_cast<double>(b))); }, a.m_number, b.m_number);
    }

    Number operator-(const Number& a) {
        return a.negate();
    }

    double Number::toDouble() const {
        return std::visit([](const auto& a) { return static_cast<float>(a); }, m_number);
    }

    bool Number::holdsLongLong() const {
        return std::holds_alternative<long long>(m_number);
    }

    long long Number::toLongLong() const {
        assert(holdsLongLong());
        return std::get<long long>(m_number);
    }

} // namespace gen