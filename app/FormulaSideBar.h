//
// Created by pc on 28-05-21.
//

#ifndef PARSER_FORMULASIDEBAR_H
#define PARSER_FORMULASIDEBAR_H

#include <QPushButton>
#include <QWidget>
#include <vector>

class QVBoxLayout;

namespace app {
    class FormulaWidget;

    class FormulaSideBar : public QWidget {
        Q_OBJECT

      public:
        explicit FormulaSideBar(QWidget* parent = nullptr);

        FormulaWidget* fromIndex(size_t index);

      private slots:
        void RemoveFormulaWidget(size_t indexOfWidget);
        void addNewFormulaWidget();

      private:
        std::vector<FormulaWidget*> m_formulaWidgets;
        QVBoxLayout*                m_layout;
        QPushButton*                m_newFormulaPushButton;
    };
} // namespace app

#endif // PARSER_FORMULASIDEBAR_H
