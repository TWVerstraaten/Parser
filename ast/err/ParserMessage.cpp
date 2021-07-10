//
// Created by pc on 08-06-21.
//

#include "ParserMessage.h"

#include <cassert>

namespace ast::err {
    ParserMessage::ParserMessage(TYPE type, std::string message, par::Range range) : m_type(type), m_message(std::move(message)), m_range(range) {
    }

    std::string ParserMessage::toString() const {
        switch (m_type) {
            case INSERT_MULTIPLICATION:
                return "Inserting '*' at " + std::to_string(m_range.startIndex());
        }
        assert(false);
        return "";
    }
} // namespace err