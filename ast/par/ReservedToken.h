//
// Created by pc on 08-06-21.
//

#ifndef PRS_RESERVEDFUNCTION_H
#define PRS_RESERVEDFUNCTION_H

#include <optional>
#include <string>
#include <variant>

namespace ast::par {

    struct Sin {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "sin";
    };
    struct Cos {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "cos";
    };
    struct Tan {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "tan";
    };
    struct ASin {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "asin";
    };
    struct ACos {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "acos";
    };
    struct ATan {
        static const size_t       s_argumentCount = 1;
        inline static const char* s_name          = "atan";
    };
    struct ATan2 {
        static const size_t       s_argumentCount = 2;
        inline static const char* s_name          = "atan2";
    };

    typedef std::variant<Sin, Cos, Tan, ASin, ACos, ATan, ATan2> ReservedToken;

    [[nodiscard]] size_t                       S_GET_ARGUMENT_COUNT(const ReservedToken& reserved);
    [[nodiscard]] std::string                  S_GET_NAME(const ReservedToken& reserved);
    [[nodiscard]] std::optional<ReservedToken> S_GET_RESERVED(const std::string& string);

    [[nodiscard]] double S_EVAL(const ReservedToken& reserved, double argument);
    [[nodiscard]] double S_EVAL(const ReservedToken& reserved, double first, double second);
} // namespace ast::par

#endif // PRS_RESERVEDFUNCTION_H
