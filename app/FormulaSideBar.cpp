//
// Created by pc on 28-05-21.
//

#include "FormulaSideBar.h"

#include "../alg/StringAlg.h"
#include "FormulaWidget.h"
#include "UndoRedoHandler.h"
#include "cmd/NewFormulaWidgetCommand.h"
#include "cmd/RemoveFormulaWidgetCommand.h"

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
    }

    void FormulaSideBar::updateAt(size_t index) {
        auto* widget = fromIndex(index);
        if (widget->isHidden() || not widget->hasSuccessfulNonEmptyAst()) {
            m_unrollManager.remove(index);
        } else {
            m_unrollManager.add(index, widget->ast());
        }
        m_unrollManager.unrollAll();
        writeInfoToInfoLabel();
    }

    void FormulaSideBar::writeInfoToInfoLabel() {
        QString info = "<b>Declared:</b><br>";
        for (const auto& formulaWidget : m_formulaWidgets) {
            if (formulaWidget->hasSuccessfulNonEmptyAst() && not formulaWidget->isHidden()) {
                const size_t index = formulaWidget->index();
                const auto   error = m_unrollManager.checkForDeclarationError(index);
                if (error.has_value()) {
                    info.append(QString::number(index) + ": " + QString::fromStdString(error.value().toString()) + "<br>");
                } else {
                    info.append(QString::number(index) + ": " + QString::fromStdString(m_unrollManager.getUnrolledAt(index).toString()) + "<br>");
                }
            }
        }
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

} // namespace app
