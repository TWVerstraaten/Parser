//
// Created by pc on 09-04-21.
//

#ifndef APP_FORMULAWIDGET_H
#define APP_FORMULAWIDGET_H

#include <QWidget>

class QCheckBox;
class QGridLayout;
class QPushButton;

namespace ast::err {
    class ParserInfo;
}

namespace app {
    class TextEdit;

    class FormulaWidget : public QWidget {
        Q_OBJECT

      public:
        explicit FormulaWidget(QWidget* parent);

        [[nodiscard]] size_t    index() const;
        [[nodiscard]] TextEdit* textEdit();
        [[nodiscard]] bool      isActive() const;
        [[nodiscard]] bool      formulaWasUpdated() const;
        void                    setFormulaWasUpdated(bool formulaWasUpdated);

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

        const size_t m_index;
        bool         m_formulaWasUpdated = false;
        QGridLayout* m_layout            = nullptr;
        QCheckBox*   m_activeCheckBox    = nullptr;
        TextEdit*    m_textEdit          = nullptr;
        QPushButton* m_deleteButton      = nullptr;
    };
} // namespace app

#endif // APP_FORMULAWIDGET_H
