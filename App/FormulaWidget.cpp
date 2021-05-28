//
// Created by pc on 09-04-21.
//

#include "FormulaWidget.h"

#include "../Command/FormulaChangedCommand.h"
#include "../Command/SkipFirstRedoCommand.h"
#include "UndoRedoConsumer.h"
#include "UndoRedoHandler.h"

#include <QCheckBox>
#include <QDebug>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolTip>

static void setTextColor(QWidget* widget, const QColor& color) {
    auto* palette = new QPalette();
    palette->setColor(QPalette::Text, color);
    widget->setPalette(*palette);
}

FormulaWidget::FormulaWidget(QWidget* parent) : QWidget(parent), m_index(m_maxIndex) {
    ++m_maxIndex;

    initPointers();

    initButtons();

    m_layout->addWidget(m_deleteButton, 0, 2);
    m_layout->addWidget(m_activeCheckBox, 0, 0);
    m_layout->addWidget(m_lineEdit, 0, 1);
    m_layout->addWidget(m_collapseButton, 1, 0, 1, 3);
    m_layout->addWidget(new QLabel("Var:"), 2, 0);
    m_layout->addWidget(m_variableLabel, 2, 1);
    m_layout->addWidget(new QLabel("Spf:"), 3, 0);
    m_layout->addWidget(m_simplifiedLabel, 4, 1);
    m_layout->addWidget(new QLabel("Msg:"), 4, 0);
    m_layout->addWidget(m_messageLabel, 4, 1);
    m_layout->addWidget(new QLabel("Idx:"), 5, 0);
    m_layout->addWidget(m_indexLabel, 5, 1);

    connectSignals();
    m_lineEdit->installEventFilter(UndoRedoConsumer::undoRedoConsumer());

    m_layout->setAlignment(Qt::AlignTop);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void FormulaWidget::setFormula(const QString& string) {
    std::string functionString = string.toStdString();
    m_formula                  = std::make_unique<Formula>(functionString);
    if (m_formula->success()) {
        handleCorrectFormula();
    } else {
        handleWrongFormula(m_formula->errorString());
    }
}

void FormulaWidget::setTextColor(const QColor& color) {
    ::setTextColor(m_lineEdit, color);
    ::setTextColor(m_variableLabel, color);
    ::setTextColor(m_messageLabel, color);
}

void FormulaWidget::handleCorrectFormula() {
    setTextColor(Qt::black);
    const auto variableSet = m_formula->declaredVariables();
    if (variableSet.empty()) {
        m_variableLabel->setText("_");
    } else {
        QString variableString = "<font color=\"green\">";
        for (const auto& var : variableSet) {
            variableString += QString::fromStdString(var) + ",";
        }
        variableString.chop(1);
        m_variableLabel->setText(variableString);
    }
    m_messageLabel->setText("_");
    m_simplifiedLabel->setText(QString::fromStdString(m_formula->ast().toString()));
    QToolTip::hideText();

    if (const auto hint = m_formula->getHints(); not hint.empty()) {
        QPalette palette = QToolTip::palette();
        palette.setColor(QPalette::ToolTipBase, QColor(16185078));
        palette.setColor(QPalette::ToolTipText, QColor(251));
        QToolTip::setPalette(palette);

        QToolTip::showText(pos() + m_lineEdit->pos(), QString::fromStdString(hint));
    }
}

void FormulaWidget::handleWrongFormula(const std::string& errorMessage) {
    setTextColor(Qt::red);
    m_variableLabel->setText("_");
    m_messageLabel->setText(QString::fromStdString(errorMessage));

    QPalette palette = QToolTip::palette();
    palette.setColor(QPalette::ToolTipBase, QColor(16185078));
    palette.setColor(QPalette::ToolTipText, QColor(16449536));
    QToolTip::setPalette(palette);

    QToolTip::showText(pos() + m_lineEdit->pos(), QString::fromStdString(errorMessage));
}

size_t FormulaWidget::index() const {
    return m_index;
}

void FormulaWidget::toggleOptionsView() {
    if (m_optionsExpanded) {
        for (int i = 2; i != 5; ++i) {
            for (int j = 0; j != m_layout->rowCount(); ++j) {
                QLayoutItem* item = m_layout->itemAtPosition(i, j);
                if (item) {
                    QWidget* widget = item->widget();
                    widget->hide();
                }
            }
        }
    } else {
    }
    m_optionsExpanded = !m_optionsExpanded;
}

void FormulaWidget::initPointers() {
    m_layout         = new QGridLayout(this);
    m_activeCheckBox = new QCheckBox(this);
    m_activeCheckBox->setChecked(true);
    m_lineEdit        = new QLineEdit(this);
    m_variableLabel   = new QLabel("_", this);
    m_messageLabel    = new QLabel("_", this);
    m_simplifiedLabel = new QLabel("_", this);
    m_indexLabel      = new QLabel(QString::number(m_index), this);
    m_deleteButton    = new QPushButton{this};
    m_collapseButton  = new QPushButton(this);
}

void FormulaWidget::initButtons() {
    m_deleteButton->setText("x");
    m_deleteButton->setFixedSize(20, 20);
    connect(m_deleteButton, &QPushButton::clicked, [this]() { emit deleteClicked(m_index); });
    m_collapseButton->setFixedHeight(9);
    connect(m_collapseButton, &QPushButton::clicked, this, &FormulaWidget::toggleOptionsView);
}

void FormulaWidget::connectSignals() {
    connect(m_lineEdit, &QLineEdit::textChanged, this, &FormulaWidget::setFormula);
    connect(m_lineEdit, &QLineEdit::textEdited, [this]() {
        UndoRedoHandler::push(new SkipFirstRedoCommand(new FormulaChangedCommand(this, m_oldFormula, m_lineEdit->text())));
        m_oldFormula = m_lineEdit->text();
    });
}
