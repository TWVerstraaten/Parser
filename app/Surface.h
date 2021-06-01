//
// Created by pc on 29-05-21.
//

#ifndef PARSER_SURFACE_H
#define PARSER_SURFACE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

namespace fml {
    class Formula;
}

namespace app {
    class Surface : protected QOpenGLFunctions {
      public:
        Surface();
        virtual ~Surface();

        void draw(QOpenGLShaderProgram* program);
        void hide();
        void show();
        void fillBuffers(const fml::Formula& formula);

        [[nodiscard]] size_t index() const;
        [[nodiscard]] bool   isHidden() const;

      private:
        void fillBuffers_testSurface();

        inline static size_t m_maxIndex = 0;
        const size_t         m_index;

        bool          m_isHidden = false;
        QOpenGLBuffer m_vertexBuffer;
        QOpenGLBuffer m_indexBuffer;
    };
} // namespace app

#endif // PARSER_SURFACE_H
