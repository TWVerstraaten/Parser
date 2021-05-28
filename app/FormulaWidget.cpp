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
        m_formula                  = std::make_unique<fml::Formula>(functionString);
        if (m_formula->success()) {
            handleCorrectFormula();
        } else {
            handleWrongFormula(m_formula->errorString());
        }
    }

    void FormulaWidget::setTextColor(const QColor& color) {
        ::app::setTextColor(m_lineEdit, color);
        ::app::setTextColor(m_variableLabel, color);
        ::app::setTextColor(m_messageLabel, color);
    }

    void FormulaWidget::handleCorrectFormula() {
        setTextColor(Qt::black);
        const auto variableSet = m_formula->declaredVariables();
        m_variableLabel->setText(
            variableSet.empty() ? "_" : "<font color=\"green\">" + QString::fromStdString(alg::StringAlg::setToString(variableSet)));
        m_messageLabel->setText("_");
        m_simplifiedLabel->setText(QString::fromStdString(m_formula->ast().toString()));
        QToolTip::hideText();

        if (const auto hint = m_formula->getHints(); not hint.empty()) {
            showToolTipAtLineEdit(251, QString::fromStdString(hint));
        }
        std::cout << "x=1,y=3"
                  << " " << m_formula->eval({{"x", gen::Number(1ll)}, {"y", gen::Number(3ll)}}).toString() << '\n';
    }

    void FormulaWidget::handleWrongFormula(const std::string& errorMessage) {
        setTextColor(Qt::red);
        m_variableLabel->setText("_");
        m_messageLabel->setText(QString::fromStdString(errorMessage));

        showToolTipAtLineEdit(16449536, QString::fromStdString(errorMessage));
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
            UndoRedoHandler::push(new cmd::SkipFirstRedoCommand(new cmd::FormulaChangedCommand(this, m_oldFormula, m_lineEdit->text())));
            m_oldFormula = m_lineEdit->text();
        });
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
} // namespace app