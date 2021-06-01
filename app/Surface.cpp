//
// Created by pc on 29-05-21.
//

#include "Surface.h"

#include <QOpenGLShaderProgram>

namespace app {

    struct VertexData {
        QVector3D m_position;
        QVector3D m_colour;
    };

    Surface::Surface() : m_index(m_maxIndex), m_vertexBuffer(QOpenGLBuffer::VertexBuffer), m_indexBuffer(QOpenGLBuffer::IndexBuffer) {
        ++m_maxIndex;
        initializeOpenGLFunctions();

        m_vertexBuffer.create();
        m_indexBuffer.create();

        fillBuffers();
    }

    Surface::~Surface() {
        m_vertexBuffer.destroy();
        m_indexBuffer.destroy();
    }

    void Surface::fillBuffers() {
        m_vertexBuffer.release();
        m_indexBuffer.release();

        QVector<VertexData> vertices;
        QVector<GLuint>     indices;

        //        const size_t i_lim = 15;
        //        const size_t j_lim = 23;

        static float o = 0.0f;

        vertices.push_back({{0.0f + o, 0.0f + o, 0.0f + o}, {0.12, 0.51, 0.33}});
        vertices.push_back({{0.6f + o, 0.0f + o, 0.0f + o}, {0.12, 0.51, 0.13}});
        vertices.push_back({{0.0f + o, 0.6f + o, 0.0f + o}, {0.12, 0.21, 0.33}});
        vertices.push_back({{0.6f + o, 0.6f + o, 0.0f + o}, {0.72, 0.51, 0.33}});

        vertices.push_back({{0.0f + o, 0.0f + o, 0.6f + o}, {0.02, 0.01, 0.83}});
        vertices.push_back({{0.6f + o, 0.0f + o, 0.6f + o}, {0.12, 0.51, 0.63}});
        vertices.push_back({{0.0f + o, 0.6f + o, 0.6f + o}, {0.62, 0.51, 0.63}});
        vertices.push_back({{0.6f + o, 0.6f + o, 0.6f + o}, {0.22, 0.11, 0.63}});

        indices.push_back({0});
        indices.push_back({1});
        indices.push_back({2});
        indices.push_back({1});
        indices.push_back({2});
        indices.push_back({3});

        indices.push_back({4});
        indices.push_back({5});
        indices.push_back({6});
        indices.push_back({5});
        indices.push_back({6});
        indices.push_back({7});

        indices.push_back({0});
        indices.push_back({1});
        indices.push_back({4});
        indices.push_back({1});
        indices.push_back({4});
        indices.push_back({5});

        indices.push_back({2});
        indices.push_back({3});
        indices.push_back({6});
        indices.push_back({3});
        indices.push_back({6});
        indices.push_back({7});

        o += 0.6f;

        m_vertexBuffer.bind();
        m_vertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

        m_indexBuffer.bind();
        m_indexBuffer.allocate(indices.data(), indices.size() * sizeof(GLuint));
    }

    void Surface::draw(QOpenGLShaderProgram* program) {
        m_vertexBuffer.bind();
        m_indexBuffer.bind();

        quintptr  offset           = 0;
        const int positionLocation = program->attributeLocation("position");
        program->enableAttributeArray(positionLocation);
        program->setAttributeBuffer(positionLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        offset += sizeof(QVector3D);

        const int colorLocation = program->attributeLocation("color");
        program->enableAttributeArray(colorLocation);
        program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
    }

    bool Surface::isHidden() const {
        return m_isHidden;
    }

    void Surface::hide() {
        m_isHidden = true;
    }

    void Surface::show() {
        m_isHidden = false;
    }

    size_t Surface::index() const {
        return m_index;
    }
} // namespace app