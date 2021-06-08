//
// Created by pc on 08-06-21.
//

#include "ReservedFunction.h"

#include "../alg/StringAlg.h"

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
