//
// Created by pc on 08-06-21.
//

#ifndef PRS_PARSERMESSAGE_H
#define PRS_PARSERMESSAGE_H

#include "../par/Range.h"

#include <string>

namespace ast::err {
    class ParserMessage {

      public:
        enum TYPE { INSERT_MULTIPLICATION };

        ParserMessage(TYPE type, std::string message, par::Range range = {});

        [[nodiscard]] std::string toString() const;

      private:
        TYPE        m_type;
        std::string m_message;
        par::Range       m_range;
    };
} // namespace err

#endif // PRS_PARSERMESSAGE_H
