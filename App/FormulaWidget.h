//
// Created by pc on 09-04-21.
//

#ifndef PARSER_FORMULAWIDGET_H
#define PARSER_FORMULAWIDGET_H

#include "../Parser/Ast.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class FormulaWidget : public QWidget {
    Q_OBJECT

  public:
    explicit FormulaWidget(QWidget* parent = nullptr);

  private slots:
    void setAst(const QString& string);

  private:
    void setTextColor(const QColor& color);
    void handleCorrectFormula();
    void handleWrongFormula(const std::string& errorMessage);

    QGridLayout* m_layout          = nullptr;
    QCheckBox*   m_activeCheckBox  = nullptr;
    QLineEdit*   m_lineEdit        = nullptr;
    QLabel*      m_variableLabel   = nullptr;
    QLabel*      m_messageLabel    = nullptr;
    QLabel*      m_simplifiedLabel = nullptr;

    std::unique_ptr<Ast> m_ast;
};

#endif // PARSER_FORMULAWIDGET_H
