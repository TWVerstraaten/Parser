//
// Created by pc on 28-05-21.
//

#include "UndoRedoConsumer.h"

#include "UndoRedoHandler.h"

#include <QDebug>

bool UndoRedoConsumer::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        auto*        key_event = dynamic_cast<QKeyEvent*>(event);
        QKeySequence key_sequence{static_cast<int>(key_event->modifiers()) + key_event->key()};
        if (key_sequence == QKeySequence::Undo) {
            UndoRedoHandler::undo();
            qDebug() << "undo";
            return true;
        } else if (key_sequence == QKeySequence::Redo) {
            UndoRedoHandler::redo();
            qDebug() << "redo";
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}
