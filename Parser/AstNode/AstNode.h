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

    virtual void addNode(AstNode* node);

    [[nodiscard]] bool           isNumeric() const;
    [[nodiscard]] bool           containsCopyOf(const AstNode* node) const;
    [[nodiscard]] size_t         indexOfCopy(const AstNode* node) const;
    [[nodiscard]] const AstNode* findViaTypeAndChild(NODE_TYPE type, const AstNode* node) const;
    [[nodiscard]] std::pair<const AstNode*, const AstNode*> findViaTypeContainingCopy(NODE_TYPE type) const;
    [[nodiscard]] bool                                      isZero() const;
    [[nodiscard]] bool                                      isOne() const;

    virtual std::vector<std::unique_ptr<AstNode>>::iterator removeChild(const AstNode* nodeToRemove);
    void                                                    removeIf(std::function<bool(const AstNode*)> predicate);
    void                                                    removeNodeAndNodeWithSameChild(NODE_TYPE type);
    void transformNodeAndNodeWithSameChild(NODE_TYPE type, const std::function<AstNode*(const AstNode*, const AstNode*)>& f);

    [[nodiscard]] virtual std::string              toString() const            = 0;
    [[nodiscard]] virtual std::unique_ptr<AstNode> copy() const                = 0;
    [[nodiscard]] virtual std::unique_ptr<AstNode> simplify() const            = 0;
    [[nodiscard]] virtual NODE_TYPE                type() const                = 0;
    [[nodiscard]] virtual size_t                   childCount() const          = 0;
    [[nodiscard]] virtual const AstNode*           childAt(size_t index) const = 0;

    static double        doubleValue(const AstNode* node);
    static long long int integerValue(const AstNode* node);

    bool        operator==(const AstNode& rhs) const;
    static bool compare(const std::unique_ptr<AstNode>& lhs, const std::unique_ptr<AstNode>& rhs);

  protected:
    [[nodiscard]] virtual bool equals(const AstNode& other) const = 0;

    static std::unique_ptr<AstNode> create(long long value);
    static std::unique_ptr<AstNode> create(double value);

    static std::unique_ptr<AstNode>
    doBinaryOperation(const std::unique_ptr<AstNode>& left, const std::unique_ptr<AstNode>& right,
                      const std::function<long long int(long long int, long long int)>& integerFunction,
                      const std::function<double(double, double)>&                      doubleFunction);
};

#endif // PARSER_ASTNODE_H
