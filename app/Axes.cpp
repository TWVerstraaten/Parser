//
// Created by pc on 31-05-21.
//

#include "Axes.h"

#include <QOpenGLShaderProgram>

namespace app {

    struct VertexData {
        QVector3D m_position;
        QVector3D m_colour;
    };

    Axes::Axes() : m_vertexBuffer(QOpenGLBuffer::VertexBuffer), m_indexBuffer(QOpenGLBuffer::IndexBuffer) {
        initializeOpenGLFunctions();

        m_vertexBuffer.create();
        m_indexBuffer.create();

        fillBuffers();
    }

    Axes::~Axes() {
        m_vertexBuffer.destroy();
        m_indexBuffer.destroy();
    }

    void Axes::fillBuffers() {
        m_vertexBuffer.release();
        m_indexBuffer.release();

        QVector<VertexData> vertices;
        QVector<GLuint>     indices;

        vertices.push_back({m_origin, {1.0f, 0.0f, 0.0f}});
        vertices.push_back({m_xTip, {1.0f, 0.0f, 0.0f}});
        vertices.push_back({m_origin, {0.0f, 1.0f, 0.0f}});
        vertices.push_back({m_yTip, {0.0f, 1.0f, 0.0f}});
        vertices.push_back({m_origin, {0.0f, 0.0f, 1.0f}});
        vertices.push_back({m_zTip, {0.0f, 0.0f, 1.0f}});

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);

        m_vertexBuffer.bind();
        m_vertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

        m_indexBuffer.bind();
        m_indexBuffer.allocate(indices.data(), indices.size() * sizeof(GLuint));

        m_vertexBuffer.release();
        m_indexBuffer.release();
    }

    void Axes::draw(QOpenGLShaderProgram* program) {
        m_vertexBuffer.bind();
        m_indexBuffer.bind();

        qintptr   offset           = 0;
        const int positionLocation = program->attributeLocation("position");
        program->enableAttributeArray(positionLocation);
        program->setAttributeBuffer(positionLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        offset += sizeof(QVector3D);
        const int colorLocation = program->attributeLocation("color");
        program->enableAttributeArray(colorLocation);
        program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
        glLineWidth(2.0f);
        glDrawElements(GL_LINES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

        m_indexBuffer.release();
        m_vertexBuffer.release();
    }

    bool Axes::isHidden() const {
        return m_isHidden;
    }

    void Axes::hide() {
        m_isHidden = true;
    }

    void Axes::show() {
        m_isHidden = false;
    }

    const QVector3D& Axes::origin() const {
        return m_origin;
    }

    const QVector3D& Axes::xTip() const {
        return m_xTip;
    }

    const QVector3D& Axes::yTip() const {
        return m_yTip;
    }

    const QVector3D& Axes::zTip() const {
        return m_zTip;
    }

} // namespace app