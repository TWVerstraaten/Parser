//
// Created by pc on 28-05-21.
//

#ifndef PARSER_FORMULASIDEBAR_H
#define PARSER_FORMULASIDEBAR_H

#include "FormulaWidget.h"

#include <QWidget>
#include <vector>

class QVBoxLayout;

class FormulaSideBar : public QWidget {
    Q_OBJECT

  public:
    explicit FormulaSideBar(QWidget* parent = nullptr);

  private slots:
    void RemoveFormulaWidget(size_t indexOfWidget);

  private:
    friend class NewFormulaWidgetCommand;
    friend class RemoveFormulaWidgetCommand;

    void addNewFormulaWidget();

    FormulaWidget* fromIndex(size_t index);

    std::vector<FormulaWidget*> m_formulaWidgets;
    std::vector<FormulaWidget*> m_stashedWidgets;
    QVBoxLayout*                m_layout;
};

#endif // PARSER_FORMULASIDEBAR_H
