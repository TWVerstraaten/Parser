//
// Created by pc on 20-11-20.
//

#ifndef PARSER_ASTNODEFUNCTION_H
#define PARSER_ASTNODEFUNCTION_H

#include "AstNode.h"

namespace ast {
    class AstNodeFunction : public AstNode {
      public:
        AstNodeFunction(std::string functionName, u_ptr_AstNode&& argument);

        [[nodiscard]] std::string           toString() const override;
        [[nodiscard]] u_ptr_AstNode         copy() const override;
        [[nodiscard]] u_ptr_AstNode         simplify() const override;
        [[nodiscard]] NODE_TYPE             type() const override;
        [[nodiscard]] size_t                childCount() const override;
        [[nodiscard]] bool                  compareEqualType(const AstNode* rhs) const override;
        [[nodiscard]] const AstNode*        childAt(size_t index) const override;
        [[nodiscard]] u_ptr_AstNode         differentiate(const std::string& variable) const override;
        [[nodiscard]] std::set<std::string> collectVariables() const override;
        [[nodiscard]] gen::Number           eval(const std::map<std::string, gen::Number>& arguments) const override;

      protected:
        [[nodiscard]] bool equals(const AstNode& other) const override;

      private:
        std::string   m_functionName;
        u_ptr_AstNode m_argument;
    };
} // namespace ast

#endif // PARSER_ASTNODEFUNCTION_H
