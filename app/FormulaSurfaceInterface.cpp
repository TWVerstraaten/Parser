//
// Created by pc on 01-06-21.
//

#include "FormulaSurfaceInterface.h"

#include "FormulaSideBar.h"
#include "FormulaWidget.h"
#include "OpenGlWidget.h"
#include "SurfaceManager.h"

app::FormulaSurfaceInterface::FormulaSurfaceInterface(app::OpenGlWidget* surfaceManager, app::FormulaSideBar* formulaSideBar)
    : m_openGlWidget(surfaceManager), m_formulaSideBar(formulaSideBar) {
    connect(m_formulaSideBar, &FormulaSideBar::sendUpdate, this, &FormulaSurfaceInterface::updateSurfaces);
}

void app::FormulaSurfaceInterface::updateSurfaces() {
    auto* surfaceManager = m_openGlWidget->surfaceManager();

    const auto formulaWidgets = m_formulaSideBar->formulaWidgets();
    surfaceManager->clear();

    // TODO
    //    for (const auto& formulaWidget : formulaWidgets) {
    //        if (formulaWidget->isHidden() || not formulaWidget->isActive() || not formulaWidget->success()) {
    //            continue;
    //        }
    //        const auto* formula = formulaWidget->formula();
    //        if (formula->declaredVariables().size() == 2) {
    //            std::unique_ptr<Surface> newSurface = std::make_unique<Surface>();
    //            newSurface->fillBuffers(*formula);
    //            surfaceManager->add(std::move(newSurface));
    //        }
    //    }
    m_openGlWidget->repaint();
}
