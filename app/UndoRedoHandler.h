//
// Created by pc on 28-05-21.
//

#ifndef APP_UNDOREDOHANDLER_H
#define APP_UNDOREDOHANDLER_H

#include <QUndoStack>
#include <memory>

namespace app {
    class UndoRedoHandler {

      public:
        static void INIT();
        static void UNDO();
        static void REDO();
        static void PUSH(QUndoCommand* undoCommand);
        static void SET_PUSH_BLOCKED(bool blocked);

      private:
        inline static bool                        M_PUSH_BLOCKED = false;
        inline static bool                        M_INITIALIZED  = false;
        inline static std::unique_ptr<QUndoStack> M_UNDO_STACK   = nullptr;
    };
} // namespace app

#endif // APP_UNDOREDOHANDLER_H
