//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODE_H
#define PARSER_ASTNODE_H

#include "Number.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

class AstNode;
typedef std::unique_ptr<AstNode> u_ptr_AstNode;

class AstNodeMul;
class AstNodePower;

class AstNode {
  public:
    enum class NODE_TYPE { NUMBER, UNARY_MINUS, VARIABLE, ADD, SUBTRACT, MULTIPLY, DIVIDE, POWER, FUNCTION, ERROR };
    explicit AstNode() = default;
    virtual ~AstNode() = default;

    [[nodiscard]] bool   isNumeric() const;
    [[nodiscard]] bool   isCommutative() const;
    [[nodiscard]] bool   isZero() const;
    [[nodiscard]] bool   isOne() const;
    [[nodiscard]] bool   isEven() const;
    [[nodiscard]] Number getNumber() const;

    static u_ptr_AstNode makeZeroNode();
    static u_ptr_AstNode makeOneNode();
    static u_ptr_AstNode makeNumber(std::variant<long long int, double> val);
    static u_ptr_AstNode makeNumber(Number val);
    static u_ptr_AstNode makeError();

    [[nodiscard]] virtual size_t                childCount() const                               = 0;
    [[nodiscard]] virtual NODE_TYPE             type() const                                     = 0;
    [[nodiscard]] virtual std::string           toString() const                                 = 0;
    [[nodiscard]] virtual const AstNode*        childAt(size_t index) const                      = 0;
    [[nodiscard]] virtual u_ptr_AstNode         simplify() const                                 = 0;
    [[nodiscard]] virtual u_ptr_AstNode         copy() const                                     = 0;
    [[nodiscard]] virtual u_ptr_AstNode         differentiate(const std::string& variable) const = 0;
    [[nodiscard]] virtual std::set<std::string> collectVariables() const                         = 0;

    static bool          compare(const AstNode* lhs, const AstNode* rhs);
    static u_ptr_AstNode copy(const u_ptr_AstNode& node);
    static u_ptr_AstNode simplify(const u_ptr_AstNode& node);

    friend bool          operator==(const AstNode& lhs, const AstNode& rhs);
    friend bool          operator<(const AstNode& lhs, const AstNode& rhs);
    friend std::ostream& operator<<(std::ostream& out, const AstNode& node);
    friend u_ptr_AstNode operator+(const AstNode& lhs, const AstNode& rhs);
    friend u_ptr_AstNode operator+(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs);
    friend u_ptr_AstNode operator*(const AstNode& lhs, const AstNode& rhs);
    friend u_ptr_AstNode operator*(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs);
    friend u_ptr_AstNode operator-(const AstNode& lhs, const AstNode& rhs);
    friend u_ptr_AstNode operator-(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs);
    friend u_ptr_AstNode operator/(const AstNode& lhs, const AstNode& rhs);
    friend u_ptr_AstNode operator/(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs);
    friend u_ptr_AstNode operator^(const AstNode& lhs, const AstNode& rhs);
    friend u_ptr_AstNode operator^(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs);
    friend u_ptr_AstNode operator-(const AstNode& val);
    friend u_ptr_AstNode operator-(u_ptr_AstNode&& val);

  protected:
    [[nodiscard]] virtual bool compareEqualType(const AstNode* rhs) const = 0;
    [[nodiscard]] virtual bool equals(const AstNode& other) const         = 0;

    static bool compare_u_ptr(const u_ptr_AstNode& lhs, const u_ptr_AstNode& rhs);
};

#endif // PARSER_ASTNODE_H
