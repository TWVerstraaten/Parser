//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

#include "../Command/NewFormulaWidgetCommand.h"
#include "../Command/RemoveFormulaWidgetCommand.h"
#include "UndoRedoConsumer.h"
#include "UndoRedoHandler.h"

#include <QVBoxLayout>

FormulaSideBar::FormulaSideBar(QWidget* parent) : QWidget(parent) {

    installEventFilter(UndoRedoConsumer::undoRedoConsumer());

    m_layout = new QVBoxLayout(this);

    UndoRedoHandler::setPushBlocked(true);
    addNewFormulaWidget();
    addNewFormulaWidget();
    UndoRedoHandler::setPushBlocked(false);

    m_layout->setAlignment(Qt::AlignTop);
    setLayout(m_layout);
}

void FormulaSideBar::addNewFormulaWidget() {
    auto* newFormulaWidget = new FormulaWidget(this);
    m_formulaWidgets.push_back(newFormulaWidget);
    UndoRedoHandler::push(new NewFormulaWidgetCommand(this, newFormulaWidget->index()));
    connect(newFormulaWidget, &FormulaWidget::deleteClicked, this, &FormulaSideBar::RemoveFormulaWidget);
    m_layout->addWidget(m_formulaWidgets.back());
}

FormulaWidget* FormulaSideBar::fromIndex(size_t index) {
    return *std::find_if(m_formulaWidgets.begin(), m_formulaWidgets.end(),
                         [index](FormulaWidget* widget) { return widget->index() == index; });
}

void FormulaSideBar::RemoveFormulaWidget(size_t indexOfWidget) {
    UndoRedoHandler::push(new RemoveFormulaWidgetCommand(this, indexOfWidget));
}
