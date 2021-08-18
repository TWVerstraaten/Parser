//
// Created by pc on 09-07-21.
//

#include "AstManager.h"

#include "../alg/StringAlg.h"
#include "../gen/defines.h"
#include "Ast.h"
#include "UnrollCompanion.h"
#include "UnrolledAst.h"

#include <QDebug>
#include <memory>
#include <sstream>

namespace ast {

    AstManager::AstManager() = default;

    AstManager::~AstManager() = default;

    void AstManager::update(size_t index, const Ast& ast) {
        assert(ast.success());
        auto it = fromIndex(index);
        if (it == m_unrollCompanions.end()) {
            m_unrollCompanions.emplace_back(std::make_unique<UnrollCompanion>(index, ast));
            assert((*m_unrollCompanions.back()).status() == UnrollCompanion::STATUS::UN_CHECKED);
        } else {
            *it = std::make_unique<UnrollCompanion>(index, ast);
            assert((*it)->status() == UnrollCompanion::STATUS::UN_CHECKED);
        }
        setAllToUnchecked();
        checkRedeclarations();
        unrollAll();
    }

    std::string AstManager::toString() const {
        std::stringstream ss;
        ss << std::string(25, '=') << '\n';
        bool firstPass = true;
        for (const auto& el : m_unrollCompanions) {
            if (not firstPass) {
                ss << std::string(25, '-') << '\n';
            }
            firstPass = false;
            ss << el->originalAst().header().toString() << "=>\t" << el->originalAst().toStringFlat() << "\n";
            ss << el->statusString() << "\n";
        }
        ss << std::string(25, '=') << '\n';
        return ss.str();
    }

    const gen::DependencyGraph& AstManager::dependencyGraph() const {
        return m_dependencyGraph;
    }

    void AstManager::remove(size_t index) {
        const auto it = fromIndex(index);
        if (it != m_unrollCompanions.end()) {
            m_unrollCompanions.erase(it);
        }
    }

    std::vector<std::unique_ptr<UnrollCompanion>>::iterator AstManager::fromIndex(size_t index) {
        return std::find_if(TT_IT(m_unrollCompanions), TT_LAMBDA_REF(a, return a->index() == index;));
    }

    void AstManager::checkRedeclarations() {
        for (auto it = m_unrollCompanions.begin(); it != m_unrollCompanions.end(); ++it) {
            const auto ast = (*it)->originalAst();
            assert(ast.success());
            if (ast.isDeclaration()) {
                const auto declaredName = ast.getDeclaredName();
                auto       duplicate    = std::find_if(std::next(it), m_unrollCompanions.end(), [&](const std::unique_ptr<UnrollCompanion>& a) {
                    return a->originalAst().isDeclaration() && a->originalAst().getDeclaredName() == declaredName;
                         });
                if (duplicate != m_unrollCompanions.end()) {
                    assert(it != duplicate);
//                    const auto itType        = ast.isFunction() ? err::RedeclarationError::TYPE::FUNCTION : err::RedeclarationError::TYPE::CONSTANT;
//                    const auto duplicateType = (*duplicate)->originalAst().isFunction() ? err::RedeclarationError::TYPE::FUNCTION : err::RedeclarationError::TYPE::CONSTANT;
//                    (*it)->addDeclarationError({err::RedeclarationError(declaredName, duplicateType, itType)});
//                    (*duplicate)->addDeclarationError({err::RedeclarationError(declaredName, itType, duplicateType)});
                    qDebug() << "Redeclared\t" << QString::fromStdString(declaredName);
                }
            }
        }
    }

