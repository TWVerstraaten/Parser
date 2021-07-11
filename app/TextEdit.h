//
// Created by pc on 10-07-21.
//

#ifndef APP_TEXTEDIT_H
#define APP_TEXTEDIT_H

#include "../ast/err/ParserInfo.h"

#include <QTextEdit>

namespace app {

    class TextEdit : public QTextEdit {

        Q_OBJECT

      public:
        explicit TextEdit(QWidget* parent);

        void setInfo(ast::err::ParserInfo&& info);

      protected:
        void mouseMoveEvent(QMouseEvent* e) override;

      private:
        void highlightInfo();
        void determineAndSetTextColor();
        void sendToUndoRedoHandler();

        ast::err::ParserInfo m_info;
        QString              m_oldString            = "";
        int                  m_oldCursorPosition    = 0;
        int                  m_penUltCursorPosition = 0;
        int                  m_oldAnchorPosition    = 0;
        int                  m_penUltAnchorPosition = 0;
    };
} // namespace app
#endif // APP_TEXTEDIT_H
