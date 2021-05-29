#include "app/FormulaSideBar.h"
#include "fml/Formula.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    fml::Formula formula("f(x,y)=x+y+3 + g(x)");

    std::cout << formula.toProcessedString() << '\n';

    //    QApplication        a(argc, argv);
    //    app::FormulaSideBar w;
    //    w.show();
    //    return a.exec();
}