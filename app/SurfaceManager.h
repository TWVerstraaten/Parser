//
// Created by pc on 31-05-21.
//

#ifndef APP_SURFACEMANAGER_H
#define APP_SURFACEMANAGER_H

#include "Axes.h"

#include <memory>
#include <vector>

class QOpenGLShaderProgram;

namespace app {

    class Surface;

    class SurfaceManager {

      public:
        SurfaceManager();

        void clear();
        void add(std::unique_ptr<Surface>&& surface);
        void remove(size_t indexOfSurface);
        void draw(QOpenGLShaderProgram* program);

        [[nodiscard]] size_t         size() const;
        [[nodiscard]] const Surface& at(size_t index) const;
        [[nodiscard]] const Axes&    axes() const;

      private:
        std::vector<std::unique_ptr<Surface>> m_surfaces;
        Axes                                  m_axes;
    };

} // namespace app

#endif // APP_SURFACEMANAGER_H
