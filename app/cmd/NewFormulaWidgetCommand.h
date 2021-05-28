//
// Created by pc on 28-05-21.
//

#ifndef PARSER_NEWFORMULAWIDGETCOMMAND_H
#define PARSER_NEWFORMULAWIDGETCOMMAND_H

#include "../FormulaSideBar.h"

#include <QUndoCommand>

namespace app::cmd {
        class NewFormulaWidgetCommand : public QUndoCommand {

          public:
            NewFormulaWidgetCommand(app::FormulaSideBar* formulaSideBar, size_t index);

            void undo() override;
            void redo() override;

          private:
            app::FormulaSideBar* m_formulaSideBar;
            size_t               m_indexOfNewFormulaWidget;
        };
    }
#endif // PARSER_NEWFORMULAWIDGETCOMMAND_H
