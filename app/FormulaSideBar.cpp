//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

#include "../alg/StringAlg.h"
#include "../ast/UnrollCompanion.h"
#include "FormulaWidget.h"
#include "UndoRedoHandler.h"
#include "cmd/NewFormulaWidgetCommand.h"
#include "cmd/RemoveFormulaWidgetCommand.h"

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

        UndoRedoHandler::SET_PUSH_BLOCKED(true);
        addNewFormulaWidget();
        addNewFormulaWidget();
        UndoRedoHandler::SET_PUSH_BLOCKED(false);

        m_layout->addWidget(m_infoLabel);

        m_layout->setSpacing(5);
        m_layout->setContentsMargins(2, 12, 12, 2);
        m_layout->setAlignment(Qt::AlignTop);
        setBaseSize({400, 800});
    }

    void FormulaSideBar::addNewFormulaWidget() {
        auto* newFormulaWidget = new FormulaWidget(m_scrollArea->widget());
        connect(newFormulaWidget, &FormulaWidget::updated, [this](size_t index) { updateAt(index); });
        connect(newFormulaWidget, &FormulaWidget::deleteClicked, this, &FormulaSideBar::removeFormulaWidget);

        m_formulaWidgets.push_back(newFormulaWidget);
        UndoRedoHandler::PUSH(new cmd::NewFormulaWidgetCommand(this, newFormulaWidget->index()));
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
        UndoRedoHandler::PUSH(new cmd::RemoveFormulaWidgetCommand(this, indexOfWidget));
        updateAt(indexOfWidget);
    }

    void FormulaSideBar::updateAt(size_t index) {
        auto* widget = fromIndex(index);
        if (widget->isHidden() || not widget->hasSuccessfulNonEmptyAst()) {
            m_astManager.remove(index);
        } else {
            m_astManager.update(index, widget->ast());
        }
        //        const auto& updatedWidget = fromIndex(index);
        //        if ()

        //        checkRedeclarations();
        //        checkUndefined();
        writeInfoToInfoLabel();
        //        updateAllTextEdits();
    }

    void FormulaSideBar::writeInfoToInfoLabel() {
        const auto declaredFunctions   = getDeclaredFunctions();
        const auto declaredConstants   = getDeclaredConstants();
        const auto referencedFunctions = getReferencedFunctions();
        const auto referencedConstants = getReferencedConstants();
        QString    info                = "<b>Declared:</b><br>";
        info.append(QString::fromStdString("Funcs: " + alg::str::CONCATENATE_STRINGS<ast::par::FunctionToken>(declaredFunctions, &ast::par::FunctionToken::toString)) + "<br>");
        info.append(QString::fromStdString("Csts: " + alg::str::CONCATENATE_STRINGS(declaredConstants)) + "<br>");
        info.append("<b>Referenced:</b><br>");
        info.append(QString::fromStdString("Funcs: " + alg::str::CONCATENATE_STRINGS<ast::par::FunctionToken>(referencedFunctions, &ast::par::FunctionToken::toString)) + "<br>");
        info.append(QString::fromStdString("Csts: " + alg::str::CONCATENATE_STRINGS(referencedConstants)) + "<br>");
        info.append(QString::fromStdString(m_astManager.toString()).replace("\n", "<br>"));
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

    std::vector<ast::par::FunctionToken> FormulaSideBar::getDeclaredFunctions() const {
        std::vector<ast::par::FunctionToken> declaredFunctions;
        for (const auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasSuccessfulNonEmptyAst()) {
                continue;
            }
            const auto& ast = widget->ast();
            if (ast.isFunction()) {
                declaredFunctions.emplace_back(ast.getFunctionToken());
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
            if (ast.isConstant()) {
                declaredConstants.emplace_back(std::get<ast::Header::ConstantHeader>(ast.header().headerVariant()).m_name);
            }
        }
        return declaredConstants;
    }

    std::set<ast::par::FunctionToken> FormulaSideBar::getReferencedFunctions() const {
        std::set<ast::par::FunctionToken> result;
        for (const auto& widget : m_formulaWidgets) {
            if (widget->isHidden() || not widget->hasAst() || not widget->ast().success() || widget->ast().isEmpty()) {
                continue;
            }
            const auto& ast = widget->ast();
            result.merge(ast.getFunctionDependencies());
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
            result.merge(ast.getConstantDependencies());
        }
        return result;
    }

} // namespace app
