//
// Created by pc on 28-05-21.
//

#include "RemoveFormulaWidgetCommand.h"

#include "../FormulaSideBar.h"
#include "../FormulaWidget.h"

namespace app::cmd {
    RemoveFormulaWidgetCommand::RemoveFormulaWidgetCommand(app::FormulaSideBar* formulaSideBar, size_t index)
        : m_formulaSideBar(formulaSideBar), m_indexOfRemovedFormulaWidget(index) {
    }

    void RemoveFormulaWidgetCommand::undo() {
        m_formulaSideBar->fromIndex(m_indexOfRemovedFormulaWidget)->show();
    }

    void RemoveFormulaWidgetCommand::redo() {
        m_formulaSideBar->fromIndex(m_indexOfRemovedFormulaWidget)->hide();
    }
} // namespace app::cmd