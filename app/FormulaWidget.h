//
// Created by pc on 09-04-21.
//

#ifndef APP_FORMULAWIDGET_H
#define APP_FORMULAWIDGET_H

#include "../ast/Ast.h"
#include "../gen/ErrorBase.h"

#include <QWidget>

class QCheckBox;
class QGridLayout;
class QLabel;
class QPushButton;
class TextEdit;

namespace ast::err {
    class ParserInfo;
}

namespace app {
    namespace cmd {
        class FormulaChangedCommand;
    }

    class FormulaWidget : public QWidget, public gen::ErrorBase {
        Q_OBJECT

        friend class cmd::FormulaChangedCommand;

      public:
        explicit FormulaWidget(QWidget* parent = nullptr);

        void updateWidget();

        [[nodiscard]] size_t    index() const;
        [[nodiscard]] TextEdit* textEdit();
        //        [[nodiscard]] const fml::Formula* formula() const;
        [[nodiscard]] bool isActive() const;
        [[nodiscard]] bool formulaWasUpdated() const;
        void               setFormulaWasUpdated(bool formulaWasUpdated);

      signals:
        void deleteClicked(size_t index);
        void updated(size_t index);

      private slots:
        void processFormula();

      private:
        void handleCorrectFormula();
        void handleWrongFormula();
        void initPointers();
        void initButtons();
        void initTextEdit();
        void initLayout();
        void connectSignals();

        static inline size_t S_MAX_INDEX = 0;

        const size_t              m_index;
        bool                      m_formulaWasUpdated = false;
        QGridLayout*              m_layout            = nullptr;
        QCheckBox*                m_activeCheckBox    = nullptr;
        TextEdit*                 m_textEdit          = nullptr;
        QPushButton*              m_deleteButton      = nullptr;
        QString                   m_oldFormula        = "";
        std::unique_ptr<ast::Ast> m_ast;
    };
} // namespace app

#endif // APP_FORMULAWIDGET_H
