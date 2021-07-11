//
// Created by pc on 11-07-21.
//

#include "OldCursor.h"

namespace app {
    void OldCursor::set(int cursorPosition, int cursorAnchor) {
        m_penUltCursorPosition = m_oldCursorPosition;
        m_oldCursorPosition    = cursorPosition;
        m_penUltAnchorPosition = m_oldAnchorPosition;
        m_oldAnchorPosition    = cursorAnchor;
    }

    int OldCursor::position() const {
        return m_penUltCursorPosition;
    }

    int OldCursor::anchor() const {
        return m_penUltAnchorPosition;
    }
} // namespace app