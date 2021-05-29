//
// Created by pc on 20-11-20.
//

#ifndef PARSER_ASTNODEFUNCTION_H
#define PARSER_ASTNODEFUNCTION_H

#include "AstNode.h"

#include <vector>

namespace ast {
    class AstNodeFunction : public AstNode {
      public:
        AstNodeFunction(std::string functionName, std::vector<u_ptr_AstNode>&& arguments);

        [[nodiscard]] bool                  compareEqualType(const AstNode* rhs) const override;
        [[nodiscard]] size_t                childCount() const override;
        [[nodiscard]] NODE_TYPE             type() const override;
        [[nodiscard]] const AstNode*        childAt(size_t index) const override;
        [[nodiscard]] u_ptr_AstNode         copy() const override;
        [[nodiscard]] u_ptr_AstNode         simplify() const override;
        [[nodiscard]] u_ptr_AstNode         differentiate(const std::string& variable) const override;
        [[nodiscard]] gen::Number           eval(const std::map<std::string, gen::Number>& arguments) const override;
        [[nodiscard]] std::string           toString() const override;
        [[nodiscard]] std::set<std::string> collectVariables() const override;

      protected:
        [[nodiscard]] bool equals(const AstNode& other) const override;

      private:
        std::string                m_functionName;
        std::vector<u_ptr_AstNode> m_arguments;
    };
} // namespace ast

#endif // PARSER_ASTNODEFUNCTION_H
