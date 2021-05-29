//
// Created by pc on 28-05-21.
//

#ifndef PARSER_UNDOREDOCONSUMER_H
#define PARSER_UNDOREDOCONSUMER_H

#include <QObject>
#include <memory>

namespace app {
    class UndoRedoConsumer : public QObject {
        Q_OBJECT

      public:
        explicit UndoRedoConsumer(QObject* parent);

        static UndoRedoConsumer* undoRedoConsumer();

      protected:
        bool eventFilter(QObject* obj, QEvent* event) override;

        static inline std::unique_ptr<UndoRedoConsumer> m_undoRedoConsumer{nullptr};
    };
} // namespace app

#endif // PARSER_UNDOREDOCONSUMER_H
