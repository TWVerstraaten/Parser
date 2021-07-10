//
// Created by pc on 09-07-21.
//

#ifndef PRS_UNROLLCOMPANION_H
#define PRS_UNROLLCOMPANION_H

#include "Ast.h"
#include "UnrolledAst.h"

#include <memory>

namespace ast {

    class UnrollCompanion {
        enum class STATUS { UN_CHECKED, READY_TO_UNROLL, CANNOT_BE_UNROLLED };

      public:
        UnrollCompanion(size_t index, Ast&& ast);

        void setStatus(STATUS status);

        [[nodiscard]] size_t                              index() const;
        [[nodiscard]] const Ast&                          originalAst() const;
        [[nodiscard]] STATUS                              status() const;
        [[nodiscard]] const std::unique_ptr<UnrolledAst>& unrolledAst() const;
        [[nodiscard]] const Ast&                          partiallyUnrolledAst() const;
        [[nodiscard]] const std::string&                  errorMessage() const;
        [[nodiscard]] std::string                         statusString() const;

      private:
        size_t                       m_index;
        Ast                          m_originalAst;
        Ast                          m_partiallyUnrolledAst;
        STATUS                       m_status = STATUS::UN_CHECKED;
        std::unique_ptr<UnrolledAst> m_unrolledAst;
        std::string                  m_errorMessage;
    };
} // namespace ast

#endif // PRS_UNROLLCOMPANION_H
