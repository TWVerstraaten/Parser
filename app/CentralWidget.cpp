//
// Created by pc on 31-05-21.
//

#include "CentralWidget.h"

#include "FormulaSideBar.h"
#include "OpenGlWidget.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QSplitter>

app::CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent) {
    m_openGlWidget   = new OpenGlWidget(this);
    m_formulaSideBar = new FormulaSideBar(this);

    m_formulaSurfaceInterface = std::make_unique<FormulaSurfaceInterface>(m_openGlWidget, m_formulaSideBar);

    m_layout = new QHBoxLayout(this);

    auto* splitter = new QSplitter;
    splitter->addWidget(m_formulaSideBar);
    splitter->setStretchFactor(0, 0);
    splitter->addWidget(m_openGlWidget);
    splitter->setStretchFactor(1, 1);
    splitter->setOrientation(Qt::Horizontal);

    m_layout->addWidget(splitter);
    setLayout(m_layout);
    resize(800, 600);
}

void app::CentralWidget::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_C:
            m_openGlWidget->cameraWidget().toggleCameraMode();
            m_openGlWidget->repaint();
            break;
        default:
            break;
    }
}
