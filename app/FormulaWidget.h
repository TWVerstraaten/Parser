//
// Created by pc on 09-04-21.
//

#ifndef PARSER_FORMULAWIDGET_H
#define PARSER_FORMULAWIDGET_H

#include "../gen/ErrorBase.h"

#include <QWidget>

class QCheckBox;
class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;

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

        [[nodiscard]] size_t     index() const;
        [[nodiscard]] QLineEdit* lineEdit();
        //        [[nodiscard]] const fml::Formula* formula() const;
        [[nodiscard]] bool isActive() const;
        [[nodiscard]] bool formulaWasUpdated() const;
        void               setFormulaWasUpdated(bool formulaWasUpdated);

      signals:
        void deleteClicked(size_t index);
        void updated(size_t index);

      private slots:
        void processFormula();
        void toggleOptionsView();

      private:
        void handleCorrectFormula();
        void handleWrongFormula();
        void initPointers();
        void initButtons();
        void connectSignals();
        void showToolTipAtLineEdit(QRgb textColor, const QString& message);

        inline static size_t S_MAX_INDEX = 0;

        const size_t                  m_index;
        bool                          m_optionsExpanded   = true;
        bool                          m_formulaWasUpdated = false;
        QGridLayout*                  m_layout            = nullptr;
        QCheckBox*                    m_activeCheckBox    = nullptr;
        QLineEdit*                    m_lineEdit          = nullptr;
        QPushButton*                  m_deleteButton      = nullptr;
        QPushButton*                  m_collapseButton    = nullptr;
        QLabel*                       m_errorMessageLabel = nullptr;
        QString                       m_oldFormula        = "";
//        std::unique_ptr<fml::Formula> m_formula;
    };
} // namespace app

#endif // PARSER_FORMULAWIDGET_H
