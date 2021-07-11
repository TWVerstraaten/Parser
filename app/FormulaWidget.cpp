//
// Created by pc on 09-04-21.
//

#include "FormulaWidget.h"

#include "../ast/Ast.h"
#include "../ast/par/Parser.h"
#include "TextEdit.h"
#include "cmd/TextEditChangedCommand.h"

#include <QCheckBox>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QToolTip>

namespace app {

    FormulaWidget::FormulaWidget(QWidget* parent) : QWidget(parent), m_index(S_MAX_INDEX) {
        ++S_MAX_INDEX;
        m_success = false;

        initPointers();
        initButtons();
        initLayout();
        connectSignals();

        m_layout->setSpacing(0);
        m_layout->setAlignment(Qt::AlignTop);
    }

    void FormulaWidget::processFormula() {
        const auto string = m_textEdit->document()->toPlainText();

        m_textEdit->blockSignals(true);
        QTextCursor     cursor(m_textEdit->document());
        QTextCharFormat fmt;
        cursor.setPosition(0, QTextCursor::MoveAnchor);
        cursor.setPosition(string.length(), QTextCursor::KeepAnchor);
        cursor.setCharFormat(fmt);
        m_textEdit->blockSignals(false);

        const auto str = string.toStdString();
        ast::Ast   ast(str);
        auto       info = ast.info();
        m_textEdit->setInfo(std::move(info));

        // TODO
        //        m_textEdit           = std::make_unique<fml::Formula>(string.toStdString());
        m_formulaWasUpdated = true;

        emit updated(m_index);
    }

    void FormulaWidget::handleCorrectFormula() {
        QToolTip::hideText();

        // TODO

        //        if (const auto hint = m_textEdit->getHints(); not hint.empty()) {
        //            //            showToolTipAtLineEdit(251, QString::fromStdString(hint));
        //            m_errorMessageLabel->setText(QString::fromStdString(hint));
        //            m_errorMessageLabel->setStyleSheet("QLabel { color : rgb(0,0,251); }");
        //        } else {
        //            m_errorMessageLabel->setText("");
        //        }
        update();
    }

    void FormulaWidget::handleWrongFormula() {
        update();
    }

    size_t FormulaWidget::index() const {
        return m_index;
    }

    void FormulaWidget::initPointers() {
        m_textEdit = new TextEdit(this);
        m_layout   = new QGridLayout(this);

        m_activeCheckBox = new QCheckBox(this);
        m_activeCheckBox->setChecked(true);

        m_deleteButton = new QPushButton{this};
        m_deleteButton->setFixedHeight(m_textEdit->height());
    }

    void FormulaWidget::initButtons() {
        m_deleteButton->setText("x");
        m_deleteButton->setFixedSize(20, 20);
    }

    void FormulaWidget::initLayout() {
        m_layout->addWidget(m_activeCheckBox, 0, 0);
        m_layout->addWidget(m_textEdit, 0, 1);
        m_layout->addWidget(m_deleteButton, 0, 2);
        m_layout->setContentsMargins(3, 0, 0, 3);
    }

    void FormulaWidget::connectSignals() {
        connect(m_textEdit, &QTextEdit::textChanged, this, &FormulaWidget::processFormula);
        connect(m_deleteButton, &QPushButton::clicked, [this]() { emit deleteClicked(m_index); });
        connect(m_activeCheckBox, &QCheckBox::toggled, [this]() { emit updated(m_index); });
    }

    TextEdit* FormulaWidget::textEdit() {
        return m_textEdit;
    }

    void FormulaWidget::updateWidget() {
        if (m_success) {
            handleCorrectFormula();
        } else {
            handleWrongFormula();
        }
        update();
    }

    bool FormulaWidget::isActive() const {
        return m_activeCheckBox->isChecked();
    }

    bool FormulaWidget::formulaWasUpdated() const {
        return m_formulaWasUpdated;
    }

    void FormulaWidget::setFormulaWasUpdated(bool formulaWasUpdated) {
        m_formulaWasUpdated = formulaWasUpdated;
    }

} // namespace app