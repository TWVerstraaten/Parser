//
// Created by pc on 10-07-21.
//

#include "TextEdit.h"

#include "../gen/defines.h"
#include "UndoRedoConsumer.h"
#include "UndoRedoHandler.h"
#include "cmd/SkipFirstRedoWrapper.h"
#include "cmd/TextEditChangedCommand.h"

#include <QAction>
#include <QDebug>
#include <QMouseEvent>
#include <QToolTip>

namespace app {
    enum class CHAR_FORMATS { ERROR, WARNING, MESSAGE };

    QTextCharFormat S_CHAR_FORMAT(CHAR_FORMATS charFormat) {
        QTextCharFormat qTextCharFormat;
        switch (charFormat) {
            case CHAR_FORMATS::ERROR:
                qTextCharFormat.setUnderlineColor(Qt::red);
                qTextCharFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
                break;
            case CHAR_FORMATS::WARNING:
                qTextCharFormat.setUnderlineColor(Qt::blue);
                qTextCharFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
                break;
            case CHAR_FORMATS::MESSAGE:
                qTextCharFormat.setUnderlineColor(Qt::darkGray);
                qTextCharFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                break;
        }
        return qTextCharFormat;
    }

    template <typename Container>
    void S_HIGHLIGHT_FROM_CONTAINER(const Container& container, QTextDocument* document, const QTextCharFormat& format) {
        QTextCursor cursor(document);
        for (const auto& i : container) {
            const auto range = i.range();
            if (range.isEmpty()) {
                continue;
            }
            cursor.setPosition(range.startIndex(), QTextCursor::MoveAnchor);
            cursor.setPosition(range.endIndex() + 1, QTextCursor::KeepAnchor);
            cursor.setCharFormat(format);
        }
    }

    static void S_SET_TOOLTIP_PALETTE(const QColor& base, const QColor& text) {
        QPalette palette = QToolTip::palette();
        palette.setColor(QPalette::ToolTipBase, base);
        palette.setColor(QPalette::ToolTipText, text);
        QToolTip::setPalette(palette);
    }

    TextEdit::TextEdit(QWidget* parent) : QTextEdit(parent) {
        setMouseTracking(true);
        setLineWrapMode(QTextEdit::NoWrap);
        QFontMetrics metrics(font());
        setFixedHeight(metrics.lineSpacing() + 8);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        //    QFont font("Monospace");
        //    font.setStyleHint(QFont::TypeWriter);
        //    setFont(font);
        installEventFilter(UndoRedoConsumer::UNDO_REDO_CONSUMER());
        connect(this, &QTextEdit::textChanged, this, &TextEdit::sendToUndoRedoHandler);
        connect(this, &QTextEdit::cursorPositionChanged, [this] { m_oldCursor.set(textCursor().position(), textCursor().anchor()); });
    }

    void TextEdit::mouseMoveEvent(QMouseEvent* e) {
        const auto charPosition = QTextCursor(cursorForPosition(e->pos())).position();
        const auto rangeCheck   = [charPosition](const auto& a) { return a.range().isInRange(charPosition); };

        if (not m_info.definitionErrors().empty()) {
            S_SET_TOOLTIP_PALETTE(QColor(16185078), Qt::red);
            QToolTip::showText(mapToGlobal(e->pos()), QString::fromStdString(m_info.definitionErrors().front().toString()));
        } else if (auto parserErrorIt = std::find_if(TT_IT(m_info.parserErrors()), rangeCheck); parserErrorIt != m_info.parserErrors().end()) {
            S_SET_TOOLTIP_PALETTE(QColor(16185078), Qt::red);
            QToolTip::showText(mapToGlobal(e->pos()), QString::fromStdString(parserErrorIt->toString()));
        } else if (auto warningIt = std::find_if(TT_IT(m_info.warnings()), rangeCheck); warningIt != m_info.warnings().end()) {
            S_SET_TOOLTIP_PALETTE(QColor(16185078), Qt::blue);
            QToolTip::showText(mapToGlobal(e->pos()), QString::fromStdString(warningIt->toString()));
        } else if (auto messageIt = std::find_if(TT_IT(m_info.messages()), rangeCheck); messageIt != m_info.messages().end()) {
            S_SET_TOOLTIP_PALETTE(QColor(16185078), Qt::black);
            QToolTip::showText(mapToGlobal(e->pos()), QString::fromStdString(messageIt->toString()));
        }
        QTextEdit::mouseMoveEvent(e);
    }

    void TextEdit::setInfo(const ast::err::ParserInfo& info) {
        m_info = info;
        blockSignals(true);
        highlightInfo();
        determineAndSetTextColor();
        blockSignals(false);
    }

    void TextEdit::highlightInfo() {
        S_HIGHLIGHT_FROM_CONTAINER(m_info.messages(), document(), S_CHAR_FORMAT(CHAR_FORMATS::MESSAGE));
        S_HIGHLIGHT_FROM_CONTAINER(m_info.warnings(), document(), S_CHAR_FORMAT(CHAR_FORMATS::WARNING));
        S_HIGHLIGHT_FROM_CONTAINER(m_info.parserErrors(), document(), S_CHAR_FORMAT(CHAR_FORMATS::ERROR));
    }

    void TextEdit::determineAndSetTextColor() {
        const auto& textColor = m_info.success() ? Qt::black : Qt::red;
        QTextCursor cursor    = textCursor();
        selectAll();
        setTextColor(textColor);
        setTextCursor(cursor);
    }

    void TextEdit::sendToUndoRedoHandler() {
        blockSignals(true);
        UndoRedoHandler::PUSH(new cmd::TextEditChangedCommand{this, m_oldString, m_oldCursor});
        m_oldString = toPlainText();
    }

} // namespace app