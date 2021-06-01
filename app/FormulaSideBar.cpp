//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

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
        m_layout     = new QVBoxLayout();
        m_scrollArea = new QScrollArea(this);
        auto* widget = new QWidget();
        m_scrollArea->setWidgetResizable(true);
        widget->setLayout(m_layout);
        m_scrollArea->setWidget(widget);
        auto* layout = new QHBoxLayout(this);
        layout->addWidget(m_scrollArea);
        setLayout(layout);

        m_newFormulaPushButton = new QPushButton(m_scrollArea->widget());
        m_newFormulaPushButton->setText("+");
        connect(m_newFormulaPushButton, &QPushButton::clicked, this, &FormulaSideBar::addNewFormulaWidget);
        m_layout->addWidget(m_newFormulaPushButton);
        m_layout->setAlignment(Qt::AlignTop);

        UndoRedoHandler::setPushBlocked(true);
        addNewFormulaWidget();
        addNewFormulaWidget();
        UndoRedoHandler::setPushBlocked(false);

        setBaseSize({300, 800});
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
        return *std::find_if(m_formulaWidgets.begin(), m_formulaWidgets.end(),
                             [index](FormulaWidget* widget) { return widget->index() == index; });
    }

    void FormulaSideBar::RemoveFormulaWidget(size_t indexOfWidget) {
        UndoRedoHandler::push(new cmd::RemoveFormulaWidgetCommand(this, indexOfWidget));
    }

    void FormulaSideBar::updateAt(size_t index) {
        std::cout << index << '\n';

        emit sendUpdate();
    }

    const std::vector<FormulaWidget*>& FormulaSideBar::formulaWidgets() const {
        return m_formulaWidgets;
    }

} // namespace app
