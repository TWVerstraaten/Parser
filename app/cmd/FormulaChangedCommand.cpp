//
// Created by pc on 28-05-21.
//

#include "FormulaChangedCommand.h"

#include <QLineEdit>

namespace app::cmd {
    FormulaChangedCommand::FormulaChangedCommand(app::FormulaWidget* formula, QString old, QString newString)
        : m_formula(formula), m_old(std::move(old)), m_new(std::move(newString)) {
    }

    void FormulaChangedCommand::undo() {
        m_formula->lineEdit()->setText(m_old);
    }

    void FormulaChangedCommand::redo() {
        m_formula->lineEdit()->setText(m_new);
    }
} // namespace app::cmd