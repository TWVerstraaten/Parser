//
// Created by pc on 29-05-21.
//

#include "OpenGlWidget.h"

#include "CameraManager.h"
#include "Surface.h"
#include "SurfaceManager.h"

#include <QMouseEvent>
#include <QPainter>
#include <memory>

namespace app {
    OpenGlWidget::OpenGlWidget(QWidget* parent) : QOpenGLWidget(parent) {
        m_cameraManager = std::make_unique<CameraManager>(this);
        m_cameraManager->setProjectionMatrix(width(), height());
    }

    OpenGlWidget::~OpenGlWidget() = default;

    void OpenGlWidget::mousePressEvent(QMouseEvent* e) {
        setFocus();
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
                m_cameraManager->translateCameraPosition(dX, dY);
                break;
            case CLICK_STATE::RIGHT:
                m_cameraManager->rotateViewDirection(dX, dY);
                break;
            case CLICK_STATE::MIDDLE:
                m_cameraManager->rotateUpDirection(dX, dY);
                break;
        }

        m_mousePressPosition = newPosition;
        update();
    }

    void OpenGlWidget::mouseReleaseEvent([[maybe_unused]] QMouseEvent* e) {
        m_clickState = CLICK_STATE::NONE;
    }

    void OpenGlWidget::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        initShaders();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // This call needs to be done after initializeOpenGLFunctions()
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
        m_cameraManager->setProjectionMatrix(w, h);
    }

    void OpenGlWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        QPainter painter(this);
        painter.beginNativePainting();
        openGlPaintRoutine();
        painter.endNativePainting();
        qtPaintRoutine(painter);
    }

    void OpenGlWidget::openGlPaintRoutine() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_shaderProgram.bind();
        m_shaderProgram.setUniformValue("mvp_matrix", m_cameraManager->modelViewProjectionMatrix());
        m_surfaceManager->draw(&m_shaderProgram);
        m_shaderProgram.release();
    }

    void OpenGlWidget::qtPaintRoutine(QPainter& painter) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        if (not m_surfaceManager->axes().isHidden()) {
            drawAxesLabels(painter);
        }
        painter.end();
    }

    void OpenGlWidget::drawAxesLabels(QPainter& painter) {
        painter.setPen(QPen{Qt::black});
        QFont font = painter.font();
        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(m_cameraManager->fromWorldToScreen(1.04 * m_surfaceManager->axes().xTip()), "x");
        painter.drawText(m_cameraManager->fromWorldToScreen(1.04 * m_surfaceManager->axes().yTip()), "y");
        painter.drawText(m_cameraManager->fromWorldToScreen(1.04 * m_surfaceManager->axes().zTip()), "z");
    }

    void OpenGlWidget::wheelEvent(QWheelEvent* event) {
        const float degrees = event->angleDelta().y() / 8.0f;
        m_cameraManager->zoom(degrees);
        m_cameraManager->setProjectionMatrix(width(), height());
        update();
    }
    CameraManager& OpenGlWidget::cameraWidget() {
        return *m_cameraManager;
    }
    SurfaceManager* OpenGlWidget::surfaceManager() const {
        return m_surfaceManager.get();
    }

} // namespace app