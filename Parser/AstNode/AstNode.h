//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODE_H
#define PARSER_ASTNODE_H

#include <iostream>
#include <memory>
#include <string>

class AstNode;
typedef std::unique_ptr<AstNode>       u_ptr_AstNode;
typedef std::unique_ptr<const AstNode> u_ptr_c_AstNode;

class AstNode {
  public:
    enum class NODE_TYPE {
        INTEGER,
        DOUBLE,
        UNARY_MINUS,
        VARIABLE,
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        POWER,
        FUNCTION,
        ERROR
    };
    explicit AstNode() = default;

    [[nodiscard]] bool isNumeric() const;
    [[nodiscard]] bool containsCopyOf(const AstNode* node) const;
    [[nodiscard]] bool isZero() const;
    [[nodiscard]] bool isOne() const;

    [[nodiscard]] virtual size_t         childCount() const          = 0;
    [[nodiscard]] virtual NODE_TYPE      type() const                = 0;
    [[nodiscard]] virtual std::string    toString() const            = 0;
    [[nodiscard]] virtual const AstNode* childAt(size_t index) const = 0;
    [[nodiscard]] virtual u_ptr_AstNode  simplify() const            = 0;
    [[nodiscard]] virtual u_ptr_AstNode  copy() const                = 0;

    static bool compare(const AstNode* lhs, const AstNode* rhs);

    friend std::ostream& operator<<(std::ostream& out, const AstNode& node);
    friend bool          operator==(const AstNode& lhs, const AstNode& rhs);
    friend bool          operator<(const AstNode& lhs, const AstNode& rhs);
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
    friend u_ptr_AstNode operator-(u_ptr_AstNode&& lhs);

  protected:
    [[nodiscard]] virtual bool compareEqualType(const AstNode* rhs) const = 0;
    [[nodiscard]] virtual bool equals(const AstNode& other) const         = 0;

    static bool compare_u_ptr(const u_ptr_AstNode& lhs, const u_ptr_AstNode& rhs);
};

#endif // PARSER_ASTNODE_H
