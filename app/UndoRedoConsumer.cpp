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
            auto*        key_event = dynamic_cast<QKeyEvent*>(event);
            QKeySequence key_sequence{static_cast<int>(key_event->modifiers()) + key_event->key()};
            if (key_sequence == QKeySequence::Undo) {
                UndoRedoHandler::undo();
                return true;
            } else if (key_sequence == QKeySequence::Redo) {
                UndoRedoHandler::redo();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

    UndoRedoConsumer* UndoRedoConsumer::undoRedoConsumer() {
        if (not m_undoRedoConsumer) {
            m_undoRedoConsumer = std::make_unique<UndoRedoConsumer>(nullptr);
        }
        return m_undoRedoConsumer.get();
    }
} // namespace app