//
// Created by pc on 08-06-21.
//

#include "ReservedToken.h"

#include "../../alg/StringAlg.h"
#include "../../gen/Overloaded.h"
#include "../../gen/defines.h"

#include <cassert>
#include <cmath>

namespace ast::par {

    static const std::vector<ReservedToken> ALL_RESERVED = {Sin(), Cos(), Tan(), ASin(), ACos(), ATan(), ATan2()};

    size_t GET_ARGUMENT_COUNT(const ReservedToken& reserved) {
        return std::visit([](const auto& a) { return std::remove_reference_t<decltype(a)>::ARGUMENT_COUNT; }, reserved);
    }

    std::optional<ReservedToken> GET_RESERVED(const std::string& string) {
        const auto trimmed = alg::str::TRIM(string);
        if (auto it = std::find_if(TT_IT(ALL_RESERVED), TT_LAMBDA_REF(a, return GET_NAME(a) == trimmed;)); it != ALL_RESERVED.end()) {
            return *it;
        } else {
            return {};
        }
    }

    std::string GET_NAME(const ReservedToken& reserved) {
        return std::visit([](const auto& a) { return std::string(std::remove_reference_t<decltype(a)>::NAME); }, reserved);
    }

    double EVAL(const ReservedToken& reserved, double argument) {
        return std::visit(Overloaded{
                              [argument](const Sin&) { return std::sin(argument); },
                              [argument](const Cos&) { return std::cos(argument); },
                              [argument](const Tan&) { return std::tan(argument); },
                              [argument](const ASin&) { return std::asin(argument); },
                              [argument](const ACos&) { return std::acos(argument); },
                              [argument](const ATan&) { return std::atan(argument); },
                              [](const auto&) {
                                  assert(false);
                                  return std::numeric_limits<double>::quiet_NaN();
                              },
                          },
                          reserved);
    }

    double EVAL(const ReservedToken& reserved, double first, double second) {
        return std::visit(Overloaded{
                              [first, second](const ATan2&) { return std::atan2(first, second); },
                              [](const auto&) {
                                  assert(false);
                                  return std::numeric_limits<double>::quiet_NaN();
                              },
                          },
                          reserved);
    }
} // namespace ast::par