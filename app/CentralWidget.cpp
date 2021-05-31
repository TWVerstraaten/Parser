//
// Created by pc on 31-05-21.
//

#include "CentralWidget.h"

#include <QHBoxLayout>
#include <QSplitter>

app::CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent) {
    m_formulaSideBar = new FormulaSideBar(this);
    m_openGlWidget   = new OpenGlWidget(this);

    m_layout = new QHBoxLayout(this);

    auto* splitter = new QSplitter;
    splitter->addWidget(m_formulaSideBar);
    splitter->setStretchFactor(0, 1);
    splitter->addWidget(m_openGlWidget);
    splitter->setStretchFactor(1, 2);
    splitter->setOrientation(Qt::Horizontal);

    //    m_layout->addWidget(m_formulaSideBar, 1);
    //    m_layout->addWidget(m_openGlWidget, 4);

    m_layout->addWidget(splitter);
    setLayout(m_layout);
    resize(800, 600);
}
