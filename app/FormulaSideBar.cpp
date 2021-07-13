//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

#include "../alg/StringAlg.h"
#include "../ast/Ast.h"
#include "../ast/err/DefinitionError.h"
#include "../gen/DependencyGraph.h"
#include "FormulaWidget.h"
#include "cmd/NewFormulaWidgetCommand.h"
#include "cmd/RemoveFormulaWidgetCommand.h"

#include <QAction>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace app {
    FormulaSideBar::FormulaSideBar(QWidget* parent) : QWidget(parent) {
        m_layout = new QVBoxLayout();

        makeScrollArea();
        addNewFormulaWidgetButton();
        addNewFormulaWidget();
        addNewFormulaWidget();
        m_layout->addWidget(m_infoLabel);

        m_layout->setSpacing(5);
        m_layout->setContentsMargins(2, 12, 12, 2);
        m_layout->setAlignment(Qt::AlignTop);
        setBaseSize({400, 800});
    }

    void FormulaSideBar::addNewFormulaWidget() {
        auto* newFormulaWidget = new FormulaWidget(m_scrollArea->widget());
        //        const auto newFormulaIndex  = newFormulaWidget->index();
        connect(newFormulaWidget, &FormulaWidget::updated, [this](size_t index) { updateAt(index); });
        connect(newFormulaWidget, &FormulaWidget::deleteClicked, this, &FormulaSideBar::removeFormulaWidget);

        m_formulaWidgets.push_back(newFormulaWidget);
        //        UndoRedoHandler::PUSH(new cmd::NewFormulaWidgetCommand(this, newFormulaIndex));
        m_layout->removeWidget(m_infoLabel);
        m_layout->removeWidget(m_newFormulaPushButton);
        m_layout->addWidget(m_formulaWidgets.back());
        m_layout->addWidget(m_newFormulaPushButton);
        m_layout->addWidget(m_infoLabel);
    }

    FormulaWidget* FormulaSideBar::fromIndex(size_t index) {
        return *std::find_if(m_formulaWidgets.begin(), m_formulaWidgets.end(), [index](const auto* widget) { return widget->index() == index; });
    }

    void FormulaSideBar::removeFormulaWidget(size_t indexOfWidget) {
        //        UndoRedoHandler::PUSH(new cmd::RemoveFormulaWidgetCommand(this, indexOfWidget));
        updateAt(indexOfWidget);
    }

    void FormulaSideBar::updateAt(size_t index) {
        qDebug() << "FormulaSideBar updated";
        clearDefinitionErrors();
        checkRedeclarations();
        checkUndefined();
        writeInfoToInfoLabel();
        updateAllTextEdits();
    }

    void FormulaSideBar::writeInfoToInfoLabel() {
        const auto declaredFunctions   = getDeclaredFunctions();
        const auto declaredConstants   = getDeclaredConstants();
        const auto referencedFunctions = getReferencedFunctions();
        const auto referencedConstants = getReferencedConstants();
        QString    info                = "<b>Declared:</b><br>";
        info.append(QString::fromStdString("Funcs: " + alg::str::CONCATENATE_STRINGS<ast::par::CustomFunctionToken>(declaredFunctions, &ast::par::CustomFunctionToken::toString)) +
                    "<br>");
        info.append(QString::fromStdString("Csts: " + alg::str::CONCATENATE_STRINGS(declaredConstants)) + "<br>");
        info.append("<b>Referenced:</b><br>");
        info.append(
            QString::fromStdString("Funcs: " + alg::str::CONCATENATE_STRINGS<ast::par::CustomFunctionToken>(referencedFunctions, &ast::par::CustomFunctionToken::toString)) +
            "<br>");
        info.append(QString::fromStdString("Csts: " + alg::str::CONCATENATE_STRINGS(referencedConstants)) + "<br>");
        m_infoLabel->setText(info);
    }

    const std::vector<FormulaWidget*>& FormulaSideBar::formulaWidgets() const {
        return m_formulaWidgets;
    }

    void FormulaSideBar::addNewFormulaWidgetButton() {
        m_newFormulaPushButton = new QPushButton(m_scrollArea->widget());
        m_newFormulaPushButton->setText("+");
        connect(m_newFormulaPushButton, &QPushButton::clicked, this, &FormulaSideBar::addNewFormulaWidget);
        m_layout->addWidget(m_newFormulaPushButton);
    }

    void FormulaSideBar::makeScrollArea() {
        m_infoLabel  = new QLabel("Info", this);
        m_scrollArea = new QScrollArea(this);
        m_scrollArea->setWidgetResizable(true);
        auto* widget = new QWidget();
        widget->setLayout(m_layout);
        m_scrollArea->setWidget(widget);
        auto* layout = new QHBoxLayout(this);
        layout->addWidget(m_scrollArea);
        setLayout(layout);
    }

    void FormulaSideBar::checkRedeclarations() {
        std::map<std::string, size_t> seenDeclarations;
        for (const auto& widget : m_formulaWidgets) {
            if ((not widget->isHidden()) && widget->hasAst() && widget->ast().info().parserErrors().empty() && (not widget->ast().isEmpty())) {
                const auto& ast = widget->ast();
                if (ast.headerType() != ast::Header::HEADER_TYPE::FULL_HEADER && ast.headerType() != ast::Header::HEADER_TYPE::CONSTANT) {
                    continue;
                }
                const size_t      index = widget->index();
                const std::string name  = ast.getDeclaredName();
                if (seenDeclarations.find(name) != seenDeclarations.end()) {
                    widget->info().add({ast::err::DefinitionError::TYPE::REDECLARATION, name});
                    fromIndex(seenDeclarations[name])->info().add({ast::err::DefinitionError::TYPE::REDECLARATION, name});
                } else {
                    seenDeclarations.insert({name, index});
                }
            }
        }
    }

    void FormulaSideBar::checkFormulaWidgetsParsed() {
    }

    void FormulaSideBar::checkUndefined() {
        const auto d                 = getDeclaredFunctions();
        const auto declaredFunctions = std::set<ast::par::CustomFunctionToken>(d.begin(), d.end());
        const auto declaredConstants = getDeclaredConstants();

        for (auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasSuccessfulNonEmptyAst()) {
                continue;
            }
            const auto                              referenced = widget->ast().functionDependencies();
            std::set<ast::par::CustomFunctionToken> undefinedFunctions;
            for (const auto& el : referenced) {
                if (declaredFunctions.find(el) == declaredFunctions.end()) {
                    undefinedFunctions.insert(el);
                }
            }
            if (not undefinedFunctions.empty()) {
                widget->info().add({ast::err::DefinitionError::TYPE::UNDEFINED_FUNCTIONS, "undefined"});
            }
        }
    }

    void FormulaSideBar::checkCircularDependenciesAndUndefined() {
    }

    void FormulaSideBar::clearDefinitionErrors() {
        for (auto& widget : m_formulaWidgets) {
            if ((not widget->isHidden()) && widget->hasAst()) {
                widget->info().clearDefinitionErrors();
                widget->updateTextEdit();
            }
        }
    }

    std::vector<ast::par::CustomFunctionToken> FormulaSideBar::getDeclaredFunctions() const {
        std::vector<ast::par::CustomFunctionToken> declaredFunctions;
        for (const auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasSuccessfulNonEmptyAst()) {
                continue;
            }
            const auto& ast = widget->ast();
            if (ast.headerType() == ast::Header::HEADER_TYPE::FULL_HEADER) {
                declaredFunctions.emplace_back(ast.getCustomFunctionToken());
            }
        }
        return declaredFunctions;
    }

    std::vector<std::string> FormulaSideBar::getDeclaredConstants() const {
        std::vector<std::string> declaredConstants;
        for (const auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasSuccessfulNonEmptyAst()) {
                continue;
            }
            const auto& ast = widget->ast();
            if (ast.headerType() == ast::Header::HEADER_TYPE::CONSTANT) {
                declaredConstants.emplace_back(std::get<ast::Header::ConstantHeader>(ast.header().headerVariant()).m_name);
            }
        }
        return declaredConstants;
    }
    std::set<ast::par::CustomFunctionToken> FormulaSideBar::getReferencedFunctions() const {
        std::set<ast::par::CustomFunctionToken> result;
        for (const auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasAst() || not widget->ast().success() || widget->ast().isEmpty()) {
                continue;
            }
            const auto& ast = widget->ast();
            result.merge(ast.functionDependencies());
        }
        return result;
    }

    std::set<std::string> FormulaSideBar::getReferencedConstants() const {
        std::set<std::string> result;
        for (const auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasAst() || not widget->ast().success() || widget->ast().isEmpty()) {
                continue;
            }
            const auto& ast = widget->ast();
            result.merge(ast.constantDependencies());
        }
        return result;
    }

    void FormulaSideBar::updateAllTextEdits() {
        for (auto& widget : m_formulaWidgets) {
            if ((not widget->isHidden()) && widget->hasAst()) {
                widget->updateTextEdit();
            }
        }
    }

} // namespace app
