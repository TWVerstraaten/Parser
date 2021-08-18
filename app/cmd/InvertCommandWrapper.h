//
// Created by pc on 13-07-21.
//

#ifndef APP_CMD_INVERTCOMMANDWRAPPER_H
#define APP_CMD_INVERTCOMMANDWRAPPER_H

#include <utility>

namespace app::cmd {
    template <typename T>
    class InvertCommandWrapper : public T {
      public:
        template <typename... Args>
        explicit InvertCommandWrapper(Args&&... args) : T(std::forward<Args>(args)...) {
        }

        void undo() override {
            T::redo();
        }

        void redo() override {
            T::undo();
        }
    };
} // namespace app::cmd

#endif // APP_CMD_INVERTCOMMANDWRAPPER_H
