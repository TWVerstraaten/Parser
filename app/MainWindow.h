//
// Created by pc on 09-04-21.
//

#ifndef PARSER_MAINWINDOW_H
#define PARSER_MAINWINDOW_H

#include <QMainWindow>

namespace app {
    class MainWindow : public QMainWindow {
        Q_OBJECT

      public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow() override;
    };
} // namespace app

#endif // PARSER_MAINWINDOW_H
