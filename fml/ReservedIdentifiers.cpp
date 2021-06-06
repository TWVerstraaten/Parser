//
// Created by pc on 03-06-21.
//

#include "ReservedIdentifiers.h"

#include <cassert>

namespace fml {

    const std::map<std::string, size_t> ReservedIdentifiers::m_reservedFunctions{{"sin", 1},  {"cos", 1},  {"tan", 1},  {"exp", 1},
                                                                                 {"asin", 1}, {"acos", 1}, {"atan", 1}, {"atan2", 2}};

    bool ReservedIdentifiers::isReservedFunctionName(const std::string& name) {
        return m_reservedFunctions.find(name) != m_reservedFunctions.end();
    }

    size_t ReservedIdentifiers::argumentCountOfReservedFunction(const std::string& name) {
        assert(isReservedFunctionName(name));
        return m_reservedFunctions.at(name);
    }

} // namespace fml
