//
// Created by pc on 28-05-21.
//

#include "RemoveFormulaWidgetCommand.h"

RemoveFormulaWidgetCommand::RemoveFormulaWidgetCommand(FormulaSideBar* formulaSideBar, size_t index)
    : m_formulaSideBar(formulaSideBar), m_indexOfRemovedFormulaWidget(index) {
}

void RemoveFormulaWidgetCommand::undo() {
    assert(m_formulaSideBar->fromIndex(m_indexOfRemovedFormulaWidget)->index() == m_indexOfRemovedFormulaWidget);
    m_formulaSideBar->fromIndex(m_indexOfRemovedFormulaWidget)->show();
}

void RemoveFormulaWidgetCommand::redo() {
    assert(m_formulaSideBar->fromIndex(m_indexOfRemovedFormulaWidget)->index() == m_indexOfRemovedFormulaWidget);
    m_formulaSideBar->fromIndex(m_indexOfRemovedFormulaWidget)->hide();
}
