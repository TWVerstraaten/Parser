//
// Created by pc on 28-05-21.
//

#ifndef PARSER_SKIPFIRSTREDOCOMMAND_H
#define PARSER_SKIPFIRSTREDOCOMMAND_H

#include <QUndoCommand>
#include <memory>

namespace app::cmd {
    template <class T>
    class SkipFirstRedoCommand : public QUndoCommand {

      public:
        explicit SkipFirstRedoCommand(T* base) : m_base(base) {
        }

        void undo() override {
            m_base->undo();
        }

        void redo() override {
            if (m_firstSkipped) {
                m_base->redo();
            } else {
                m_firstSkipped = true;
            }
        }

      private:
        bool               m_firstSkipped = false;
        std::unique_ptr<T> m_base;
    };
} // namespace app::cmd
#endif // PARSER_SKIPFIRSTREDOCOMMAND_H
