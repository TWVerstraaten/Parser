#include "app/FormulaSideBar.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication        a(argc, argv);
    app::FormulaSideBar w;
    w.show();
    return a.exec();
}