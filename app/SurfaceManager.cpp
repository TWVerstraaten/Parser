//
// Created by pc on 31-05-21.
//

#include "SurfaceManager.h"

#include <QOpenGLShaderProgram>

const app::Surface& app::SurfaceManager::at(size_t index) const {
    assert(index < m_surfaces.size());
    return *m_surfaces.at(index);
}

size_t app::SurfaceManager::size() const {
    return m_surfaces.size();
}

void app::SurfaceManager::add(std::unique_ptr<Surface>&& surface) {
    m_surfaces.push_back(std::move(surface));
}

void app::SurfaceManager::draw(QOpenGLShaderProgram* program) {
    if (not m_axes.isHidden()){
        m_axes.draw(program);
    }
    for (const auto& surface : m_surfaces) {
        if (not surface->isHidden()) {
            surface->draw(program);
        }
    }
}

void app::SurfaceManager::remove(size_t indexOfSurface) {

    auto it = std::find_if(m_surfaces.begin(), m_surfaces.end(), [indexOfSurface](const auto& a) { return a->index() == indexOfSurface; });
    if (it != m_surfaces.end()) {
        m_surfaces.erase(it);
    }
}
