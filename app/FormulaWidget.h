//
// Created by pc on 09-04-21.
//

#ifndef APP_FORMULAWIDGET_H
#define APP_FORMULAWIDGET_H

#include <QWidget>
#include <memory>

class QCheckBox;
class QGridLayout;
class QPushButton;

namespace ast {
    namespace err {
        class ParserInfo;
    }
    class Ast;
} // namespace ast

namespace app {
    class TextEdit;

    class FormulaWidget : public QWidget {
        Q_OBJECT

      public:
        explicit FormulaWidget(QWidget* parent);
        ~FormulaWidget() override;

        void setFormulaWasUpdated(bool formulaWasUpdated);
        void updateTextEdit();

        [[nodiscard]] size_t                index() const;
        [[nodiscard]] TextEdit*             textEdit();
        [[nodiscard]] bool                  isActive() const;
        [[nodiscard]] bool                  hasAst() const;
        [[nodiscard]] bool                  formulaWasUpdated() const;
        [[nodiscard]] const ast::Ast&       ast() const;
        [[nodiscard]] ast::err::ParserInfo& info();

      signals:
        void deleteClicked(size_t index);
        void updated(size_t index);

      private slots:
        void processFormula();

      private:
        void initPointers();
        void initButtons();
        void initLayout();
        void connectSignals();

        static inline size_t M_MAX_INDEX = 0;

        const size_t              m_index;
        bool                      m_formulaWasUpdated = false;
        QGridLayout*              m_layout            = nullptr;
        QCheckBox*                m_activeCheckBox    = nullptr;
        TextEdit*                 m_textEdit          = nullptr;
        QPushButton*              m_deleteButton      = nullptr;
        std::unique_ptr<ast::Ast> m_ast;
    };
} // namespace app

#endif // APP_FORMULAWIDGET_H
