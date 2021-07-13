//
// Created by pc on 10-07-21.
//

#ifndef AST_ERR_DEFINITIONERROR_H
#define AST_ERR_DEFINITIONERROR_H

#include <string>

namespace ast::err {

    class DefinitionError {

      public:
        enum class TYPE { REDECLARATION, UNDEFINED_FUNCTIONS };

        DefinitionError(TYPE type, std::string message = "");

        [[nodiscard]] std::string toString() const;

      private:
        TYPE        m_type;
        std::string m_message;
    };

} // namespace ast::err

#endif // AST_ERR_DEFINITIONERROR_H
