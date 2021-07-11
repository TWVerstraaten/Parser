//
// Created by pc on 10-07-21.
//

#ifndef APP_TEXTEDIT_H
#define APP_TEXTEDIT_H

#include "../ast/err/ParserInfo.h"

#include <QTextEdit>

class TextEdit : public QTextEdit {
    Q_OBJECT
  public:
    explicit TextEdit(QWidget* parent = nullptr);

    void setInfo(ast::err::ParserInfo&& info);

  protected:
    void mouseMoveEvent(QMouseEvent* e) override;

  private:
    void highlightInfo();
    void determineAndSetTextColor();

    ast::err::ParserInfo m_info;
};

#endif // APP_TEXTEDIT_H
