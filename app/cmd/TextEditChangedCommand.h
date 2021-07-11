//
// Created by pc on 28-05-21.
//

#ifndef APP_CMD_TEXTEDITCHANGEDCOMMAND_H
#define APP_CMD_TEXTEDITCHANGEDCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace app {
    class TextEdit;
    class OldCursor;
} // namespace app

namespace app::cmd {
    class TextEditChangedCommand : public QUndoCommand {
      public:
        TextEditChangedCommand(app::TextEdit* textEdit, QString oldString, const OldCursor& oldCursor);

        void undo() override;
        void redo() override;

      private:
        app::TextEdit* m_textEdit;
        QString        m_oldString;
        QString        m_newString;
        int            m_oldCursorPosition;
        int            m_newCursorPosition;
        int            m_oldAnchorPosition;
        int            m_newAnchorPosition;
    };
} // namespace app::cmd
#endif // APP_CMD_TEXTEDITCHANGEDCOMMAND_H
