//
// Created by pc on 28-05-21.
//

#ifndef PARSER_UNDOREDOHANDLER_H
#define PARSER_UNDOREDOHANDLER_H

#include <QUndoStack>
#include <QUndoView>
#include <memory>

class UndoRedoHandler {

  public:
    static void init();

    static void undo();

    static void redo();

    static void push(QUndoCommand* undoCommand);

    static void setPushBlocked(bool blocked);

  private:
    inline static bool                        m_pushBlocked = false;
    inline static bool                        m_initialized = false;
    inline static std::unique_ptr<QUndoStack> m_undoStack   = nullptr;
    inline static std::unique_ptr<QUndoView>  m_undoView    = nullptr;
};

#endif // PARSER_UNDOREDOHANDLER_H
