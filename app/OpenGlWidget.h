//
// Created by pc on 29-05-21.
//

#ifndef PARSER_OPENGLWIDGET_H
#define PARSER_OPENGLWIDGET_H

#include "Surface.h"
#include "SurfaceManager.h"

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>

namespace app {
    class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions {
        Q_OBJECT

      public:
        explicit OpenGlWidget(QWidget* parent = nullptr);
        ~OpenGlWidget() override;

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void wheelEvent(QWheelEvent* event) override;

        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

        void initShaders();

      private:
        enum class CLICK_STATE { NONE, RIGHT, LEFT, MIDDLE };

        void rotateViewDirection(float dX, float dY);
        void translateCameraPosition(float dX, float dY);
        void rotateUpDirection(float dX, float dY);
        void setProjectionMatrix(int w, int h);
        [[nodiscard]] float zoomParameterToLeft(int w) const;
        [[nodiscard]] float zoomParameterToTop(int h) const;


        float                           m_zoomParameter      = 2.0f;
        CLICK_STATE                     m_clickState         = CLICK_STATE::NONE;
        QOpenGLShaderProgram            m_shaderProgram;
        std::unique_ptr<SurfaceManager> m_surfaceManager;
        QMatrix4x4                      m_projectionMatrix;
        QVector2D                       m_mousePressPosition;
        QVector3D                       m_cameraPosition{0.0f, 0.0f, 5.0f};
        QVector3D                       m_viewDirection{0.0f, 0.0f, -5.0f};
        QVector3D                       m_cameraUpDirection{0.0f, 1.0f, 0.0f};

        float m_zoomSensitivity = 0.021;
    };
} // namespace app

#endif // PARSER_OPENGLWIDGET_H
