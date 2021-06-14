//
// Created by pc on 08-06-21.
//

#include "ReservedFunction.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"

#include <cassert>
#include <cmath>

ReservedFunction::ReservedFunction(ReservedFunction::Reserved reserved) : m_reserved(reserved) {
}

std::optional<ReservedFunction::Reserved> ReservedFunction::getReserved(const std::string& string) {
    const auto trimmed = alg::StringAlg::trim(string);
    for (const auto& type : s_allReserved) {
        if (trimmed == getName(type)) {
            return type;
        }
    }
    return {};
}

size_t ReservedFunction::getArgumentCount(const ReservedFunction::Reserved& reserved) {
    return std::visit([](const auto& a) { return std::remove_reference_t<decltype(a)>::s_argumentCount; }, reserved);
}

std::string ReservedFunction::getName(const ReservedFunction::Reserved& reserved) {
    return std::visit([](const auto& a) { return std::string(std::remove_reference_t<decltype(a)>::s_name); }, reserved);
}

double ReservedFunction::eval(const ReservedFunction::Reserved& reserved, double argument) {
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

double ReservedFunction::eval(const ReservedFunction::Reserved& reserved, double first, double second) {
    return std::visit(Overloaded{
                          [first, second](const ATan2&) { return std::atan2(first, second); },
                          [](const auto&) {
                              assert(false);
                              return std::numeric_limits<double>::quiet_NaN();
                          },
                      },
                      reserved);
}
