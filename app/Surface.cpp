//
// Created by pc on 29-05-21.
//

#include "Surface.h"

#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <cmath>

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

        const size_t i_lim = 15;
        const size_t j_lim = 23;

        static float o = 0.0f;

        for (size_t i = 0; i != i_lim; ++i) {
            for (size_t j = 0; j != j_lim; ++j) {
                vertices.push_back(
                    {{o - 0.5f + 0.2f * i, o - 0.5f + 0.2f * j, o + std::abs(0.2f * i)}, {0.15f * i, 0.15f * j, 0.02f * i + 0.04f * j}});
            }
        }
        o -= 1.0f;

        for (size_t i = 0; i != i_lim - 1; ++i) {
            for (size_t j = 0; j != j_lim - 1; ++j) {
                indices.push_back(j + 1 + j_lim * i);
                indices.push_back(j + j_lim * i);
                indices.push_back(j + j_lim * (i + 1));

                indices.push_back(j + 1 + j_lim * i);
                indices.push_back(j + j_lim * (i + 1));
                indices.push_back(j + 1 + j_lim * (i + 1));
            }
        }

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

        glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
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