//
// Created by pc on 09-07-21.
//

#include "AstManager.h"

#include <sstream>

AstManager::AstManager() = default;

void AstManager::add(size_t index, Ast&& ast) {
    fromHeaderType(ast.header().type()).emplace_back(index, std::move(ast));
    setUnrollStatuses();
}

std::string AstManager::toString() const {
    std::stringstream ss;
    for (const auto& el : m_astTokens) {
        ss << std::string(80, '=') << '\n';
        switch (el.first) {
            case Header::HEADER_TYPE::EMPTY:
                ss << ">> Anonymous functions\n\n";
                break;
            case Header::HEADER_TYPE::CONSTANT:
                ss << ">> Constants\n\n";
                break;
            case Header::HEADER_TYPE::FULL_HEADER:
                ss << ">> Full functions\n\n";
                break;
            case Header::HEADER_TYPE::SINGLE_COORDINATE:
                ss << ">> Single coordinate\n\n";
                break;
            case Header::HEADER_TYPE::COORDINATE_VECTOR:
                ss << ">> Coordinate vector\n\n";
                break;
        }
        for (const auto& unrollCompanion : m_astTokens.at(el.first)) {
            ss << unrollCompanion.originalAst().toStringFlat() << "\n";
            ss << unrollCompanion.statusString() << "\n\n";
            //            ss << "Depends on:\t ";
            //            const auto dependencies = unrollCompanion.functionDependencies();
            //            for (const auto& dep : dependencies) {
            //                ss << dep.name() << "(" << dep.argumentCount() << ")"
            //                   << "\t";
            //            }
            //            ss << "\nUnrolls to:\n";
            //            ss << unroll(unrollCompanion).toString() << "\n\n";
        }
        ss << std::string(80, '=') << '\n';
    }
    return ss.str();
}

UnrolledAst AstManager::unroll(const Ast& ast) const {
    auto unrolled = ast;
    //    const auto& fullFunctions = m_astTokens.at(Header::HEADER_TYPE::FULL_HEADER);
    //    bool        wasUpdated    = true;
    //
    //    while (wasUpdated) {
    //        wasUpdated = false;
    //        for (const auto& full : fullFunctions) {
    //            auto dependencies = unrolled.functionDependencies();
    //            if (dependencies.find(full.originalAst().getCustomFunctionToken()) != dependencies.end()) {
    //                wasUpdated = true;
    //                unrolled.replaceFunctionInPlace(full);
    //            }
    //        }
    //    }
    return UnrolledAst{unrolled};
}

void AstManager::setUnrollStatuses() {
    //    for (auto& el : m_astTokens){
    //        for (auto& companion : el.second){
    //
    //        }
    //    }
}

std::vector<UnrollCompanion>& AstManager::fromHeaderType(Header::HEADER_TYPE type) {
    return m_astTokens[type];
}

std::optional<UnrollCompanion*> AstManager::fromIndex(size_t index) {
    return fromComparator([index](const auto& a) { return a.index() == index; });
}

std::optional<UnrollCompanion*> AstManager::fromFunctionName(const std::string& functionName) {
    return fromComparator([functionName](const UnrollCompanion& a) {
        return (std::holds_alternative<Header::FullHeader>(a.originalAst().header().headerVariant())) &&
               std::get<Header::FullHeader>(a.originalAst().header().headerVariant()).m_name == functionName;
    });
}

std::optional<UnrollCompanion*> AstManager::fromComparator(const std::function<bool(const UnrollCompanion&)>& comparator) {
    for (auto& el : m_astTokens) {
        for (auto& companion : el.second) {
            if (comparator(companion)) {
                return &companion;
            }
        }
    }
    return {};
}

std::optional<UnrollCompanion*> AstManager::fromFunctionToken(const CustomFunctionToken& functionToken) {
    return fromComparator([functionToken](const UnrollCompanion& a) {
        return (std::holds_alternative<Header::FullHeader>(a.originalAst().header().headerVariant())) && (a.originalAst().getCustomFunctionToken() == functionToken);
    });
}

const gen::DependencyGraph& AstManager::dependencyGraph() const {
    return m_dependencyGraph;
}
