//
// Created by pc on 31-05-21.
//

#ifndef APP_CENTRALWIDGET_H
#define APP_CENTRALWIDGET_H

#include <QWidget>
#include <memory>

class QHBoxLayout;

namespace app {
    class FormulaSurfaceInterface;
    class FormulaSideBar;
    class OpenGlWidget;

    class CentralWidget : public QWidget {
        Q_OBJECT
      public:
        explicit CentralWidget(QWidget* parent = nullptr);
        ~CentralWidget() override;

      protected:
        void keyPressEvent(QKeyEvent* event) override;

      private:
        OpenGlWidget*                            m_openGlWidget   = nullptr;
        FormulaSideBar*                          m_formulaSideBar = nullptr;
        QHBoxLayout*                             m_layout         = nullptr;
        std::unique_ptr<FormulaSurfaceInterface> m_formulaSurfaceInterface;
    };
} // namespace app
#endif // APP_CENTRALWIDGET_H
