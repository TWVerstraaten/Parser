//
// Created by pc on 28-05-21.
//

#ifndef APP_FORMULASIDEBAR_H
#define APP_FORMULASIDEBAR_H

#include "../ast/AstManager.h"

#include <QWidget>
#include <memory>
#include <set>
#include <vector>

class QVBoxLayout;
class QLabel;
class QScrollArea;
class QPushButton;

namespace ast::par {
    class FunctionToken;
}

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

        [[nodiscard]] std::vector<ast::par::FunctionToken> getDeclaredFunctions() const;
        [[nodiscard]] std::set<ast::par::FunctionToken>    getReferencedFunctions() const;
        [[nodiscard]] std::vector<std::string>                   getDeclaredConstants() const;
        [[nodiscard]] std::set<std::string>                      getReferencedConstants() const;

        std::vector<FormulaWidget*> m_formulaWidgets;
        ast::AstManager             m_astManager;
        QPushButton*                m_newFormulaPushButton = nullptr;
        QScrollArea*                m_scrollArea           = nullptr;
        QVBoxLayout*                m_layout               = nullptr;
        QLabel*                     m_infoLabel            = nullptr;
    };
} // namespace app

#endif // APP_FORMULASIDEBAR_H
