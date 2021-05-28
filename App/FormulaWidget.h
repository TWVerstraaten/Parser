//
// Created by pc on 09-04-21.
//

#ifndef PARSER_FORMULAWIDGET_H
#define PARSER_FORMULAWIDGET_H

#include "../Parser/Formula.h"

#include <QWidget>

class QCheckBox;
class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;

class FormulaWidget : public QWidget {
    Q_OBJECT

    friend class FormulaChangedCommand;

  public:
    explicit FormulaWidget(QWidget* parent = nullptr);

    [[nodiscard]] size_t index() const;

  signals:
    void deleteClicked(size_t index);

  private slots:
    void setFormula(const QString& string);
    void toggleOptionsView();

  private:
    void setTextColor(const QColor& color);
    void handleCorrectFormula();
    void handleWrongFormula(const std::string& errorMessage);
    void initPointers();
    void initButtons();
    void connectSignals();

    inline static size_t m_maxIndex = 0;

    const size_t m_index;
    bool         m_optionsExpanded = true;
    QGridLayout* m_layout          = nullptr;
    QCheckBox*   m_activeCheckBox  = nullptr;
    QLineEdit*   m_lineEdit        = nullptr;
    QLabel*      m_variableLabel   = nullptr;
    QLabel*      m_messageLabel    = nullptr;
    QLabel*      m_indexLabel      = nullptr;
    QLabel*      m_simplifiedLabel = nullptr;
    QPushButton* m_deleteButton    = nullptr;
    QPushButton* m_collapseButton  = nullptr;
    QString      m_oldFormula      = "";

    std::unique_ptr<Formula> m_formula;
};

#endif // PARSER_FORMULAWIDGET_H
