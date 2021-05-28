#include "App/FormulaSideBar.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication   a(argc, argv);
    FormulaSideBar w;
    w.show();
    return a.exec();
}