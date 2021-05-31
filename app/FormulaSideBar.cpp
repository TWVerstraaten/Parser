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

namespace app {
    FormulaSideBar::FormulaSideBar(QWidget* parent) : QWidget(parent) {
        m_scrollArea = new QScrollArea(this);
        m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        m_scrollArea->setWidgetResizable(true);

        auto* widget = new QWidget();
        m_scrollArea->setWidget(widget);

        m_layout = new QVBoxLayout();
        widget->setLayout(m_layout);

        m_newFormulaPushButton = new QPushButton(m_scrollArea->widget());
        m_newFormulaPushButton->setText("+");
        m_layout->addWidget(m_newFormulaPushButton);

        connect(m_newFormulaPushButton, &QPushButton::clicked, this, &FormulaSideBar::addNewFormulaWidget);

        UndoRedoHandler::setPushBlocked(true);
        addNewFormulaWidget();
        addNewFormulaWidget();
        UndoRedoHandler::setPushBlocked(false);

        m_layout->setAlignment(Qt::AlignTop);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        auto* layout = new QHBoxLayout(this);
        layout->addWidget(m_scrollArea);
        setLayout(layout);
        installEventFilter(UndoRedoConsumer::undoRedoConsumer());

        //        setMinimumWidth(300);
    }

    void FormulaSideBar::addNewFormulaWidget() {
        auto* newFormulaWidget = new FormulaWidget(m_scrollArea->widget());
        connect(newFormulaWidget, &FormulaWidget::updated, [this]() { emit updated(); });

        m_formulaWidgets.push_back(newFormulaWidget);
        UndoRedoHandler::push(new cmd::NewFormulaWidgetCommand(this, newFormulaWidget->index()));
        connect(newFormulaWidget, &FormulaWidget::deleteClicked, this, &FormulaSideBar::RemoveFormulaWidget);

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

    QSize FormulaSideBar::sizeHint() const {
        return {300, 1000};
    }
} // namespace app