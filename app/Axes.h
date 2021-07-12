//
// Created by pc on 31-05-21.
//

#ifndef APP_AXES_H
#define APP_AXES_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QVector3D>

class QOpenGLShaderProgram;

namespace app {
    class Axes : protected QOpenGLFunctions {
      public:
        Axes();
        virtual ~Axes();

        void draw(QOpenGLShaderProgram* program);
        void hide();
        void show();

        [[nodiscard]] const QVector3D& origin() const;
        [[nodiscard]] const QVector3D& xTip() const;
        [[nodiscard]] const QVector3D& yTip() const;
        [[nodiscard]] const QVector3D& zTip() const;
        [[nodiscard]] bool             isHidden() const;

      private:
        void fillBuffers();

        bool          m_isHidden = false;
        QOpenGLBuffer m_vertexBuffer;
        QOpenGLBuffer m_indexBuffer;

        QVector3D m_origin = {0, 0, 0};
        QVector3D m_xTip   = {2, 0, 0};
        QVector3D m_yTip   = {0, 2, 0};
        QVector3D m_zTip   = {0, 0, 2};
    };
} // namespace app

#endif // APP_AXES_H
