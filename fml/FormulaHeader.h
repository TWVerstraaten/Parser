//
// Created by pc on 09-04-21.
//

#ifndef PARSER_FORMULAHEADER_H
#define PARSER_FORMULAHEADER_H

#include "../gen/ErrorBase.h"

#include <string>
#include <vector>

namespace fml {
    class FormulaHeader : public gen::ErrorBase {

      public:
        explicit FormulaHeader(std::string headerString);

        [[nodiscard]] const std::string&              name() const;
        [[nodiscard]] const std::string&              headerString() const;
        [[nodiscard]] const std::vector<std::string>& variables() const;

      private:
        bool containsIllegalCharacters();
        void parseArguments(const std::string& argumentString);

        std::string              m_name;
        std::vector<std::string> m_variables;
        std::string              m_headerString;
    };
} // namespace fml

#endif // PARSER_FORMULAHEADER_H
