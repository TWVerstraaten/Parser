//
// Created by pc on 08-06-21.
//

#ifndef PRS_RESERVEDFUNCTION_H
#define PRS_RESERVEDFUNCTION_H

#include <cmath>
#include <optional>
#include <string>
#include <variant>

namespace rsrvd {
    struct Sin {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "sin";
        inline static const auto  m_func          = [](const auto& a) { return std::sin(a); };
    };
    struct Cos {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "cos";
        inline static const auto  m_func          = [](const auto& a) { return std::cos(a); };
    };
    struct Tan {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "tan";
        inline static const auto  m_func          = [](const auto& a) { return std::tan(a); };
    };
    struct ASin {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "asin";
        inline static const auto  m_func          = [](const auto& a) { return std::asin(a); };
    };
    struct ACos {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "acos";
        inline static const auto  m_func          = [](const auto& a) { return std::acos(a); };
    };
    struct ATan {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "atan";
        inline static const auto  m_func          = [](const auto& a) { return std::atan(a); };
    };
    struct ATan2 {
        static const size_t       s_argumentCount = 2;
        inline static const char* s_name          = "atan2";
        inline static const auto  m_func          = [](const auto& a, const auto& b) { return std::atan2(a, b); };
    };

    typedef std::variant<Sin, Cos, Tan, ASin, ACos, ATan, ATan2> Reserved;

    [[nodiscard]] size_t                  S_GET_ARGUMENT_COUNT(const Reserved& reserved);
    [[nodiscard]] std::string             S_GET_NAME(const Reserved& reserved);
    [[nodiscard]] std::optional<Reserved> S_GET_RESERVED(const std::string& string);

    [[nodiscard]] double S_EVAL(const Reserved& reserved, double argument);
    [[nodiscard]] double S_EVAL(const Reserved& reserved, double first, double second);
} // namespace rsrvd

#endif // PRS_RESERVEDFUNCTION_H
