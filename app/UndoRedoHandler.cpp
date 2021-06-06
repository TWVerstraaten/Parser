//
// Created by pc on 28-05-21.
//

#include "UndoRedoHandler.h"

namespace app {
    void UndoRedoHandler::init() {
        m_undoStack = std::make_unique<QUndoStack>(nullptr);
        //        m_undoView  = std::make_unique<QUndoView>(m_undoStack.get());
        //        m_undoView->show();
        m_initialized = true;
        m_undoStack->setUndoLimit(250);
    }

    void UndoRedoHandler::undo() {
        if (not m_initialized) {
            init();
        }
        m_undoStack->undo();
    }

    void UndoRedoHandler::redo() {
        if (not m_initialized) {
            init();
        }
        m_undoStack->redo();
    }

    void UndoRedoHandler::push(QUndoCommand* undoCommand) {
        if (not m_initialized) {
            init();
        }
        if (m_pushBlocked) {
            return;
        }
        m_undoStack->push(undoCommand);
    }

    void UndoRedoHandler::setPushBlocked(bool blocked) {
        m_pushBlocked = blocked;
    }
} // namespace app