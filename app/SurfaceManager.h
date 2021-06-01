//
// Created by pc on 31-05-21.
//

#ifndef PARSER_SURFACEMANAGER_H
#define PARSER_SURFACEMANAGER_H

#include "Axes.h"
#include "Surface.h"

#include <memory>
#include <vector>

class QOpenGLShaderProgram;

namespace app {

    class SurfaceManager {

      public:
        SurfaceManager() {
            m_surfaces.push_back(std::make_unique<Surface>());
            m_surfaces.push_back(std::make_unique<Surface>());
            m_surfaces.push_back(std::make_unique<Surface>());
        };

        void clear();
        void add(std::unique_ptr<Surface>&& surface);
        void remove(size_t indexOfSurface);
        void draw(QOpenGLShaderProgram* program);

        [[nodiscard]] size_t         size() const;
        [[nodiscard]] const Surface& at(size_t index) const;

      private:
        std::vector<std::unique_ptr<Surface>> m_surfaces;
        Axes                                  m_axes;
    };

} // namespace app

#endif // PARSER_SURFACEMANAGER_H
