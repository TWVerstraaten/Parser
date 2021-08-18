//
// Created by pc on 28-05-21.
//

#ifndef APP_CMD_REMOVEFORMULAWIDGETCOMMAND_H
#define APP_CMD_REMOVEFORMULAWIDGETCOMMAND_H

#include "InvertCommandWrapper.h"
#include "NewFormulaWidgetCommand.h"

namespace app::cmd {
    typedef InvertCommandWrapper<NewFormulaWidgetCommand> RemoveFormulaWidgetCommand;
}

#endif // APP_CMD_REMOVEFORMULAWIDGETCOMMAND_H
