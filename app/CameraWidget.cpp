//
// Created by pc on 31-05-21.
//

#include "CameraWidget.h"

#include <QDoubleSpinBox>
#include <QLabel>
#include <cmath>
#include <iostream>

namespace app {

    CameraWidget::CameraWidget(QWidget* parent) : QWidget(parent) {
        m_cameraUpDirection -= (QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection) * m_viewDirection).normalized();
        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);

        m_layout      = new QFormLayout(this);
        auto* spinBox = new QDoubleSpinBox(this);
        spinBox->setMinimum(-20.0f);
        spinBox->setMaximum(1.0f);
        spinBox->setSingleStep(0.1f);
        spinBox->setValue(m_zoomParameter);
        connect(spinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this](double a) {
            m_zoomParameter = a;
            emit updated();
        });

        m_layout->addRow(new QLabel("zmPar"), spinBox);
    }

    void CameraWidget::rotateViewDirection(float dX, float dY) {
        const auto normal = QVector3D::crossProduct(m_viewDirection, m_cameraUpDirection).normalized();
        QMatrix4x4 rotation;
        rotation.setToIdentity();
        rotation.rotate(-0.08f * dX, m_cameraUpDirection);
        rotation.rotate(-0.08f * dY, normal);
        m_viewDirection     = m_viewDirection * rotation;
        m_cameraUpDirection = m_cameraUpDirection * rotation;
        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);
    }

    void CameraWidget::translateCameraPosition(float dX, float dY) {
        const auto normal = QVector3D::crossProduct(m_viewDirection, m_cameraUpDirection).normalized();
        m_cameraPosition -= dX * normal * 2.0f * zoomParameterToLeft(width()) / width();
        m_cameraPosition += dY * m_cameraUpDirection * 2.0f * zoomParameterToTop(height()) / height();
    }

    void CameraWidget::rotateUpDirection(float dX, float dY) {
        QMatrix4x4 rotation;
        rotation.setToIdentity();
        rotation.rotate(-0.08f * dX, m_viewDirection);
        m_cameraUpDirection = m_cameraUpDirection * rotation;
        assert(std::abs(QVector3D::dotProduct(m_cameraUpDirection, m_viewDirection)) < 0.01f);
    }

    void CameraWidget::setProjectionMatrix(int w, int h) {
        //        const qreal aspect = qreal(w) / qreal(h ? h : 1);

        const qreal zNear = 0.2;
        const qreal zFar  = 1000.0;
        //        const qreal fov   = 54.0;

        m_projectionMatrix.setToIdentity();
        //        m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
        const auto right = zoomParameterToLeft(w);
        const auto top   = zoomParameterToTop(h);
        m_projectionMatrix.ortho(-right, right, -top, top, zNear, zFar);
    }

    float CameraWidget::zoomParameterToLeft(int w) const {
        return static_cast<float>(w) * std::exp(m_zoomParameter);
    }

    float CameraWidget::zoomParameterToTop(int h) const {
        return static_cast<float>(h) * std::exp(m_zoomParameter);
    }

    const QMatrix4x4& CameraWidget::projectionMatrix() const {
        return m_projectionMatrix;
    }

    void CameraWidget::zoom(float degrees) {
        if (degrees != 0.0f) {
            m_cameraPosition += m_zoomSensitivity * degrees * m_viewDirection;
            m_zoomParameter -= m_zoomSensitivity * degrees;
        }
    }

    QMatrix4x4 CameraWidget::lookAtMatrix() const {
        QMatrix4x4 lookAtMatrix;
        lookAtMatrix.lookAt(m_cameraPosition, m_cameraPosition + m_viewDirection, m_cameraUpDirection);
        return lookAtMatrix;
    }
} // namespace app
