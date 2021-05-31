//
// Created by pc on 29-05-21.
//

#ifndef PARSER_OPENGLWIDGET_H
#define PARSER_OPENGLWIDGET_H

#include "CameraWidget.h"
#include "SurfaceManager.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector2D>

namespace app {
    class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions {
        Q_OBJECT

      public:
        explicit OpenGlWidget(QWidget* parent = nullptr);
        ~OpenGlWidget() override = default;

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

        CameraWidget*                   m_cameraWidget = nullptr;
        CLICK_STATE                     m_clickState   = CLICK_STATE::NONE;
        QOpenGLShaderProgram            m_shaderProgram;
        std::unique_ptr<SurfaceManager> m_surfaceManager;
        QVector2D                       m_mousePressPosition;
    };
} // namespace app

#endif // PARSER_OPENGLWIDGET_H
