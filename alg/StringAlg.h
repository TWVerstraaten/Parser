//
// Created by pc on 28-05-21.
//

#ifndef ALG_STRINGALG_H
#define ALG_STRINGALG_H

#include <functional>
#include <sstream>
#include <string>

namespace alg::str {
    template <class Type = std::string, class Container>
    std::string CONCATENATE_STRINGS(
        const Container& container, const std::function<std::string(const Type&)> f = [](const Type& a) { return a; }) {
        std::ostringstream ss;
        bool               firstPass = true;
        for (const auto& el : container) {
            ss << (firstPass ? "" : ", ") + f(el);
            firstPass = false;
        }
        return ss.str();
    }

    std::vector<std::string> TRIM_AND_SPLIT(std::string string, const std::string& splitCharacters);
    std::string              TRIM(std::string string);
} // namespace alg::str

#endif // ALG_STRINGALG_H
