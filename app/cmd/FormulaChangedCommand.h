//
// Created by pc on 28-05-21.
//

#ifndef PARSER_FORMULACHANGEDCOMMAND_H
#define PARSER_FORMULACHANGEDCOMMAND_H

#include "../FormulaWidget.h"

#include <QString>
#include <QUndoCommand>

namespace app::cmd {
        class FormulaChangedCommand : public QUndoCommand {
          public:
            FormulaChangedCommand(app::FormulaWidget* formula, QString old, QString newString);

            void undo() override;
            void redo() override;

          private:
            app::FormulaWidget* m_formula;
            QString             m_old;
            QString             m_new;
        };
    } // namespace app
#endif // PARSER_FORMULACHANGEDCOMMAND_H
