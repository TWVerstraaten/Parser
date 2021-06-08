//
// Created by pc on 08-06-21.
//

#ifndef PRS_RESERVEDFUNCTION_H
#define PRS_RESERVEDFUNCTION_H

#include <optional>
#include <string>
#include <variant>

class ReservedFunction {

    friend class AstToken;

  private:
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

    typedef std::variant<Sin, Cos, Tan, ASin, ACos, ATan, ATan2> Reserved;

    inline static Reserved s_allReserved[] = {Sin(), Cos(), Tan(), ASin(), ACos(), ATan(), ATan2()};
    explicit ReservedFunction(Reserved reserved);

    [[nodiscard]] static size_t                  getArgumentCount(const Reserved& reserved);
    [[nodiscard]] static std::string             getName(const Reserved& reserved);
    [[nodiscard]] static std::optional<Reserved> getReserved(const std::string& string);

    Reserved m_reserved;
};

#endif // PRS_RESERVEDFUNCTION_H
