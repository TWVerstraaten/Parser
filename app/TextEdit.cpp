//
// Created by pc on 10-07-21.
//

#include "TextEdit.h"

#include "../gen/defines.h"

#include <QMouseEvent>
#include <QToolTip>

enum class CHAR_FORMATS { ERROR, WARNING };

QTextCharFormat S_CHAR_FORMAT(CHAR_FORMATS charFormat) {
    QTextCharFormat qTextCharFormat;
    switch (charFormat) {
        case CHAR_FORMATS::ERROR:
            qTextCharFormat.setForeground(Qt::red);
            break;
        case CHAR_FORMATS::WARNING:
            qTextCharFormat.setUnderlineColor(Qt::blue);
            qTextCharFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
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
}

void TextEdit::mouseMoveEvent(QMouseEvent* e) {
    size_t charPosition = QTextCursor(cursorForPosition(e->pos())).position();

    if (auto it = std::find_if(TT_IT(m_info.errors()), TT_LAMBDA_REF(a, return a.range().isInRange(charPosition);)); it != m_info.errors().end()) {
        S_SET_TOOLTIP_PALETTE(QColor(16185078), Qt::red);
        QToolTip::showText(mapToGlobal(e->pos()), QString::fromStdString(it->toString()));
    } else if (auto it2 = std::find_if(TT_IT(m_info.warnings()), TT_LAMBDA_REF(a, return a.range().isInRange(charPosition);)); it2 != m_info.warnings().end()) {
        S_SET_TOOLTIP_PALETTE(QColor(16185078), Qt::blue);
        QToolTip::showText(mapToGlobal(e->pos()), QString::fromStdString(it2->toString()));
    }

    QTextEdit::mouseMoveEvent(e);
}

void TextEdit::setInfo(ast::err::ParserInfo&& info) {
    m_info = std::move(info);
    highlightInfo();
}

void TextEdit::highlightInfo() {
    blockSignals(true);

    const auto& errors   = m_info.errors();
    const auto& warnings = m_info.warnings();

    S_HIGHLIGHT_FROM_CONTAINER(errors, document(), S_CHAR_FORMAT(CHAR_FORMATS::ERROR));
    S_HIGHLIGHT_FROM_CONTAINER(warnings, document(), S_CHAR_FORMAT(CHAR_FORMATS::WARNING));

    blockSignals(false);
}
