//
// Created by pc on 28-05-21.
//

#ifndef PARSER_UNDOREDOCONSUMER_H
#define PARSER_UNDOREDOCONSUMER_H

#include <QDebug>
#include <QKeyEvent>
#include <QObject>
#include <memory>

class UndoRedoConsumer : public QObject {
    Q_OBJECT

  public:
    explicit UndoRedoConsumer(QObject* parent) : QObject(parent) {
    }

    static UndoRedoConsumer* undoRedoConsumer() {
        if (not m_undoRedoConsumer) {
            m_undoRedoConsumer = std::make_unique<UndoRedoConsumer>(nullptr);
        }
        return m_undoRedoConsumer.get();
    }

  protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

    static inline std::unique_ptr<UndoRedoConsumer> m_undoRedoConsumer{nullptr};
};

#endif // PARSER_UNDOREDOCONSUMER_H
