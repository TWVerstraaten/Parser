//
// Created by pc on 07-06-21.
//

#include "AstToken.h"

#include "../../gen/Overloaded.h"
#include "../../gen/VariantTemplates.h"
#include "../../gen/defines.h"
#include "../Dimension.h"
#include "../err/ParserError.h"
#include "../err/ParserInfo.h"
#include "TokenTemplates.h"
#include "TokenWriter.h"

#include <algorithm>
#include <cassert>

namespace ast::par {

    static void S_REPLACE_UNARY_MINUSES(AstToken::TempTokenList& tempTokens, err::ParserInfo& info) {
        const auto needsReplacing = [&](const AstToken::TempToken& tempToken) { return TokenTemplates::S_IS_TOKEN_OF_TYPE<Token>(tempToken, Token::TYPE::UNARY_MINUS); };

        for (auto it = std::find_if(tempTokens.begin(), tempTokens.end(), needsReplacing); it != tempTokens.end() && std::next(it) != tempTokens.end();
             it      = std::find_if(it, tempTokens.end(), needsReplacing)) {
            assert(std::holds_alternative<AstToken>(*std::next(it)));
            const size_t start = std::get<Token>(*it).range().startIndex();
            it                 = tempTokens.erase(it);
            const size_t end   = std::get<AstToken>(*it).range().endIndex();
            *it                = AstToken{AstToken::OPERATOR_TYPE::UNARY_MINUS, std::get<AstToken>(*it), Range{start, end}, info};
            if (tempTokens.size() <= 1) {
                return;
            }
        }
    }

    static void S_REPLACE_TOKEN_OPERATORS(AstToken::TempTokenList& tempTokens, err::ParserInfo& info, const std::map<Token::TYPE, AstToken::OPERATOR_TYPE>& replacements) {
        const auto needsReplacing = [&](const AstToken::TempToken& tempToken) {
            return std::holds_alternative<Token>(tempToken) &&
                   std::find_if(TT_IT(replacements), [&](const auto& a) { return a.first == std::get<Token>(tempToken).type(); }) != replacements.end();
        };

        for (auto it = std::find_if(tempTokens.begin(), tempTokens.end(), needsReplacing); it != tempTokens.end() && std::next(it) != tempTokens.end();
             it      = std::find_if(it, tempTokens.end(), needsReplacing)) {
            if (it == tempTokens.begin()) {
                it = std::next(it);
                continue;
            }
            assert(std::holds_alternative<AstToken>(*std::next(it)));
            assert(std::holds_alternative<AstToken>(*std::prev(it)));
            const size_t start        = std::get<AstToken>(*std::prev(it)).range().startIndex();
            const size_t end          = std::get<AstToken>(*std::next(it)).range().endIndex();
            const auto   astTokenType = replacements.at(std::get<Token>(*it).type());
            *std::prev(it)            = AstToken{astTokenType, std::move(std::get<AstToken>(*std::prev(it))), std::move(std::get<AstToken>(*std::next(it))), {start, end}, info};
            it                        = tempTokens.erase(it, std::next(it, 2));
        }
    }

    AstToken::AstToken() = default;

