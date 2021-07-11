//
// Created by pc on 28-05-21.
//

#include "UndoRedoHandler.h"

#include <QDebug>

namespace app {
    void UndoRedoHandler::S_INIT() {
        S_UNDO_STACK  = std::make_unique<QUndoStack>(nullptr);
        S_INITIALIZED = true;
        S_UNDO_STACK->setUndoLimit(250);
    }

    void UndoRedoHandler::S_UNDO() {
        if (not S_INITIALIZED) {
            S_INIT();
        }
        qDebug() << "Undo!";
        S_UNDO_STACK->undo();
    }

    void UndoRedoHandler::S_REDO() {
        if (not S_INITIALIZED) {
            S_INIT();
        }
        qDebug() << "Redo!";
        S_UNDO_STACK->redo();
    }

    void UndoRedoHandler::S_PUSH(QUndoCommand* undoCommand) {
        if (not S_INITIALIZED) {
            S_INIT();
        }
        if (S_PUSH_BLOCKED) {
            return;
        }
        S_UNDO_STACK->push(undoCommand);
    }

    void UndoRedoHandler::S_SET_PUSH_BLOCKED(bool blocked) {
        S_PUSH_BLOCKED = blocked;
    }
} // namespace app