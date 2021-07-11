//
// Created by pc on 28-05-21.
//

#include "TextEditChangedCommand.h"

#include "../TextEdit.h"
#include "../UndoRedoHandler.h"

namespace app::cmd {

    static void S_UPDATE_TEXT_EDIT(app::TextEdit* textEdit, const QString& string, int cursorPosition, int anchorPosition) {
        UndoRedoHandler::SET_PUSH_BLOCKED(true);
        textEdit->setText(string);
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition(anchorPosition);
        cursor.setPosition(cursorPosition, QTextCursor::KeepAnchor);
        textEdit->setTextCursor(cursor);
        textEdit->setFocus();
        UndoRedoHandler::SET_PUSH_BLOCKED(false);
    }

    TextEditChangedCommand::TextEditChangedCommand(app::TextEdit* textEdit, QString oldString, const OldCursor& oldCursor)
        : m_textEdit(textEdit),
          m_oldString(std::move(oldString)),
          m_newString(textEdit->toPlainText()),
          m_oldCursorPosition(oldCursor.position()),
          m_newCursorPosition(textEdit->textCursor().position()),
          m_oldAnchorPosition(oldCursor.anchor()),
          m_newAnchorPosition(textEdit->textCursor().anchor()) {
    }

    void TextEditChangedCommand::undo() {
        S_UPDATE_TEXT_EDIT(m_textEdit, m_oldString, m_oldCursorPosition, m_oldAnchorPosition);
    }

    void TextEditChangedCommand::redo() {
        S_UPDATE_TEXT_EDIT(m_textEdit, m_newString, m_newCursorPosition, m_newAnchorPosition);
    }
} // namespace app::cmd
