//
// Created by pc on 28-05-21.
//

#include "NewFormulaWidgetCommand.h"

namespace app::cmd {
    NewFormulaWidgetCommand::NewFormulaWidgetCommand(app::FormulaSideBar* formulaSideBar, size_t index)
        : m_formulaSideBar(formulaSideBar), m_indexOfNewFormulaWidget(index) {
    }

    void NewFormulaWidgetCommand::undo() {
        m_formulaSideBar->fromIndex(m_indexOfNewFormulaWidget)->hide();
    }

    void NewFormulaWidgetCommand::redo() {
        m_formulaSideBar->fromIndex(m_indexOfNewFormulaWidget)->show();
    }
} // namespace app::cmd