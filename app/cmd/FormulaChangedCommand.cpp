//
// Created by pc on 28-05-21.
//

#include "FormulaChangedCommand.h"

#include "../FormulaWidget.h"
#include "../UndoRedoHandler.h"

#include <QLineEdit>

namespace app::cmd {
    FormulaChangedCommand::FormulaChangedCommand(app::FormulaWidget* formula, QString old, QString newString)
        : m_formula(formula), m_old(std::move(old)), m_new(std::move(newString)) {
    }

    void FormulaChangedCommand::undo() {
        UndoRedoHandler::setPushBlocked(true);
        m_formula->lineEdit()->setText(m_old);
        m_formula->processFormula();
        UndoRedoHandler::setPushBlocked(false);
    }

    void FormulaChangedCommand::redo() {
        UndoRedoHandler::setPushBlocked(true);
        m_formula->lineEdit()->setText(m_new);
        m_formula->processFormula();
        UndoRedoHandler::setPushBlocked(false);
    }
} // namespace app::cmd