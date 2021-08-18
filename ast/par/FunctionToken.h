//
// Created by pc on 08-06-21.
//

#ifndef AST_PAR_FUNCTIONTOKEN_H
#define AST_PAR_FUNCTIONTOKEN_H

#include <string>

namespace ast::par {
    class FunctionToken {
        friend bool operator<(const FunctionToken& left, const FunctionToken& right);

      public:
        FunctionToken(std::string name, size_t argumentCount);

        void                             setArgumentCount(size_t argumentCount);
        [[nodiscard]] const std::string& name() const;
        [[nodiscard]] size_t             argumentCount() const;
        [[nodiscard]] std::string        toString() const;

        friend bool operator==(const FunctionToken& r, const FunctionToken& l);

      private:
        std::string m_name;
        size_t      m_argumentCount;
    };
} // namespace ast::par

#endif // AST_PAR_FUNCTIONTOKEN_H
