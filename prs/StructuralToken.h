//
// Created by pc on 07-06-21.
//

#ifndef PRS_STRUCTURALTOKEN_H
#define PRS_STRUCTURALTOKEN_H

#include "Token.h"

#include <list>
#include <variant>
#include <vector>

class StructuralToken {

  public:
    struct Bracketed {
        std::vector<std::list<StructuralToken>> m_tokenLists;
    };

    struct Function {
        std::string m_name;
        Bracketed   m_arguments;
    };

    typedef std::variant<Token, Bracketed, Function, std::string, double, long long int> StructuralTokenVariant;

    explicit StructuralToken(const Token& token);
    StructuralToken(const std::string& variable, Range range);
    StructuralToken(double number, Range range);
    StructuralToken(long long number, Range range);
    StructuralToken(Bracketed&& bracketed, Range range);
    StructuralToken(Function&& function, Range range);

    [[nodiscard]] bool                          isString() const;
    [[nodiscard]] std::string                   toString() const;
    [[nodiscard]] const StructuralTokenVariant& token() const;
    [[nodiscard]] const Range&                  range() const;

    [[nodiscard]] static StructuralToken makeFromCommaSeparated(std::list<StructuralToken>&& tokenList);

  private:
    [[nodiscard]] static Bracketed makeBracketed(std::list<StructuralToken>& tokenList);

    std::variant<Token, Bracketed, Function, std::string, double, long long> m_token;
    Range                                                                    m_range;
};

#endif // PRS_STRUCTURALTOKEN_H
