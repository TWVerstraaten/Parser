//
// Created by pc on 28-05-21.
//

#ifndef APP_FORMULASIDEBAR_H
#define APP_FORMULASIDEBAR_H

#include <QWidget>
#include <set>
#include <vector>

class QVBoxLayout;
class QLabel;
class QScrollArea;
class QPushButton;

namespace ast::par {
    class CustomFunctionToken;
}

namespace app {

    namespace cmd {
        class RemoveFormulaWidgetCommand;
    }

    class FormulaWidget;

    class FormulaSideBar : public QWidget {
        Q_OBJECT

        friend class cmd::RemoveFormulaWidgetCommand;

      public:
        explicit FormulaSideBar(QWidget* parent);

        [[nodiscard]] FormulaWidget*                     fromIndex(size_t index);
        [[nodiscard]] const std::vector<FormulaWidget*>& formulaWidgets() const;

      signals:
        void sendUpdate();

      private slots:
        void removeFormulaWidget(size_t indexOfWidget);
        void addNewFormulaWidget();
        void checkRedeclarations();

      private:
        void checkFormulaWidgetsParsed();
        void checkUndefined();
        void checkCircularDependenciesAndUndefined();
        void updateAt(size_t index);
        void addNewFormulaWidgetButton();
        void makeScrollArea();
        void writeInfoToInfoLabel();
        void clearDefinitionErrors();

        [[nodiscard]] std::vector<ast::par::CustomFunctionToken> getDeclaredFunctions() const;
        [[nodiscard]] std::set<ast::par::CustomFunctionToken>    getReferencedFunctions() const;
        [[nodiscard]] std::vector<std::string>                   getDeclaredConstants() const;
        [[nodiscard]] std::set<std::string>                      getReferencedConstants() const;

        std::vector<FormulaWidget*> m_formulaWidgets;
        QPushButton*                m_newFormulaPushButton = nullptr;
        QScrollArea*                m_scrollArea           = nullptr;
        QVBoxLayout*                m_layout               = nullptr;
        QLabel*                     m_infoLabel            = nullptr;
        void                        updateAllTextEdits();
    };
} // namespace app

#endif // APP_FORMULASIDEBAR_H
