//
// Created by pc on 09-07-21.
//

#include "UnrollCompanion.h"

#include "../ast/err/DefinitionError.h"
#include "../ast/err/ParserError.h"
#include "../ast/err/ParserMessage.h"
#include "../ast/err/ParserWarning.h"

#include <sstream>

namespace ast {

    UnrollCompanion::UnrollCompanion(size_t index, Ast&& ast) : m_index(index), m_originalAst(std::move(ast)), m_partiallyUnrolledAst(m_originalAst) {
    }

    size_t UnrollCompanion::index() const {
        return m_index;
    }

    const Ast& UnrollCompanion::originalAst() const {
        return m_originalAst;
    }

    UnrollCompanion::STATUS UnrollCompanion::status() const {
        return m_status;
    }

    const std::unique_ptr<UnrolledAst>& UnrollCompanion::unrolledAst() const {
        return m_unrolledAst;
    }

    const std::string& UnrollCompanion::errorMessage() const {
        return m_errorMessage;
    }

    std::string UnrollCompanion::statusString() const {
        std::stringstream ss;
        switch (m_status) {
            case STATUS::UN_CHECKED:
                ss << "Unchecked unroll status\t";
                break;
            case STATUS::READY_TO_UNROLL:
                ss << "Ready to unroll\t";
                break;
            case STATUS::CANNOT_BE_UNROLLED:
                ss << "Cannot be unrolled\t";
                break;
        }
        ss << m_errorMessage;
        return ss.str();
    }

    void UnrollCompanion::setStatus(UnrollCompanion::STATUS status) {
        m_status = status;
    }

    const Ast& UnrollCompanion::partiallyUnrolledAst() const {
        return m_partiallyUnrolledAst;
    }
} // namespace ast