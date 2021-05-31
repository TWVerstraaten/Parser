//
// Created by pc on 31-05-21.
//

#ifndef PARSER_CAMERAWIDGET_H
#define PARSER_CAMERAWIDGET_H

#include <QFormLayout>
#include <QMatrix4x4>
#include <QVector3D>
#include <QWidget>

namespace app {
    class CameraWidget : public QWidget {
        Q_OBJECT

      public:
        explicit CameraWidget(QWidget* parent = nullptr);

        [[nodiscard]] const QMatrix4x4& projectionMatrix() const;
        [[nodiscard]] QMatrix4x4        lookAtMatrix() const;

        void rotateViewDirection(float dX, float dY);
        void translateCameraPosition(float dX, float dY);
        void rotateUpDirection(float dX, float dY);
        void setProjectionMatrix(int w, int h);
        void zoom(float degrees);

      signals:
        void updated();

      private:
        [[nodiscard]] float zoomParameterToLeft(int w) const;
        [[nodiscard]] float zoomParameterToTop(int h) const;

        float        m_zoomParameter{-4.0f};
        float        m_zoomSensitivity{0.021};
        QMatrix4x4   m_projectionMatrix;
        QVector3D    m_cameraPosition{0.0f, 0.0f, 5.0f};
        QVector3D    m_viewDirection{0.0f, 0.0f, -5.0f};
        QVector3D    m_cameraUpDirection{0.0f, 1.0f, 0.0f};
        QFormLayout* m_layout{nullptr};
    };

} // namespace app

#endif // PARSER_CAMERAWIDGET_H
