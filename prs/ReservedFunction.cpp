//
// Created by pc on 08-06-21.
//

#include "ReservedFunction.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"
#include "../gen/defines.h"

#include <cassert>
#include <cmath>

namespace rsrvd {

    std::optional<rsrvd::Reserved> S_GET_RESERVED(const std::string& string) {
        static const std::vector<rsrvd::Reserved> S_ALL_RESERVED = {rsrvd::Sin(), rsrvd::Cos(), rsrvd::Tan(), rsrvd::ASin(), rsrvd::ACos(), rsrvd::ATan(), rsrvd::ATan2()};
        const auto                                trimmed        = alg::StringAlg::S_TRIM(string);
        if (auto it = std::find_if(TT_IT(S_ALL_RESERVED), TT_LAMBDA_REF(a, return S_GET_NAME(a) == trimmed;)); it != S_ALL_RESERVED.end()) {
            return *it;
        } else {
            return {};
        }
    }

    size_t S_GET_ARGUMENT_COUNT(const rsrvd::Reserved& reserved) {
        return std::visit([](const auto& a) { return std::remove_reference_t<decltype(a)>::s_argumentCount; }, reserved);
    }

    std::string S_GET_NAME(const rsrvd::Reserved& reserved) {
        return std::visit([](const auto& a) { return std::string(std::remove_reference_t<decltype(a)>::s_name); }, reserved);
    }

    double S_EVAL(const rsrvd::Reserved& reserved, double argument) {
        return std::visit(Overloaded{
                              [argument](const rsrvd::Sin&) { return std::sin(argument); },
                              [argument](const rsrvd::Cos&) { return std::cos(argument); },
                              [argument](const rsrvd::Tan&) { return std::tan(argument); },
                              [argument](const rsrvd::ASin&) { return std::asin(argument); },
                              [argument](const rsrvd::ACos&) { return std::acos(argument); },
                              [argument](const rsrvd::ATan&) { return std::atan(argument); },
                              [](const auto&) {
                                  assert(false);
                                  return std::numeric_limits<double>::quiet_NaN();
                              },
                          },
                          reserved);
    }

    double S_EVAL(const rsrvd::Reserved& reserved, double first, double second) {
        return std::visit(Overloaded{
                              [first, second](const rsrvd::ATan2&) { return std::atan2(first, second); },
                              [](const auto&) {
                                  assert(false);
                                  return std::numeric_limits<double>::quiet_NaN();
                              },
                          },
                          reserved);
    }
} // namespace rsrvd