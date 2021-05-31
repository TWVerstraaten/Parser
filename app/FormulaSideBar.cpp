//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

#include "FormulaWidget.h"
#include "UndoRedoConsumer.h"
#include "UndoRedoHandler.h"
#include "cmd/NewFormulaWidgetCommand.h"
#include "cmd/RemoveFormulaWidgetCommand.h"

#include <QVBoxLayout>

namespace app {
    FormulaSideBar::FormulaSideBar(QWidget* parent) : QWidget(parent) {
        m_layout = new QVBoxLayout(this);

        m_newFormulaPushButton = new QPushButton(this);
        m_newFormulaPushButton->setText("+");
        m_layout->addWidget(m_newFormulaPushButton);

        connect(m_newFormulaPushButton, &QPushButton::clicked, this, &FormulaSideBar::addNewFormulaWidget);

        UndoRedoHandler::setPushBlocked(true);
        addNewFormulaWidget();
        addNewFormulaWidget();
        UndoRedoHandler::setPushBlocked(false);

        m_layout->setAlignment(Qt::AlignTop);
        setLayout(m_layout);
        installEventFilter(UndoRedoConsumer::undoRedoConsumer());
    }

    void FormulaSideBar::addNewFormulaWidget() {

        layout()->removeWidget(m_newFormulaPushButton);
        auto* newFormulaWidget = new FormulaWidget(this);
        m_formulaWidgets.push_back(newFormulaWidget);
        UndoRedoHandler::push(new cmd::NewFormulaWidgetCommand(this, newFormulaWidget->index()));
        connect(newFormulaWidget, &FormulaWidget::deleteClicked, this, &FormulaSideBar::RemoveFormulaWidget);
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
} // namespace app
