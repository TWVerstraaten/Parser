//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

#include "../gen/DependencyGraph.h"
#include "FormulaWidget.h"
#include "UndoRedoConsumer.h"
#include "UndoRedoHandler.h"
#include "cmd/NewFormulaWidgetCommand.h"
#include "cmd/RemoveFormulaWidgetCommand.h"

#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <iostream>

namespace app {
    FormulaSideBar::FormulaSideBar(QWidget* parent) : QWidget(parent) {
        m_layout = new QVBoxLayout();
        makeScrollArea();

        addNewFormulaWidgetButton();

        UndoRedoHandler::setPushBlocked(true);
        addNewFormulaWidget();
        addNewFormulaWidget();
        UndoRedoHandler::setPushBlocked(false);

        m_layout->setAlignment(Qt::AlignTop);
        setBaseSize({400, 800});
        installEventFilter(UndoRedoConsumer::undoRedoConsumer());
    }

    void FormulaSideBar::addNewFormulaWidget() {
        auto*      newFormulaWidget = new FormulaWidget(m_scrollArea->widget());
        const auto newFormulaIndex  = newFormulaWidget->index();
        connect(newFormulaWidget, &FormulaWidget::updated, [this](size_t index) { updateAt(index); });
        connect(newFormulaWidget, &FormulaWidget::deleteClicked, this, &FormulaSideBar::RemoveFormulaWidget);

        m_formulaWidgets.push_back(newFormulaWidget);
        UndoRedoHandler::push(new cmd::NewFormulaWidgetCommand(this, newFormulaIndex));

        m_layout->removeWidget(m_newFormulaPushButton);
        m_layout->addWidget(m_formulaWidgets.back());
        m_layout->addWidget(m_newFormulaPushButton);
    }

    FormulaWidget* FormulaSideBar::fromIndex(size_t index) {
        return *std::find_if(m_formulaWidgets.begin(), m_formulaWidgets.end(), [index](FormulaWidget* widget) { return widget->index() == index; });
    }

    void FormulaSideBar::RemoveFormulaWidget(size_t indexOfWidget) {
        UndoRedoHandler::push(new cmd::RemoveFormulaWidgetCommand(this, indexOfWidget));
        updateAt(indexOfWidget);
    }

    void FormulaSideBar::updateAt(size_t index) {
        checkFormulaWidgetsParsed();
        checkForRedeclarations();
        //        checkReferenceToUndefined();
        checkCircularDependenciesAndUndefined();

        for (auto& formulaWidget : m_formulaWidgets) {
            formulaWidget->updateWidget();
        }
        emit sendUpdate();
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
        m_scrollArea = new QScrollArea(this);
        m_scrollArea->setWidgetResizable(true);
        auto* widget = new QWidget();
        widget->setLayout(m_layout);
        m_scrollArea->setWidget(widget);
        auto* layout = new QHBoxLayout(this);
        layout->addWidget(m_scrollArea);
        setLayout(layout);
    }

    void FormulaSideBar::checkForRedeclarations() {
        // TODO
        //        for (auto it = m_formulaWidgets.begin(); it != m_formulaWidgets.end(); ++it) {
        //            if (not(*it)->success()) {
        //                continue;
        //            }
        //            auto it2 = std::find_if(m_formulaWidgets.begin(), m_formulaWidgets.end(), [&](const auto& a) {
        //                return a->formula() && (*it)->formula()->formulaHeader().name() == a->formula()->formulaHeader().name();
        //            });
        //            if (it2 != it) {
        //                (*it)->setError("Multiple declaration of " + (*it)->formula()->formulaHeader().name());
        //                (*it2)->setError("Multiple declaration of " + (*it)->formula()->formulaHeader().name());
        //            }
        //        }
    }

    void FormulaSideBar::checkFormulaWidgetsParsed() {
        // TODO
        //        for (auto& formulaWidget : m_formulaWidgets) {
        //            if (not formulaWidget->formula()) {
        //                formulaWidget->setError("");
        //            } else if (not formulaWidget->formula()->success()) {
        //                formulaWidget->setError(formulaWidget->formula()->errorString());
        //            } else {
        //                formulaWidget->setSuccess(true);
        //            }
        //        }
    }

    void FormulaSideBar::checkReferenceToUndefined() {
        // TODO
        //        std::set<fml::FunctionSignature> declaredFunctions;
        //        for (const auto& formulaWidget : m_formulaWidgets) {
        //            if (formulaWidget->success()) {
        //                declaredFunctions.insert(formulaWidget->formula()->getSignature());
        //            }
        //        }
        //        for (const auto& formulaWidget : m_formulaWidgets) {
        //            if (not formulaWidget->success()) {
        //                continue;
        //            }
        //            const auto&                      dependencies = formulaWidget->formula()->ast().functionDependencies();
        //            std::set<fml::FunctionSignature> undefined;
        //            std::set_difference(dependencies.begin(), dependencies.end(), declaredFunctions.begin(), declaredFunctions.end(), std::inserter(undefined,
        //            undefined.begin())); if (undefined.empty()) {
        //                continue;
        //            }
        //            std::string errorMessage = "Undefined functions: ";
        //            for (const auto& el : undefined) {
        //                errorMessage += el.functionName() + "(" + std::to_string(el.argumentCount()) + ") ";
        //            }
        //            formulaWidget->setError(errorMessage);
        //        }
    }

    void FormulaSideBar::checkCircularDependenciesAndUndefined() {
        // TODO
        //        gen::DependencyGraph             g;
        //        std::set<fml::FunctionSignature> declaredFunctions;
        //        for (const auto& formulaWidget : m_formulaWidgets) {
        //            if (not formulaWidget->success()) {
        //                continue;
        //            }
        //            const auto currentName = formulaWidget->formula()->formulaHeader().name();
        //        }
    }
} // namespace app
