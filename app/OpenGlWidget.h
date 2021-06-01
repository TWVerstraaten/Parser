//
// Created by pc on 29-05-21.
//

#ifndef PARSER_OPENGLWIDGET_H
#define PARSER_OPENGLWIDGET_H

#include "CameraManager.h"
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

        [[nodiscard]] CameraManager&  cameraWidget();
        [[nodiscard]] SurfaceManager* surfaceManager() const;

      protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void mouseReleaseEvent([[maybe_unused]] QMouseEvent* e) override;
        void wheelEvent(QWheelEvent* event) override;
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void initShaders();

      private:
        enum class CLICK_STATE { NONE, RIGHT, LEFT, MIDDLE };

        CLICK_STATE                     m_clickState = CLICK_STATE::NONE;
        QVector2D                       m_mousePressPosition;
        QOpenGLShaderProgram            m_shaderProgram;
        std::unique_ptr<CameraManager>  m_cameraManager;
        std::unique_ptr<SurfaceManager> m_surfaceManager;
    };
} // namespace app

#endif // PARSER_OPENGLWIDGET_H
