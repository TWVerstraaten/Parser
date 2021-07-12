//
// Created by pc on 28-05-21.
//

#ifndef APP_CMD_SKIPFIRSTREDOWRAPPER_H
#define APP_CMD_SKIPFIRSTREDOWRAPPER_H

#include <QUndoCommand>

namespace app::cmd {
    template <class T>
    class SkipFirstRedoWrapper : public T {

      public:
        template <typename... Args>
        explicit SkipFirstRedoWrapper(Args&&... args) : T(std::forward<Args>(args)...) {
        }

        void redo() override {
            if (m_firstSkipped) {
                T::redo();
            } else {
                m_firstSkipped = true;
            }
        }

      private:
        bool m_firstSkipped = false;
    };
} // namespace app::cmd

#endif // APP_CMD_SKIPFIRSTREDOWRAPPER_H