    void AstManager::checkUndefinedReferences() {
        //        std::set<par::FunctionToken> declaredFunctions;
        //        std::set<std::string>        declaredConstants;
        //        for (const auto& unrollCompanion : m_unrollCompanions) {
        //            if (unrollCompanion->status() == UnrollCompanion::STATUS::CANNOT_BE_UNROLLED) {
        //                continue;
        //            }
        //            const auto& ast = (*unrollCompanion).originalAst();
        //            if (ast.isDeclaration()) {
        //                if (ast.isFunction()) {
        //                    declaredFunctions.emplace(ast.getFunctionToken());
        //                } else {
        //                    assert(ast.isConstant());
        //                    declaredConstants.emplace(ast.getDeclaredName());
        //                }
        //            }
        //        }
        //
        //        for (auto& unrollCompanion : m_unrollCompanions) {
        //            const auto&                  ast                  = (*unrollCompanion).originalAst();
        //            const auto                   getFunctionDependencies = ast.getFunctionDependencies();
        //            std::set<par::FunctionToken> undefinedFunctions;
        //            std::set_difference(TT_IT(getFunctionDependencies), TT_IT(declaredFunctions), std::inserter(undefinedFunctions, undefinedFunctions.end()));
        //            if (not undefinedFunctions.empty()) {
        //                unrollCompanion->addDeclarationError({err::UndefinedReferenceError{
        //                    alg::str::CONCATENATE_STRINGS<par::FunctionToken>(undefinedFunctions, &par::FunctionToken::toString), err::UndefinedReferenceError::TYPE::FUNCTION}});
        //                qDebug() << "Undefined: " << QString::fromStdString(alg::str::CONCATENATE_STRINGS<par::FunctionToken>(undefinedFunctions, &par::FunctionToken::toString));
        //            }
        //            const auto            constantDependencies = ast.getConstantDependencies();
        //            std::set<std::string> undefinedConstants;
        //            std::set_difference(TT_IT(getConstantDependencies), TT_IT(declaredConstants), std::inserter(undefinedConstants, undefinedConstants.end()));
        //            if (not undefinedConstants.empty()) {
        //                unrollCompanion->addDeclarationError(
        //                    {err::UndefinedReferenceError{alg::str::CONCATENATE_STRINGS(undefinedConstants), err::UndefinedReferenceError::TYPE::CONSTANT}});
        //                qDebug() << "Undefined: " << QString::fromStdString(alg::str::CONCATENATE_STRINGS(undefinedConstants));
        //            }
        //        }
    }

    void AstManager::unrollAll() {
        for (auto& unrollCompanion : m_unrollCompanions) {
            unroll(*unrollCompanion);
        }
    }

    void AstManager::setAllToUnchecked() {
        for (auto& unrollCompanion : m_unrollCompanions) {
            unrollCompanion->setStatus(UnrollCompanion::STATUS::UN_CHECKED);
        }
    }

    void AstManager::unroll(UnrollCompanion& current, const std::vector<size_t>& history) {
        if (current.status() == UnrollCompanion::STATUS::UNROLLED || current.status() == UnrollCompanion::STATUS::CANNOT_BE_UNROLLED) {
            return;
        }
        auto&      partial              = current.partiallyUnrolledAst();
        const auto functionDependencies = partial.getFunctionDependencies();
        const auto constantDependencies = partial.getConstantDependencies();
        if (functionDependencies.empty() && constantDependencies.empty()) {
            current.doUnroll();
        }
    }

    std::vector<std::unique_ptr<UnrollCompanion>>::iterator AstManager::fromFunctionToken(const par::FunctionToken& functionToken) {
        return std::find_if(TT_IT(m_unrollCompanions),
                            [&](const std::unique_ptr<UnrollCompanion>& a) { return a->originalAst().isFunction() && a->originalAst().getFunctionToken() == functionToken; });
    }

    std::vector<std::unique_ptr<UnrollCompanion>>::iterator AstManager::fromConstantName(const std::string& constantName) {
        return std::find_if(TT_IT(m_unrollCompanions),
                            [&](const std::unique_ptr<UnrollCompanion>& a) { return a->originalAst().isConstant() && a->originalAst().getDeclaredName() == constantName; });
    }

} // namespace ast