//
// Created by pc on 18-08-21.
//

#ifndef AST_ERR_CIRCULARDEPENDENCYERROR_H
#define AST_ERR_CIRCULARDEPENDENCYERROR_H

#include "../Declaration.h"

#include <vector>

namespace ast::err {

    class CircularDependencyError {

      public:
        CircularDependencyError(size_t index, std::vector<std::pair<size_t, Declaration>>&& pathToCycle);
        [[nodiscard]] std::string toString() const;

      private:
        size_t                                      m_index;
        std::vector<std::pair<size_t, Declaration>> m_pathToCycle;
    };

} // namespace ast::err

#endif // AST_ERR_CIRCULARDEPENDENCYERROR_H
