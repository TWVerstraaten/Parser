//
// Created by pc on 25-11-20.
//

#include "Numeric.h"

#include "AstNode/AstNodeDouble.h"
#include "AstNode/AstNodeInteger.h"

#include <iostream>

Numeric::Numeric(double value) : m_dataType(DATA_TYPE::DOUBLE), m_doubleValue(value) {
}

Numeric::Numeric(long long int value) : m_integerValue(value) {
}

Numeric::Numeric(long int value) : m_integerValue(value) {
}

Numeric::Numeric(int value) : m_integerValue(value) {
}

std::unique_ptr<AstNode> Numeric::toNode() const {
    if (m_dataType == DATA_TYPE::INTEGER) {
        return std::unique_ptr<AstNode>(new AstNodeInteger(m_integerValue));
    } else {
        return std::unique_ptr<AstNode>(new AstNodeDouble(m_doubleValue));
    }
}

Numeric& Numeric::operator+=(const Numeric& other) {
    if (m_dataType == DATA_TYPE::INTEGER && other.m_dataType == DATA_TYPE::INTEGER) {
        m_integerValue += other.m_integerValue;
    } else {
        m_doubleValue = doubleValue() + other.doubleValue();
        m_dataType    = DATA_TYPE::DOUBLE;
    }
    return *this;
}

Numeric& Numeric::operator-=(const Numeric& other) {
    if (m_dataType == DATA_TYPE::INTEGER && other.m_dataType == DATA_TYPE::INTEGER) {
        m_integerValue -= other.m_integerValue;
    } else {
        m_doubleValue = doubleValue() - other.doubleValue();
        m_dataType    = DATA_TYPE::DOUBLE;
    }
    return *this;
}

Numeric& Numeric::operator*=(const Numeric& other) {
    if (m_dataType == DATA_TYPE::INTEGER && other.m_dataType == DATA_TYPE::INTEGER) {
        m_integerValue *= other.m_integerValue;
    } else {
        m_doubleValue = doubleValue() * other.doubleValue();
        m_dataType    = DATA_TYPE::DOUBLE;
    }
    return *this;
}

Numeric& Numeric::operator/=(const Numeric& other) {
    if (m_dataType == DATA_TYPE::INTEGER && other.m_dataType == DATA_TYPE::INTEGER) {
        m_integerValue /= other.m_integerValue;
    } else {
        m_doubleValue = doubleValue() / other.doubleValue();
        m_dataType    = DATA_TYPE::DOUBLE;
    }
    return *this;
}

double Numeric::doubleValue() const {
    return (m_dataType == DATA_TYPE::DOUBLE) ? m_doubleValue : static_cast<double>(m_integerValue);
}

std::ostream& operator<<(std::ostream& os, const Numeric& numeric) {
    if (numeric.m_dataType == Numeric::DATA_TYPE::INTEGER) {
        os << numeric.m_integerValue;
    } else {
        os << numeric.m_doubleValue;
    }
    return os;
}

Numeric operator+(const Numeric& lhs, const Numeric& rhs) {
    Numeric result{lhs};
    result += rhs;
    return result;
}

Numeric operator*(const Numeric& lhs, const Numeric& rhs) {
    Numeric result{lhs};
    result *= rhs;
    return result;
}
