//
// Created by pc on 31-05-21.
//

#ifndef APP_CAMERAMANAGER_H
#define APP_CAMERAMANAGER_H

#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>

namespace app {
    class OpenGlWidget;

    class CameraManager {
      public:
        explicit CameraManager(OpenGlWidget* openGlWidget);

        [[nodiscard]] const QMatrix4x4& projectionMatrix() const;
        [[nodiscard]] QMatrix4x4        lookAtMatrix() const;
        [[nodiscard]] const QMatrix4x4& modelViewProjectionMatrix() const;

        void   rotateViewDirection(float dX, float dY);
        void   translateCameraPosition(float dX, float dY);
        void   rotateUpDirection(float dX, float dY);
        void   setProjectionMatrix(int w, int h);
        void   zoom(float degrees);
        void   toggleCameraMode();
        void   updateLookAtAndModelViewProjectionMatrix();
        QPoint fromWorldToScreen(const QVector3D& world);

      private:
        enum class MODE { PROJECTION, ORTHOGRAPHIC };

        [[nodiscard]] float     zoomParameterToRight(int w) const;
        [[nodiscard]] float     zoomParameterToTop(int h) const;
        [[nodiscard]] QVector3D lookAtToCamera() const;

        MODE                m_mode              = MODE::ORTHOGRAPHIC;
        float               m_verticalAngle     = 54.0f;
        float               m_nearClippingPlane = 0.01f;
        float               m_farClippingPlane  = 1000.0f;
        float               m_zoomParameter     = -2.0f;
        float               m_zoomSensitivity   = 0.021;
        QVector3D           m_lookAt            = {0, 0, 0};
        QVector3D           m_cameraPosition    = {0.0f, 0.0f, 5.0f};
        QVector3D           m_cameraUpDirection = {0.0f, 1.0f, 0.0f};
        QMatrix4x4          m_projectionMatrix;
        QMatrix4x4          m_lookAtMatrix;
        QMatrix4x4          m_modelViewProjectionMatrix;
        const OpenGlWidget* m_openGlWidget;
    };

} // namespace app

#endif // APP_CAMERAMANAGER_H
