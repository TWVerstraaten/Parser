//
// Created by pc on 31-05-21.
//

#ifndef APP_AXES_H
#define APP_AXES_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

namespace app {
    class Axes : protected QOpenGLFunctions {
      public:
        Axes();
        virtual ~Axes();

        void draw(QOpenGLShaderProgram* program);
        void hide();
        void show();

        [[nodiscard]] bool isHidden() const;

      private:
        void fillBuffers();

        bool          m_isHidden = false;
        QOpenGLBuffer m_vertexBuffer;
        QOpenGLBuffer m_indexBuffer;
    };
} // namespace app

#endif // APP_AXES_H
