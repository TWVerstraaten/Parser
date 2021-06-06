//
// Created by pc on 09-04-21.
//

#include "FormulaWidget.h"

#include "../alg/StringAlg.h"
#include "UndoRedoConsumer.h"
#include "UndoRedoHandler.h"
#include "cmd/FormulaChangedCommand.h"
#include "cmd/SkipFirstRedoCommand.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolTip>

namespace app {
    static void setTextColor(QWidget* widget, const QColor& color) {
        auto* palette = new QPalette();
        palette->setColor(QPalette::Text, color);
        widget->setPalette(*palette);
    }

    FormulaWidget::FormulaWidget(QWidget* parent) : QWidget(parent), m_index(m_maxIndex) {
        ++m_maxIndex;
        m_success = false;

        initPointers();
        initButtons();

        m_layout->addWidget(m_activeCheckBox, 0, 0);
        m_layout->addWidget(m_lineEdit, 0, 1);
        m_layout->addWidget(m_deleteButton, 0, 2);
        m_layout->addWidget(m_errorMessageLabel, 1, 0, 1, 3);
        m_layout->addWidget(m_collapseButton, 2, 0, 1, 3);

        connectSignals();
        m_lineEdit->installEventFilter(UndoRedoConsumer::undoRedoConsumer());

        m_layout->setAlignment(Qt::AlignTop);
    }

    void FormulaWidget::processFormula() {
        const auto string = m_lineEdit->text();
        UndoRedoHandler::push(new cmd::SkipFirstRedoCommand(new cmd::FormulaChangedCommand(this, m_oldFormula, string)));
        m_oldFormula = string;

        m_formula           = std::make_unique<fml::Formula>(string.toStdString());
        m_formulaWasUpdated = true;

        emit updated(m_index);
    }

    void FormulaWidget::handleCorrectFormula() {
        setTextColor(m_lineEdit, Qt::black);
        QToolTip::hideText();

        if (const auto hint = m_formula->getHints(); not hint.empty()) {
            //            showToolTipAtLineEdit(251, QString::fromStdString(hint));
            m_errorMessageLabel->setText(QString::fromStdString(hint));
            m_errorMessageLabel->setStyleSheet("QLabel { color : rgb(0,0,251); }");
        } else {
            m_errorMessageLabel->setText("");
        }
        update();
    }

    void FormulaWidget::handleWrongFormula() {
        setTextColor(m_lineEdit, Qt::red);
        //        showToolTipAtLineEdit(16449536, QString::fromStdString(m_errorString));
        m_errorMessageLabel->setText(QString::fromStdString(m_errorString));
        m_errorMessageLabel->setStyleSheet("QLabel { color : rgb(251,0,0); }");
        update();
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
        m_lineEdit          = new QLineEdit(this);
        m_deleteButton      = new QPushButton{this};
        m_collapseButton    = new QPushButton(this);
        m_errorMessageLabel = new QLabel(this);
    }

    void FormulaWidget::initButtons() {
        m_deleteButton->setText("x");
        m_deleteButton->setFixedSize(20, 20);
        m_collapseButton->setFixedHeight(9);
    }

    void FormulaWidget::connectSignals() {
        connect(m_lineEdit, &QLineEdit::textChanged, this, &FormulaWidget::processFormula);
        connect(m_deleteButton, &QPushButton::clicked, [this]() { emit deleteClicked(m_index); });
        connect(m_collapseButton, &QPushButton::clicked, this, &FormulaWidget::toggleOptionsView);
        connect(m_activeCheckBox, &QCheckBox::toggled, [this]() { emit updated(m_index); });
    }

    void FormulaWidget::showToolTipAtLineEdit(QRgb textColor, const QString& message) {
        QPalette palette = QToolTip::palette();
        palette.setColor(QPalette::ToolTipBase, QColor(16185078));
        palette.setColor(QPalette::ToolTipText, QColor(textColor));
        QToolTip::setPalette(palette);

        QToolTip::showText(mapToGlobal(m_lineEdit->pos()), message);
    }

    QLineEdit* FormulaWidget::lineEdit() {
        return m_lineEdit;
    }

    const fml::Formula* FormulaWidget::formula() const {
        return m_formula.get();
    }

    void FormulaWidget::updateWidget() {
        if (m_success) {
            handleCorrectFormula();
        } else {
            handleWrongFormula();
        }
        update();
    }

    bool FormulaWidget::isActive() const {
        return m_activeCheckBox->isChecked();
    }

    bool FormulaWidget::formulaWasUpdated() const {
        return m_formulaWasUpdated;
    }

    void FormulaWidget::setFormulaWasUpdated(bool formulaWasUpdated) {
        m_formulaWasUpdated = formulaWasUpdated;
    }

} // namespace app