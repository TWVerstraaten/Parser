//
// Created by pc on 28-05-21.
//

#ifndef APP_FORMULASIDEBAR_H
#define APP_FORMULASIDEBAR_H

#include "../ast/UnrollManager.h"

#include <QWidget>
#include <memory>
#include <set>
#include <vector>

class QVBoxLayout;
class QLabel;
class QScrollArea;
class QPushButton;

namespace app {

    namespace cmd {
        class NewFormulaWidgetCommand;
    }

    class FormulaWidget;

    class FormulaSideBar : public QWidget {
        Q_OBJECT

        friend class cmd::NewFormulaWidgetCommand;

      public:
        explicit FormulaSideBar(QWidget* parent);

        [[nodiscard]] FormulaWidget*                     fromIndex(size_t index);
        [[nodiscard]] const std::vector<FormulaWidget*>& formulaWidgets() const;

      signals:
        void sendUpdate();

      private slots:
        void removeFormulaWidget(size_t indexOfWidget);
        void addNewFormulaWidget();

      private:
        void updateAt(size_t index);
        void addNewFormulaWidgetButton();
        void makeScrollArea();
        void writeInfoToInfoLabel();

        std::vector<FormulaWidget*> m_formulaWidgets;
        ast::UnrollManager          m_unrollManager;
        QPushButton*                m_newFormulaPushButton = nullptr;
        QScrollArea*                m_scrollArea           = nullptr;
        QVBoxLayout*                m_layout               = nullptr;
        QLabel*                     m_infoLabel            = nullptr;
    };
} // namespace app

#endif // APP_FORMULASIDEBAR_H
