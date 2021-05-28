//
// Created by pc on 28-05-21.
//

#include "FormulaChangedCommand.h"

#include <QDebug>
#include <QLineEdit>

FormulaChangedCommand::FormulaChangedCommand(FormulaWidget* formula, QString old, QString newString)
    : m_formula(formula), m_old(std::move(old)), m_new(std::move(newString)) {
    qDebug() << m_old << " to " << m_new << '\n';
}

void FormulaChangedCommand::undo() {
    m_formula->m_lineEdit->setText(m_old);
}

void FormulaChangedCommand::redo() {
    m_formula->m_lineEdit->setText(m_new);
}
