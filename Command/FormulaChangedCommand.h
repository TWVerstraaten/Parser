//
// Created by pc on 28-05-21.
//

#ifndef PARSER_FORMULACHANGEDCOMMAND_H
#define PARSER_FORMULACHANGEDCOMMAND_H

#include "../App/FormulaWidget.h"

#include <QString>
#include <QUndoCommand>

class FormulaChangedCommand : public QUndoCommand {
  public:
    FormulaChangedCommand(FormulaWidget* formula, QString old, QString newString);

    void undo() override;
    void redo() override;

  private:
    FormulaWidget* m_formula;
    QString  m_old;
    QString  m_new;
};

#endif // PARSER_FORMULACHANGEDCOMMAND_H