    AstToken::AstToken(const std::list<StructuralToken>& structuralTokens, err::ParserInfo& info) {
        if (structuralTokens.empty()) {
            m_range = Range{};
            m_token = Empty{};
            return;
        }
        m_range = {structuralTokens.front().range().startIndex(), structuralTokens.back().range().endIndex()};

        if (auto it = std::find_if(TT_IT(structuralTokens), TT_LAMBDA(a, return TokenTemplates::S_IS_TOKEN_OF_TYPE<Token>(a.token(), Token::TYPE::EQUALS);));
            it != structuralTokens.end()) {
            m_token    = OPERATOR_TYPE::EQUALS;
            m_children = {AstToken{std::list<StructuralToken>(structuralTokens.begin(), it), info},
                          AstToken{std::list<StructuralToken>(std::next(it), structuralTokens.end()), info}};
            return;
        }

        TempTokenList tempTokens;
        for (const auto& el : structuralTokens) {
            std::visit(Overloaded{[&](const Token& a) { tempTokens.emplace_back(a); }, [&](const auto& a) { tempTokens.emplace_back(AstToken{a, el.range(), info}); }}, el.token());
        }
        S_REPLACE_TOKEN_OPERATORS(tempTokens, info, {{Token::TYPE::POWER, AstToken::OPERATOR_TYPE::POWER}});
        S_REPLACE_TOKEN_OPERATORS(tempTokens, info, {{Token::TYPE::TIMES, AstToken::OPERATOR_TYPE::TIMES}, {Token::TYPE::DIVIDE, AstToken::OPERATOR_TYPE::DIVIDE}});
        S_REPLACE_UNARY_MINUSES(tempTokens, info);
        S_REPLACE_TOKEN_OPERATORS(tempTokens, info, {{Token::TYPE::PLUS, AstToken::OPERATOR_TYPE::PLUS}, {Token::TYPE::MINUS, AstToken::OPERATOR_TYPE::MINUS}});

        if (tempTokens.size() > 1) {
            info.add({err::ParserError::TYPE::GENERIC, std::string("More than one token left at AstToken? "), m_range});
            return;
        }
        if (not std::holds_alternative<AstToken>(tempTokens.front())) {
            info.add({err::ParserError::TYPE::GENERIC, std::string("Cannot parse "), m_range});
            return;
        }
        assert(not tempTokens.empty());
        m_children = std::move(std::get<AstToken>(tempTokens.front()).m_children);
        m_token    = std::get<AstToken>(tempTokens.front()).m_token;
    }

    AstToken::AstToken(const StructuralToken::Bracketed& bracketed, Range range, err::ParserInfo& info) : m_token(VectorToken{bracketed.m_tokenLists.size()}), m_range(range) {
        for (const auto& el : bracketed.m_tokenLists) {
            m_children.emplace_back(el, info);
        }
        if (m_children.size() == 1 && std::holds_alternative<Empty>(m_children.front().m_token)) {
            m_children.clear();
            std::get<VectorToken>(m_token).m_dimension = 0;
        }
    }

    AstToken::AstToken(const StructuralToken::Function& function, Range range, err::ParserInfo& info)
        : m_token(FunctionToken{function.m_name, function.m_arguments.m_tokenLists.size()}), m_range(range) {
        for (const auto& el : function.m_arguments.m_tokenLists) {
            m_children.emplace_back(el, info);
        }
        if (m_children.size() == 1 && std::holds_alternative<Empty>(m_children.front().m_token)) {
            m_children.clear();
            std::get<FunctionToken>(m_token).setArgumentCount(0);
        }
        maybeCastToReservedFunction(info);
    }

    AstToken::AstToken(const std::string& string, Range range, err::ParserInfo& info) : m_token(string), m_range(range) {
    }

    AstToken::AstToken(long long int value, Range range, err::ParserInfo& info) : m_token(value), m_range(range) {
    }

    AstToken::AstToken(double value, Range range, err::ParserInfo& info) : m_token(value), m_range(range) {
    }

    AstToken::AstToken(AstToken::OPERATOR_TYPE type, AstToken astToken, Range range, err::ParserInfo& info) : m_token(type), m_children({std::move(astToken)}), m_range(range) {
    }

    AstToken::AstToken(AstToken::OPERATOR_TYPE type, AstToken left, AstToken right, Range range, err::ParserInfo& info)
        : m_token(type), m_children({std::move(left), std::move(right)}), m_range(range) {
    }

    void AstToken::maybeCastToReservedFunction(err::ParserInfo& info) {
        assert(std::holds_alternative<FunctionToken>(m_token));
        if (auto reserved = GET_RESERVED(std::get<FunctionToken>(m_token).name()); reserved.has_value()) {
            const auto val                   = reserved.value();
            const auto argumentCount         = std::get<FunctionToken>(m_token).argumentCount();
            const auto requiredArgumentCount = GET_ARGUMENT_COUNT(val);
            if (requiredArgumentCount != argumentCount) {
                info.add({err::ParserError::TYPE::WRONG_ARGUMENT_COUNT_RESERVED,
                          GET_NAME(val) + " takes " + std::to_string(requiredArgumentCount) + " arguments, not " + std::to_string(argumentCount),
                          m_range});
            }
            m_token = val;
        }
    }

