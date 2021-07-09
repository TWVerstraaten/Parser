//
// Created by pc on 02-06-21.
//

#ifndef PARSER_FUNCTIONSIGNATURE_H
#define PARSER_FUNCTIONSIGNATURE_H

#include <string>

namespace fml {

    class FunctionSignature {

      public:
        FunctionSignature(std::string functionName, size_t argumentCount);

        [[nodiscard]] const std::string& functionName() const;
        [[nodiscard]] size_t             argumentCount() const;

        bool operator<(const FunctionSignature& rhs) const;

      private:
        std::string m_functionName;
        size_t      m_argumentCount;
    };

} // namespace originalAst

#endif // PARSER_FUNCTIONSIGNATURE_H
