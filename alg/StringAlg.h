//
// Created by pc on 28-05-21.
//

#ifndef ALG_STRINGALG_H
#define ALG_STRINGALG_H

#include <functional>
#include <sstream>
#include <string>

namespace alg {
    class StringAlg {
      public:
        template <class Type = std::string, class Container>
        static std::string S_CONCATENATE_STRINGS(
            const Container& container, const std::function<std::string(const Type&)> f = [](const Type& a) { return a; }) {
            std::ostringstream ss;
            bool               firstPass = true;
            for (const auto& el : container) {
                ss << (firstPass ? "" : ", ") + f(el);
                firstPass = false;
            }
            return ss.str();
        }

        static std::vector<std::string> S_TRIM_AND_SPLIT(std::string string, const std::string& splitCharacters);
        static std::string              S_TRIM(std::string string);
    };
} // namespace alg

#endif // ALG_STRINGALG_H
