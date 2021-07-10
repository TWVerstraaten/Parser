//
// Created by pc on 07-06-21.
//

#ifndef PRS_PARSERWARNING_H
#define PRS_PARSERWARNING_H

#include "../par/Range.h"

#include <string>

namespace ast::err {
    class ParserWarning {

      public:
        enum TYPE { SUSPICIOUS_IDENTIFIER_NUM_PATTERN };

        ParserWarning(TYPE type, std::string message, par::Range range = {});

        [[nodiscard]] std::string       toString() const;
        [[nodiscard]] const par::Range& range() const;

      private:
        TYPE        m_type;
        std::string m_message;
        par::Range  m_range;
    };
} // namespace ast::err

#endif // PRS_PARSERWARNING_H
