//
// Created by pc on 29-05-21.
//

#include "OpenGlWidget.h"

#include <QMouseEvent>
#include <cmath>

namespace app {

    OpenGlWidget::OpenGlWidget(QWidget* parent) : QOpenGLWidget(parent) {
        m_cameraUpDirection -= (QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection) * m_viewDirection).normalized();
        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);
    }

    OpenGlWidget::~OpenGlWidget() {
        makeCurrent();
        doneCurrent();
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
                rotateViewDirection(dX, dY);
                break;
            case CLICK_STATE::RIGHT:
                translateCameraPosition(dX, dY);
                break;
            case CLICK_STATE::MIDDLE:
                rotateUpDirection(dX, dY);
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
        //        qreal aspect = qreal(w) / qreal(h ? h : 1);
        //
        const qreal zNear = 1.0;
        const qreal zFar  = 1000.0;
        //        const qreal fov   = 54.0;

        m_projectionMatrix.setToIdentity();
        //        m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
        const float right = static_cast<float>(w) * std::exp(m_zoomParameter) * 0.002f;
        const float top   = static_cast<float>(h) * std::exp(m_zoomParameter) * 0.002f;
        m_projectionMatrix.ortho(-right, right, -top, top, zNear, zFar);
    }

    void OpenGlWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setProjectionMatrix(width(), height());
        QMatrix4x4 lookAtMatrix;
        lookAtMatrix.lookAt(m_cameraPosition, m_cameraPosition + m_viewDirection, m_cameraUpDirection);
        m_shaderProgram.setUniformValue("mvp_matrix", m_projectionMatrix * lookAtMatrix);
        m_surfaceManager->draw(&m_shaderProgram);
    }

    void OpenGlWidget::wheelEvent(QWheelEvent* event) {
        const float degrees = event->angleDelta().y() / 8.0f;
        if (degrees != 0.0f) {
            m_cameraPosition += m_zoomSensitivity * degrees * m_viewDirection;
            m_zoomParameter -= m_zoomSensitivity * degrees;
        }

        update();
    }

    void OpenGlWidget::rotateViewDirection(float dX, float dY) {
        const auto normal = QVector3D::crossProduct(m_viewDirection, m_cameraUpDirection).normalized();
        QMatrix4x4 rotation;
        rotation.setToIdentity();
        rotation.rotate(-0.08f * dX, m_cameraUpDirection);
        rotation.rotate(-0.08f * dY, normal);
        m_viewDirection     = m_viewDirection * rotation;
        m_cameraUpDirection = m_cameraUpDirection * rotation;
        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);
    }

    void OpenGlWidget::translateCameraPosition(float dX, float dY) {
        const auto normal = QVector3D::crossProduct(m_viewDirection, m_cameraUpDirection).normalized();
        m_cameraPosition -= dX * normal * 2.0f * zoomParameterToLeft(width()) / width();
        m_cameraPosition += dY * m_cameraUpDirection * 2.0f * zoomParameterToTop(height()) / height();
    }

    void OpenGlWidget::rotateUpDirection(float dX, float dY) {
        QMatrix4x4 rotation;
        rotation.setToIdentity();
        rotation.rotate(-0.08f * dX, m_viewDirection);
        m_cameraUpDirection = m_cameraUpDirection * rotation;
        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);
    }

    void OpenGlWidget::setProjectionMatrix(int w, int h) {
        const qreal zNear = 1.0;
        const qreal zFar  = 1000.0;
        //        const qreal fov   = 54.0;

        m_projectionMatrix.setToIdentity();
        //        m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
        const auto right = zoomParameterToLeft(w);
        const auto top   = zoomParameterToTop(h);
        m_projectionMatrix.ortho(-right, right, -top, top, zNear, zFar);
    }

    float OpenGlWidget::zoomParameterToLeft(int w) const {
        return w * std::exp(m_zoomParameter) * 0.002;
    }

    float OpenGlWidget::zoomParameterToTop(int h) const {
        return h * std::exp(m_zoomParameter) * 0.002;
    }

} // namespace app