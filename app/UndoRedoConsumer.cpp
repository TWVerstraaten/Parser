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
                UndoRedoHandler::UNDO();
                return true;
            } else if (keySequence == QKeySequence::Redo) {
                UndoRedoHandler::REDO();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

    UndoRedoConsumer* UndoRedoConsumer::UNDO_REDO_CONSUMER() {
        if (not M_UNDO_REDO_CONSUMER) {
            M_UNDO_REDO_CONSUMER = std::make_unique<UndoRedoConsumer>(nullptr);
        }
        return M_UNDO_REDO_CONSUMER.get();
    }
} // namespace app