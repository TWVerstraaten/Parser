//
// Created by pc on 17-03-21.
//

#include "Number.h"

#include <cassert>

Number::Number(std::variant<long long int, double> value) : m_number(value) {
}

std::string Number::toString() const {
    return std::visit([](const auto& a) { return std::to_string(a); }, m_number);
}

const std::variant<long long int, double>& Number::number() const {
    return m_number;
}

Number::Number(const std::string& value) {
    if (value.find('.') == std::string::npos) {
        m_number = stoll(value);
    } else {
        m_number = stod(value);
    }
}

Number Number::negate() const {
    return std::visit([](const auto& a) { return Number(-a); }, m_number);
}

Number operator+(const Number& a, const Number& b) {
    return std::visit([](const auto& a, const auto& b) { return Number(a + b); }, a.m_number, b.m_number);
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
