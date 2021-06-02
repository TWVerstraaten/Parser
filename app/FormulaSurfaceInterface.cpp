//
// Created by pc on 01-06-21.
//

#include "FormulaSurfaceInterface.h"

#include "../fml/Formula.h"
#include "FormulaSideBar.h"
#include "FormulaWidget.h"
#include "OpenGlWidget.h"
#include "SurfaceManager.h"

#include <iostream>
#include <memory>

app::FormulaSurfaceInterface::FormulaSurfaceInterface(app::OpenGlWidget* surfaceManager, app::FormulaSideBar* formulaSideBar)
    : m_openGlWidget(surfaceManager), m_formulaSideBar(formulaSideBar) {
    connect(m_formulaSideBar, &FormulaSideBar::sendUpdate, this, &FormulaSurfaceInterface::updateSurfaces);
}

void app::FormulaSurfaceInterface::updateSurfaces() {
    auto* surfaceManager = m_openGlWidget->surfaceManager();

    const auto formulaWidgets = m_formulaSideBar->formulaWidgets();
    surfaceManager->clear();

    for (const auto& formulaWidget : formulaWidgets) {
        const auto* formula = formulaWidget->formula();
        if (formulaWidget->formula() && formula->success()) {
            if (formula->declaredVariables().size() == 2) {
                std::unique_ptr<Surface> newSurface = std::make_unique<Surface>();
                newSurface->fillBuffers(*formula);
                surfaceManager->add(std::move(newSurface));
            }
        }
    }
    m_openGlWidget->repaint();
}
