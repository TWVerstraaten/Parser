//
// Created by pc on 28-05-21.
//

#include "UndoRedoHandler.h"

namespace app {
    void UndoRedoHandler::INIT() {
        M_UNDO_STACK  = std::make_unique<QUndoStack>(nullptr);
        M_INITIALIZED = true;
        M_UNDO_STACK->setUndoLimit(250);
    }

    void UndoRedoHandler::UNDO() {
        if (not M_INITIALIZED) {
            INIT();
        }
        M_UNDO_STACK->undo();
    }

    void UndoRedoHandler::REDO() {
        if (not M_INITIALIZED) {
            INIT();
        }
        M_UNDO_STACK->redo();
    }

    void UndoRedoHandler::PUSH(QUndoCommand* undoCommand) {
        if (not M_INITIALIZED) {
            INIT();
        }
        if (M_PUSH_BLOCKED) {
            return;
        }
        M_UNDO_STACK->push(undoCommand);
    }

    void UndoRedoHandler::SET_PUSH_BLOCKED(bool blocked) {
        M_PUSH_BLOCKED = blocked;
    }
} // namespace app