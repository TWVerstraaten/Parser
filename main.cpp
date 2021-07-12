#include "app/CentralWidget.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication       a(argc, argv);
    app::CentralWidget w;
    w.show();
    return a.exec();
}