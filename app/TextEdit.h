//
// Created by pc on 10-07-21.
//

#ifndef APP_TEXTEDIT_H
#define APP_TEXTEDIT_H

#include "../ast/err/ParserInfo.h"
#include "OldCursor.h"

#include <QTextEdit>

namespace app {

    class TextEdit : public QTextEdit {
        Q_OBJECT

      public:
        explicit TextEdit(QWidget* parent);

        void setInfo(const ast::err::ParserInfo& info);

      protected:
        void mouseMoveEvent(QMouseEvent* e) override;

      private:
        void highlightInfo();
        void determineAndSetTextColor();
        void sendToUndoRedoHandler();

        ast::err::ParserInfo m_info;
        QString              m_oldString = "";
        OldCursor            m_oldCursor;
    };
} // namespace app
#endif // APP_TEXTEDIT_H
