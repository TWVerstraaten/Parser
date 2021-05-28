//
// Created by pc on 28-05-21.
//

#include "SetFormulaTextCommand.h"

SetFormulaTextCommand::SetFormulaTextCommand(const size_t indexToFormula, const QString& oldString, const QString& newString)
    : m_indexToFormula(indexToFormula), m_oldString(oldString), m_newString(newString) {
}
