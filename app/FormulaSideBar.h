//
// Created by pc on 28-05-21.
//

#ifndef PARSER_FORMULASIDEBAR_H
#define PARSER_FORMULASIDEBAR_H

#include <QWidget>
#include <vector>

class QVBoxLayout;
class QScrollArea;
class QPushButton;

namespace app {
    class FormulaWidget;

    class FormulaSideBar : public QWidget {
        Q_OBJECT

      public:
        explicit FormulaSideBar(QWidget* parent = nullptr);

        FormulaWidget* fromIndex(size_t index);

      signals:
        void updated();

      private slots:
        void RemoveFormulaWidget(size_t indexOfWidget);
        void addNewFormulaWidget();

      private:
        std::vector<FormulaWidget*> m_formulaWidgets;
        QPushButton*                m_newFormulaPushButton;
        QScrollArea*                m_scrollArea;
        QVBoxLayout*                m_layout;
    };
} // namespace app

#endif // PARSER_FORMULASIDEBAR_H
