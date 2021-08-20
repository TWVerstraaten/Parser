//
// Created by pc on 28-05-21.
//

#ifndef APP_UNDOREDOCONSUMER_H
#define APP_UNDOREDOCONSUMER_H

#include <QObject>
#include <memory>

namespace app {
    class UndoRedoConsumer : public QObject {
        Q_OBJECT

      public:
        explicit UndoRedoConsumer(QObject* parent);

        static UndoRedoConsumer* UNDO_REDO_CONSUMER();

      protected:
        bool eventFilter(QObject* obj, QEvent* event) override;

        static inline std::unique_ptr<UndoRedoConsumer> M_UNDO_REDO_CONSUMER{nullptr};
    };
} // namespace app

#endif // APP_UNDOREDOCONSUMER_H
