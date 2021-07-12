//
// Created by pc on 31-05-21.
//

#include "CameraManager.h"

#include "OpenGlWidget.h"

#include <cmath>

namespace app {

    CameraManager::CameraManager(OpenGlWidget* openGlWidget) : m_openGlWidget(openGlWidget) {
        m_cameraUpDirection -= (QVector3D::dotProduct(m_cameraUpDirection, m_cameraPosition - m_lookAt) * (m_cameraPosition - m_lookAt)).normalized();
    }

    void CameraManager::rotateViewDirection(float dX, float dY) {
        const auto viewDirection = lookAtToCamera();
        const auto normal        = QVector3D::crossProduct(viewDirection, m_cameraUpDirection).normalized();

        QMatrix4x4 rotation;
        rotation.setToIdentity();
        rotation.rotate(0.2f * dX, m_cameraUpDirection);
        rotation.rotate(-0.2f * dY, normal);
        auto newViewDirection = viewDirection;
        newViewDirection      = newViewDirection * rotation;
        m_cameraUpDirection   = m_cameraUpDirection * rotation;
        m_cameraPosition      = m_lookAt + newViewDirection;
        updateLookAtAndModelViewProjectionMatrix();
    }

    void CameraManager::translateCameraPosition(float dX, float dY) {
        const auto normal      = QVector3D::crossProduct(lookAtToCamera(), m_cameraUpDirection).normalized();
        const auto translation = 2.0 * (dX * normal * zoomParameterToRight(m_openGlWidget->width()) / m_openGlWidget->width() +
                                        dY * m_cameraUpDirection * zoomParameterToTop(m_openGlWidget->height()) / m_openGlWidget->height());
        m_cameraPosition += 0.02 * translation;
        m_lookAt += 0.02 * translation;
        updateLookAtAndModelViewProjectionMatrix();
    }

    void CameraManager::rotateUpDirection(float dX, float dY) {
        //        QMatrix4x4 rotation;
        //        rotation.setToIdentity();
        //        rotation.rotate(-0.08f * dX, m_viewDirection);
        //        m_cameraUpDirection = m_cameraUpDirection * rotation;
        //        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);
    }

    QVector3D CameraManager::lookAtToCamera() const {
        return m_cameraPosition - m_lookAt;
    }

    void CameraManager::setProjectionMatrix(int w, int h) {
        const float aspect = static_cast<float>(w) / static_cast<float>(h ? h : 1.0);

        m_projectionMatrix.setToIdentity();
        switch (m_mode) {
            case MODE::PROJECTION:
                m_projectionMatrix.perspective(m_verticalAngle, aspect, m_nearClippingPlane, m_farClippingPlane);
                break;
            case MODE::ORTHOGRAPHIC: {
                const float top   = std::tan(0.5 * m_verticalAngle * M_PI / 180.0) * lookAtToCamera().length();
                const float right = top * aspect;
                m_projectionMatrix.ortho(-right, right, -top, top, m_nearClippingPlane, m_farClippingPlane);
            } break;
        }
        updateLookAtAndModelViewProjectionMatrix();
    }

    float CameraManager::zoomParameterToRight(int w) const {
        return static_cast<float>(w) * std::exp(m_zoomParameter);
    }

    float CameraManager::zoomParameterToTop(int h) const {
        return static_cast<float>(h) * std::exp(m_zoomParameter);
    }

    const QMatrix4x4& CameraManager::projectionMatrix() const {
        return m_projectionMatrix;
    }

    void CameraManager::zoom(float degrees) {
        if (degrees != 0.0f) {
            m_cameraPosition -= m_zoomSensitivity * degrees * lookAtToCamera();
            m_zoomParameter -= m_zoomSensitivity * degrees;
        }
        updateLookAtAndModelViewProjectionMatrix();
    }

    QMatrix4x4 CameraManager::lookAtMatrix() const {
        return m_lookAtMatrix;
    }

    void CameraManager::toggleCameraMode() {
        m_mode = m_mode == MODE::PROJECTION ? MODE::ORTHOGRAPHIC : MODE::PROJECTION;
    }

    QPoint CameraManager::fromWorldToScreen(const QVector3D& world) {
        QVector4D clipSpace = m_modelViewProjectionMatrix * QVector4D{world, 1};

        return {static_cast<int>((1 + clipSpace.x() / clipSpace.w()) * m_openGlWidget->width() / 2),
                static_cast<int>((1 - clipSpace.y() / clipSpace.w()) * m_openGlWidget->height() / 2)};
    }

    const QMatrix4x4& CameraManager::modelViewProjectionMatrix() const {
        return m_modelViewProjectionMatrix;
    }

    void CameraManager::updateLookAtAndModelViewProjectionMatrix() {
        m_lookAtMatrix = QMatrix4x4{};
        m_lookAtMatrix.lookAt(m_cameraPosition, m_lookAt, m_cameraUpDirection);
        m_modelViewProjectionMatrix = m_projectionMatrix * m_lookAtMatrix;
    }

} // namespace app