    std::set<FunctionToken> AstToken::getFunctionDependencies() const {
        std::set<FunctionToken> functionDependencies;
        if (std::holds_alternative<FunctionToken>(m_token)) {
            functionDependencies.insert(std::get<FunctionToken>(m_token));
        }
        for (const auto& child : m_children) {
            functionDependencies.merge(child.getFunctionDependencies());
        }
        return functionDependencies;
    }

    std::set<std::string> AstToken::variablesUsed() const {
        std::set<std::string> variables;
        if (std::holds_alternative<std::string>(m_token)) {
            variables.insert(std::get<std::string>(m_token));
        }
        for (const auto& child : m_children) {
            variables.merge(child.variablesUsed());
        }
        return variables;
    }

    const AstToken::AstTokenVariant& AstToken::token() const {
        return m_token;
    }

    const std::vector<AstToken>& AstToken::children() const {
        return m_children;
    }

    const Range& AstToken::range() const {
        return m_range;
    }

    void AstToken::replaceVariable(const std::string& variable, const AstToken& token) {
        std::visit(Overloaded{[&](const std::string& variableToken) {
                                  assert(m_children.empty());
                                  if (variableToken == variable) {
                                      *this = token;
                                  }
                              },
                              [&](const auto& a) {
                                  for (auto& child : m_children) {
                                      child.replaceVariable(variable, token);
                                  }
                              }},
                   m_token);
    }

    void AstToken::replaceVariables(const std::map<std::string, AstToken>& variableMap) {
        std::visit(Overloaded{[&](const std::string& variableToken) {
                                  assert(m_children.empty());
                                  if (variableMap.find(variableToken) != variableMap.end()) {
                                      *this = variableMap.at(variableToken);
                                  }
                              },
                              [&](const auto& a) {
                                  for (auto& child : m_children) {
                                      child.replaceVariables(variableMap);
                                  }
                              }},
                   m_token);
    }

    void AstToken::replaceVariables(const std::vector<std::string>& variables, const std::vector<AstToken>& tokens) {
        assert(variables.size() == tokens.size());
        std::map<std::string, AstToken> variableMap;
        for (size_t i = 0; i != variables.size(); ++i) {
            assert(variableMap.find(variables.at(i)) == variableMap.end());
            variableMap.insert({variables.at(i), tokens.at(i)});
        }
        replaceVariables(variableMap);
    }

    std::string AstToken::toStringAsTree() const {
        return TokenWriter::S_TO_STRING_AS_TREE(*this);
    }

    void AstToken::replaceFunction(const FullHeader& header, const AstToken& functionAst) {
        for (auto& child : m_children) {
            child.replaceFunction(header, functionAst);
        }
        if (std::holds_alternative<FunctionToken>(m_token)) {
            const auto& function = std::get<FunctionToken>(m_token);
            if (function.name() == header.m_name && function.argumentCount() == header.m_variables.size()) {
                auto replaced = functionAst;
                replaced.replaceVariables(header.m_variables, m_children);
                *this = replaced;
            }
        }
    }

    void AstToken::replaceConstant(const ConstantHeader& header, const AstToken& constantAst) {
        for (auto& child : m_children) {
            child.replaceConstant(header, constantAst);
        }
        std::visit(Overloaded{[&](const ConstantToken& constant) {
                                  if (constant == header) {
                                      *this = constantAst;
                                  }
                              },
                              [&](const auto& a) {}},
                   m_token);
    }

    std::string AstToken::toStringFlat() const {
        return TokenWriter::S_TO_STRING_FLAT(*this);
    }

    std::set<std::string> AstToken::getUndeclaredVariables(const std::set<std::string>& declared) const {
        std::set<std::string> result;
        for (const auto& el : m_children) {
            result.merge(el.getUndeclaredVariables(declared));
        }
        if (std::holds_alternative<std::string>(m_token)) {
            const auto& variableName = std::get<std::string>(m_token);
            if (declared.find(variableName) == declared.end()) {
                result.insert(variableName);
            }
        }
        return result;
    }

    bool AstToken::hasEmptyBody() const {
        if (gen::S_VARIANT_EQUALS(m_token, OPERATOR_TYPE::EQUALS)) {
            assert(m_children.size() == 2);
            return m_children.back().hasEmptyBody();
        } else {
            return std::holds_alternative<Empty>(m_token);
        }
    }

