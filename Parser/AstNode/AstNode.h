//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODE_H
#define PARSER_ASTNODE_H

#include <functional>
#include <memory>
#include <string>

class AstNode {
  public:
    enum class NODE_TYPE {
        DOUBLE,
        INTEGER,
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

    [[nodiscard]] virtual std::string              toString() const = 0;
    [[nodiscard]] virtual std::unique_ptr<AstNode> copy() const     = 0;
    [[nodiscard]] virtual std::unique_ptr<AstNode> simplify() const = 0;
    [[nodiscard]] bool                             isNumeric() const;
    [[nodiscard]] virtual NODE_TYPE                type() const = 0;

    friend bool operator==(const AstNode& lhs, const AstNode& rhs);
    friend bool operator<(const AstNode& lhs, const AstNode& rhs);

  protected:
    [[nodiscard]] virtual bool equals(const AstNode& other) const = 0;

    static std::unique_ptr<AstNode> create(long long value);
    static std::unique_ptr<AstNode> create(double value);
    static double                   doubleValue(const AstNode* node);
    static long long int            integerValue(const AstNode* node);

    static std::unique_ptr<AstNode>
    doBinaryOperation(const std::unique_ptr<AstNode>& left, const std::unique_ptr<AstNode>& right,
                      const std::function<long long int(long long int, long long int)>& integerFunction,
                      const std::function<double(double, double)>&                      doubleFunction);
};

#endif // PARSER_ASTNODE_H
