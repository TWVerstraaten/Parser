//
// Created by pc on 09-04-21.
//

#include "FormulaWidget.h"

#include "../ast/Ast.h"
#include "../ast/par/Parser.h"
#include "TextEdit.h"
#include "cmd/TextEditChangedCommand.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>

namespace app {

    FormulaWidget::FormulaWidget(QWidget* parent) : QWidget(parent), m_index(M_MAX_INDEX) {
        ++M_MAX_INDEX;

        initPointers();
        initButtons();
        initLayout();
        connectSignals();

        m_layout->setSpacing(0);
        m_layout->setAlignment(Qt::AlignTop);
    }

    FormulaWidget::~FormulaWidget() = default;

    void FormulaWidget::processFormula() {
        const auto string = m_textEdit->document()->toPlainText();

        m_textEdit->blockSignals(true);
        QTextCursor     cursor(m_textEdit->document());
        QTextCharFormat fmt;
        cursor.setPosition(0, QTextCursor::MoveAnchor);
        cursor.setPosition(string.length(), QTextCursor::KeepAnchor);
        cursor.setCharFormat(fmt);
        m_textEdit->blockSignals(false);

        auto cString = string.toStdString();
        m_ast        = std::make_unique<ast::Ast>(cString);

        emit updated(m_index);
        m_textEdit->setInfo(m_ast->info());
        m_formulaWasUpdated = true;
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

    bool FormulaWidget::isActive() const {
        return m_activeCheckBox->isChecked();
    }

    const ast::Ast& FormulaWidget::ast() const {
        return *m_ast;
    }

    bool FormulaWidget::hasAst() const {
        return m_ast != nullptr;
    }

    void FormulaWidget::updateTextEdit() {
        m_textEdit->setInfo(m_ast->info());
    }

    bool FormulaWidget::hasSuccessfulNonEmptyAst() const {
        return m_ast != nullptr && m_ast->success() && (not m_ast->isEmpty());
    }

} // namespace app