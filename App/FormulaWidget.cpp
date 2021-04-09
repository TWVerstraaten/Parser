//
// Created by pc on 09-04-21.
//

#include "FormulaWidget.h"

FormulaWidget::FormulaWidget(QWidget* parent) : QWidget(parent) {
    m_layout = new QGridLayout(this);

    m_activeCheckBox  = new QCheckBox(this);
    m_lineEdit        = new QLineEdit(this);
    m_variableLabel   = new QLabel("_", this);
    m_messageLabel    = new QLabel("_", this);
    m_simplifiedLabel = new QLabel("_", this);

    connect(m_lineEdit, &QLineEdit::textChanged, this, &FormulaWidget::setAst);

    m_layout->addWidget(m_activeCheckBox, 0, 0);
    m_layout->addWidget(m_lineEdit, 0, 1);
    m_layout->addWidget(new QLabel("Var:"), 1, 0);
    m_layout->addWidget(m_variableLabel, 1, 1);
    m_layout->addWidget(new QLabel("Spf:"), 2, 0);
    m_layout->addWidget(m_simplifiedLabel, 2, 1);
    m_layout->addWidget(new QLabel("Msg:"), 3, 0);
    m_layout->addWidget(m_messageLabel, 3, 1);

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void FormulaWidget::setAst(const QString& string) {
    std::string functionString = string.toStdString();
    try {
        m_ast = std::make_unique<Ast>(functionString);
        handleCorrectFormula();
    } catch (const std::runtime_error& error) { handleWrongFormula(error); }
}

static void setTextColor(QWidget* widget, const QColor& color) {
    auto* palette = new QPalette();
    palette->setColor(QPalette::Text, color);
    widget->setPalette(*palette);
}

void FormulaWidget::setTextColor(const QColor& color) {
    ::setTextColor(m_lineEdit, color);
    ::setTextColor(m_variableLabel, color);
    ::setTextColor(m_messageLabel, color);
}

void FormulaWidget::handleCorrectFormula() {
    setTextColor(Qt::black);
    const auto variableSet = m_ast->variables();
    if (variableSet.empty()) {
        m_variableLabel->setText("_");
    } else {
        QString variableString = "<font color=\"green\">";
        for (const auto& var : variableSet) {
            variableString += QString::fromStdString(var) + ",";
        }
        variableString.chop(1);
        m_variableLabel->setText(variableString);
        m_messageLabel->setText("_");
    }
    m_simplifiedLabel->setText(QString::fromStdString(m_ast->simplify().toString()));
}

void FormulaWidget::handleWrongFormula(const std::runtime_error& error) {
    setTextColor(Qt::red);
    m_variableLabel->setText("_");
    m_messageLabel->setText(error.what());
}
