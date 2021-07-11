//
// Created by pc on 28-05-21.
//

#ifndef APP_CMD_REMOVEFORMULAWIDGETCOMMAND_H
#define APP_CMD_REMOVEFORMULAWIDGETCOMMAND_H

#include <QUndoCommand>

namespace app {
    class FormulaSideBar;
}

namespace app::cmd {
    class RemoveFormulaWidgetCommand : public QUndoCommand {

      public:
        RemoveFormulaWidgetCommand(app::FormulaSideBar* formulaSideBar, size_t index);

        void undo() override;
        void redo() override;

      private:
        app::FormulaSideBar* m_formulaSideBar;
        size_t               m_indexOfRemovedFormulaWidget;
    };
} // namespace app::cmd
#endif // APP_CMD_REMOVEFORMULAWIDGETCOMMAND_H
