//
// Created by pc on 03-06-21.
//

#ifndef PARSER_RESERVEDIDENTIFIERS_H
#define PARSER_RESERVEDIDENTIFIERS_H

#include <map>
#include <string>

namespace fml {
    class ReservedIdentifiers {
      public:
        static bool   isReservedFunctionName(const std::string& name);
        static size_t argumentCountOfReservedFunction(const std::string& name);

      private:
        static const std::map<std::string, size_t> m_reservedFunctions;
    };
} // namespace fml

#endif // PARSER_RESERVEDIDENTIFIERS_H
