//
// Created by pc on 28-05-21.
//

#ifndef PARSER_REMOVEFORMULAWIDGETCOMMAND_H
#define PARSER_REMOVEFORMULAWIDGETCOMMAND_H

#include "../FormulaSideBar.h"

#include <QUndoCommand>

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
    } // namespace app
#endif // PARSER_REMOVEFORMULAWIDGETCOMMAND_H
