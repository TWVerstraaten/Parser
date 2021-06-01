//
// Created by pc on 31-05-21.
//

#ifndef PARSER_CENTRALWIDGET_H
#define PARSER_CENTRALWIDGET_H

#include "FormulaSurfaceInterface.h"

#include <QWidget>
#include <memory>

class QHBoxLayout;

namespace app {
    class FormulaSideBar;
    class OpenGlWidget;

    class CentralWidget : public QWidget {
        Q_OBJECT
      public:
        explicit CentralWidget(QWidget* parent = nullptr);

      protected:
        void keyPressEvent(QKeyEvent* event) override;

      private:
        OpenGlWidget*                            m_openGlWidget   = nullptr;
        FormulaSideBar*                          m_formulaSideBar = nullptr;
        QHBoxLayout*                             m_layout         = nullptr;
        std::unique_ptr<FormulaSurfaceInterface> m_formulaSurfaceInterface;
    };
} // namespace app
#endif // PARSER_CENTRALWIDGET_H
