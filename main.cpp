#include "app/FormulaSideBar.h"
#include "fml/Formula.h"
#include "fml/prs/Tokenizer.h"

#include <QApplication>

int main(int argc, char* argv[]) {

    //    fml::prs::Tokenizer tok("hf(2,3)+1");

    //    std::cout << tok.toString() << '\n';

    fml::Formula formula("f(x,y)= h(x+t(y),y*2,3-1)  ");
    if (formula.success()) {
        std::cout << formula.toProcessedString() << '\n';
    } else {
        std::cout << formula.errorString() << '\n';
    }
    //    QApplication        a(argc, argv);
    //    app::FormulaSideBar w;
    //    w.show();
    //    return a.exec();
}