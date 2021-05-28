//
// Created by pc on 28-05-21.
//

#ifndef PARSER_SETFORMULATEXTCOMMAND_H
#define PARSER_SETFORMULATEXTCOMMAND_H

#include <QUndoCommand>

class SetFormulaTextCommand : public QUndoCommand {

  public:
    SetFormulaTextCommand(const size_t indexToFormula, const QString& oldString, const QString& newString);

  private:
    const size_t  m_indexToFormula;
    const QString m_oldString;
    const QString m_newString;
};

#endif // PARSER_SETFORMULATEXTCOMMAND_H
