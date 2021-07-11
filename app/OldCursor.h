//
// Created by pc on 11-07-21.
//

#ifndef APP_OLDCURSOR_H
#define APP_OLDCURSOR_H

namespace app {
    class OldCursor {

      public:
        void set(int cursorPosition, int cursorAnchor);

        [[nodiscard]] int position() const;
        [[nodiscard]] int anchor() const;

      private:
        int m_oldCursorPosition    = 0;
        int m_penUltCursorPosition = 0;
        int m_oldAnchorPosition    = 0;
        int m_penUltAnchorPosition = 0;
    };
} // namespace app

#endif // APP_OLDCURSOR_H
