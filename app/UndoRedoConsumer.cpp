//
// Created by pc on 28-05-21.
//

#include "UndoRedoConsumer.h"

#include "UndoRedoHandler.h"

#include <QKeyEvent>

namespace app {
    UndoRedoConsumer::UndoRedoConsumer(QObject* parent) : QObject(parent) {
    }

    bool UndoRedoConsumer::eventFilter(QObject* obj, QEvent* event) {
        if (event->type() == QEvent::KeyPress) {
            auto*        keyEvent = dynamic_cast<QKeyEvent*>(event);
            QKeySequence keySequence{static_cast<int>(keyEvent->modifiers()) + keyEvent->key()};
            if (keySequence == QKeySequence::Undo) {
                UndoRedoHandler::S_UNDO();
                return true;
            } else if (keySequence == QKeySequence::Redo) {
                UndoRedoHandler::S_REDO();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

    UndoRedoConsumer* UndoRedoConsumer::undoRedoConsumer() {
        if (not S_UNDO_REDO_CONSUMER) {
            S_UNDO_REDO_CONSUMER = std::make_unique<UndoRedoConsumer>(nullptr);
        }
        return S_UNDO_REDO_CONSUMER.get();
    }
} // namespace app