//
// Created by pc on 02-06-21.
//

#ifndef PARSER_FUNCTIONSIGNATURE_H
#define PARSER_FUNCTIONSIGNATURE_H

#include <string>

namespace ast {

    class FunctionSignature {

      public:
        FunctionSignature(std::string functionName, size_t argumentCount);

        [[nodiscard]] const std::string& functionName() const;
        [[nodiscard]] size_t             argumentCount() const;

      private:
        const std::string m_functionName;
        const size_t      m_argumentCount;
    };

} // namespace ast

#endif // PARSER_FUNCTIONSIGNATURE_H