    size_t AstToken::dimension() const {
        return std::visit(Overloaded{[](const Error& a) {
                                         assert(false);
                                         return DIMENSION_MISMATCH;
                                     },
                                     [](const FunctionToken& a) {
                                         assert(false);
                                         return DIMENSION_MISMATCH;
                                     },
                                     [](const Empty& a) {
                                         assert(false);
                                         return DIMENSION_MISMATCH;
                                     },
                                     [this](const OPERATOR_TYPE& a) { return dimensionOfOperator(); },
                                     [](const ReservedToken& a) { return ast::par::GET_DIMENSION(a); },
                                     [](const VectorToken& a) { return a.m_dimension; },
                                     [](const ConstantToken& a) { return static_cast<size_t>(1ul); },
                                     [](const double& a) { return static_cast<size_t>(1ul); },
                                     [](const long long int& a) { return static_cast<size_t>(1ul); }},
                          m_token);
    }

    size_t AstToken::dimensionOfOperator() const {
        assert(std::holds_alternative<OPERATOR_TYPE>(m_token));
        const auto& op = std::get<OPERATOR_TYPE>(m_token);
        switch (op) {
            case OPERATOR_TYPE::PLUS:
                assert(m_children.size() >= 2);
                {
                    std::vector<size_t> dimensions{};
                    dimensions.reserve(m_children.size());
                    std::transform(TT_IT(m_children), std::back_inserter(dimensions), [](const auto& a) { return a.dimension(); });
                    if (std::find(TT_IT(dimensions), DIMENSION_MISMATCH) != dimensions.end() ||
                        not std::equal(std::next(dimensions.begin()), dimensions.end(), dimensions.begin())) {
                        return DIMENSION_MISMATCH;
                    } else {
                        return dimensions.front();
                    }
                }
            case OPERATOR_TYPE::MINUS:
                assert(m_children.size() >= 2);
                {
                    std::vector<size_t> dimensions{};
                    dimensions.reserve(m_children.size());
                    std::transform(TT_IT(m_children), std::back_inserter(dimensions), [](const auto& a) { return a.dimension(); });
                    if (std::find(TT_IT(dimensions), DIMENSION_MISMATCH) != dimensions.end() ||
                        not std::equal(std::next(dimensions.begin()), dimensions.end(), dimensions.begin())) {
                        return DIMENSION_MISMATCH;
                    } else {
                        return dimensions.front();
                    }
                }
            case OPERATOR_TYPE::TIMES: {
                assert(m_children.size() >= 2);
                std::vector<size_t> dimensions{};
                dimensions.reserve(m_children.size());
                std::transform(TT_IT(m_children), std::back_inserter(dimensions), [](const auto& a) { return a.dimension(); });
                if (std::find(TT_IT(dimensions), DIMENSION_MISMATCH) != dimensions.end()) {
                    return DIMENSION_MISMATCH;
                } else {
                    auto firstVectorIt = std::find_if(TT_IT(dimensions), [](const size_t d) { return d > 1; });
                    if (firstVectorIt == dimensions.end()) {
                        return 1;
                    }
                    if (std::find_if(std::next(firstVectorIt), dimensions.end(), [](const size_t d) { return d > 1; }) != dimensions.end()) {
                        return DIMENSION_MISMATCH;
                    } else {
                        return *firstVectorIt;
                    }
                }
            } break;
            case OPERATOR_TYPE::DIVIDE:
                assert(m_children.size() == 2);
                if (m_children.back().dimension() == 1) {
                    return m_children.front().dimension();
                } else {
                    return DIMENSION_MISMATCH;
                }
                break;
            case OPERATOR_TYPE::POWER:
                assert(m_children.size() == 2);
                if (m_children.front().dimension() == 1 && m_children.back().dimension() == 1) {
                    return 1;
                } else {
                    return DIMENSION_MISMATCH;
                }
                break;
            case OPERATOR_TYPE::UNARY_MINUS:
                assert(m_children.size() == 1);
                return m_children.front().dimension();
                break;
            case OPERATOR_TYPE::EQUALS:
                assert(false);
                break;
        }
        return DIMENSION_MISMATCH;
    }
} // namespace ast::par