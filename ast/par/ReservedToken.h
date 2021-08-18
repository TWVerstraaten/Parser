//
// Created by pc on 08-06-21.
//

#ifndef AST_PAR_RESERVEDTOKEN_H
#define AST_PAR_RESERVEDTOKEN_H

#include <optional>
#include <string>
#include <variant>

namespace ast::par {

    struct Sin {
        static const size_t       ARGUMENT_COUNT = 1;
        inline static const char* NAME           = "sin";
    };
    struct Cos {
        static const size_t       ARGUMENT_COUNT = 1;
        inline static const char* NAME           = "cos";
    };
    struct Tan {
        static const size_t       ARGUMENT_COUNT = 1;
        inline static const char* NAME           = "tan";
    };
    struct ASin {
        static const size_t       ARGUMENT_COUNT = 1;
        inline static const char* NAME           = "asin";
    };
    struct ACos {
        static const size_t       ARGUMENT_COUNT = 1;
        inline static const char* NAME           = "acos";
    };
    struct ATan {
        static const size_t       ARGUMENT_COUNT = 1;
        inline static const char* NAME           = "atan";
    };
    struct ATan2 {
        static const size_t       ARGUMENT_COUNT = 2;
        inline static const char* NAME           = "atan2";
    };

    typedef std::variant<Sin, Cos, Tan, ASin, ACos, ATan, ATan2> ReservedToken;

    [[nodiscard]] size_t                       GET_ARGUMENT_COUNT(const ReservedToken& reserved);
    [[nodiscard]] std::string                  GET_NAME(const ReservedToken& reserved);
    [[nodiscard]] std::optional<ReservedToken> GET_RESERVED(const std::string& string);

    [[nodiscard]] double EVAL(const ReservedToken& reserved, double argument);
    [[nodiscard]] double EVAL(const ReservedToken& reserved, double first, double second);
} // namespace ast::par

#endif // AST_PAR_RESERVEDTOKEN_H
