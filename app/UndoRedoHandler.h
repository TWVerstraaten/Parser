//
// Created by pc on 28-05-21.
//

#ifndef PARSER_UNDOREDOHANDLER_H
#define PARSER_UNDOREDOHANDLER_H

#include <QUndoStack>
#include <QUndoView>
#include <memory>

namespace app {
    class UndoRedoHandler {

      public:
        static void S_INIT();
        static void S_UNDO();
        static void S_REDO();
        static void S_PUSH(QUndoCommand* undoCommand);
        static void S_SET_PUSH_BLOCKED(bool blocked);

      private:
        inline static bool                        S_PUSH_BLOCKED = false;
        inline static bool                        S_INITIALIZED  = false;
        inline static std::unique_ptr<QUndoStack> S_UNDO_STACK   = nullptr;
    };
} // namespace app

#endif // PARSER_UNDOREDOHANDLER_H
