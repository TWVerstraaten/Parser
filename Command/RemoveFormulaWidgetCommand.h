//
// Created by pc on 28-05-21.
//

#ifndef PARSER_REMOVEFORMULAWIDGETCOMMAND_H
#define PARSER_REMOVEFORMULAWIDGETCOMMAND_H

#include "../App/FormulaSideBar.h"

#include <QUndoCommand>

class RemoveFormulaWidgetCommand : public QUndoCommand {

  public:
    RemoveFormulaWidgetCommand(FormulaSideBar* formulaSideBar, size_t index);

    void undo() override;
    void redo() override;

  private:
    FormulaSideBar* m_formulaSideBar;
    size_t          m_indexOfRemovedFormulaWidget;
};

#endif // PARSER_REMOVEFORMULAWIDGETCOMMAND_H
