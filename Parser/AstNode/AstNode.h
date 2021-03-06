//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODE_H
#define PARSER_ASTNODE_H

#include <iostream>
#include <memory>
#include <string>

class AstNode;
typedef std::unique_ptr<AstNode> u_ptr_AstNode;

struct IntersectStruct;
class AstNodeMul;
class AstNodePower;

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
    [[nodiscard]] bool isCommutative() const;
    [[nodiscard]] bool containsCopyOf(const AstNode* node) const;
    [[nodiscard]] bool isZero() const;
    [[nodiscard]] bool isOne() const;
    [[nodiscard]] bool isEven() const;

    static IntersectStruct factor(const AstNode* first, const AstNode* second, bool isFirstPass = true);
    static IntersectStruct factorNodeAndMultiply(const AstNode* first, const AstNodeMul* second);
    static IntersectStruct factor(const AstNodeMul* first, const AstNode* second);
    static IntersectStruct factor(const AstNodePower* first, const AstNode* second);
    static IntersectStruct factorPowerAndMultiply(const AstNodePower* power, const AstNodeMul* multiplyNode);
    static IntersectStruct factorPowers(const AstNodePower* power1, const AstNodePower* power2);

    static u_ptr_AstNode zero();
    static u_ptr_AstNode one();
    static u_ptr_AstNode makeInteger(long long val);

    enum class SIMPLIFY_RULES { NONE, DISTRIBUTE_MULTIPLICATION };

    [[nodiscard]] virtual size_t         childCount() const                           = 0;
    [[nodiscard]] virtual NODE_TYPE      type() const                                 = 0;
    [[nodiscard]] virtual std::string    toString() const                             = 0;
    [[nodiscard]] virtual const AstNode* childAt(size_t index) const                  = 0;
    [[nodiscard]] virtual u_ptr_AstNode  simplify(SIMPLIFY_RULES simplifyRules) const = 0;
    [[nodiscard]] virtual u_ptr_AstNode  copy() const                                 = 0;

    static bool          compare(const AstNode* lhs, const AstNode* rhs);
    static u_ptr_AstNode copy(const u_ptr_AstNode& node);
    static u_ptr_AstNode simplify(const u_ptr_AstNode& node);

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
    friend u_ptr_AstNode operator-(const AstNode& val);
    friend u_ptr_AstNode operator-(u_ptr_AstNode&& val);

  protected:
    [[nodiscard]] virtual bool compareEqualType(const AstNode* rhs) const = 0;
    [[nodiscard]] virtual bool equals(const AstNode& other) const         = 0;

    static bool compare_u_ptr(const u_ptr_AstNode& lhs, const u_ptr_AstNode& rhs);
};


#endif // PARSER_ASTNODE_H
