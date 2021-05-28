//
// Created by pc on 28-05-21.
//

#ifndef PARSER_NEWFORMULAWIDGETCOMMAND_H
#define PARSER_NEWFORMULAWIDGETCOMMAND_H

#include "../App/FormulaSideBar.h"

#include <QUndoCommand>

class NewFormulaWidgetCommand : public QUndoCommand {

  public:
    NewFormulaWidgetCommand(FormulaSideBar* formulaSideBar, size_t index);

    void undo() override;
    void redo() override;

  private:
    FormulaSideBar* m_formulaSideBar;
    size_t          m_indexOfNewFormulaWidget;
};

#endif // PARSER_NEWFORMULAWIDGETCOMMAND_H
