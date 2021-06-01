//
// Created by pc on 01-06-21.
//

#ifndef PARSER_FORMULASURFACEINTERFACE_H
#define PARSER_FORMULASURFACEINTERFACE_H

#include <QObject>

namespace app {
    class OpenGlWidget;
    class FormulaSideBar;

    class FormulaSurfaceInterface : public QObject {
        Q_OBJECT

      public:
        FormulaSurfaceInterface(OpenGlWidget* surfaceManager, FormulaSideBar* formulaSideBar);

      public slots:
        void updateSurfaces();

      private:
        OpenGlWidget*   m_openGlWidget;
        FormulaSideBar* m_formulaSideBar;
    };
} // namespace app

#endif // PARSER_FORMULASURFACEINTERFACE_H
