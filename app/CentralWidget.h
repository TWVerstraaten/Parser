//
// Created by pc on 31-05-21.
//

#ifndef PARSER_CENTRALWIDGET_H
#define PARSER_CENTRALWIDGET_H

#include <QWidget>

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
        FormulaSideBar* m_formulaSideBar = nullptr;
        OpenGlWidget*   m_openGlWidget   = nullptr;
        QHBoxLayout*    m_layout         = nullptr;
    };
} // namespace app
#endif // PARSER_CENTRALWIDGET_H
