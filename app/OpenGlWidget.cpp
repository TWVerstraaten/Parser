//
// Created by pc on 29-05-21.
//

#include "OpenGlWidget.h"

#include <QMouseEvent>

namespace app {
    OpenGlWidget::OpenGlWidget(QWidget* parent) : QOpenGLWidget(parent) {
        m_cameraWidget = new CameraWidget();
        m_cameraWidget->show();
        connect(m_cameraWidget, &CameraWidget::updated, this, [this]() { update(); });

        setMinimumWidth(400);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void OpenGlWidget::mousePressEvent(QMouseEvent* e) {
        m_mousePressPosition = QVector2D(e->localPos());
        const auto button    = e->button();
        m_clickState         = button == Qt::LeftButton   ? CLICK_STATE::LEFT
                             : button == Qt::RightButton  ? CLICK_STATE::RIGHT
                             : button == Qt::MiddleButton ? CLICK_STATE::MIDDLE
                                                          : CLICK_STATE::NONE;
    }

    void OpenGlWidget::mouseMoveEvent(QMouseEvent* e) {
        const auto newPosition = QVector2D(e->localPos());
        const auto dPosition   = newPosition - m_mousePressPosition;
        const auto dX          = dPosition.x();
        const auto dY          = dPosition.y();

        switch (m_clickState) {
            case CLICK_STATE::NONE:
                break;
            case CLICK_STATE::LEFT:
                m_cameraWidget->translateCameraPosition(dX, dY);
                break;
            case CLICK_STATE::RIGHT:
                m_cameraWidget->rotateViewDirection(dX, dY);
                break;
            case CLICK_STATE::MIDDLE:
                m_cameraWidget->rotateUpDirection(dX, dY);
                break;
        }

        m_mousePressPosition = newPosition;
        update();
    }

    void OpenGlWidget::mouseReleaseEvent(QMouseEvent* e) {
        m_clickState = CLICK_STATE::NONE;
    }

    void OpenGlWidget::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(1, 1, 1, 1);
        initShaders();

        glEnable(GL_DEPTH_TEST);
        //        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_surfaceManager = std::make_unique<SurfaceManager>();
    }

    void OpenGlWidget::initShaders() {
        if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl")) {
            close();
        }
        if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl")) {
            close();
        }
        if (!m_shaderProgram.link()) {
            close();
        }
        if (!m_shaderProgram.bind()) {
            close();
        }
    }

    void OpenGlWidget::resizeGL(int w, int h) {
        m_cameraWidget->setProjectionMatrix(w, h);
    }

    void OpenGlWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_cameraWidget->setProjectionMatrix(width(), height());
        m_shaderProgram.setUniformValue("mvp_matrix", m_cameraWidget->projectionMatrix() * m_cameraWidget->lookAtMatrix());
        m_surfaceManager->draw(&m_shaderProgram);
    }

    void OpenGlWidget::wheelEvent(QWheelEvent* event) {
        const float degrees = event->angleDelta().y() / 8.0f;
        m_cameraWidget->zoom(degrees);
        update();
    }

} // namespace app