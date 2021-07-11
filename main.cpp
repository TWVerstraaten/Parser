#include "app/CentralWidget.h"
#include "ast/AstManager.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    ast::Ast a1{"f(y)=2*(1+2) -4"};
    //    std::cout << a1.toStringFlat() << "\t csts:\t";
    //    for (const auto& el : a1.constantDependencies()) {
    //        std::cout << el << "\t";
    //    }
    //    std::cout << "\n\n";
    std::cout << a1.toStringAsTree() << '\n';

    //    AstManager m;
    //
    //    m.add(0, Ast{"h(x) = tan(x)"});
    //    m.add(1, Ast{"f(x,y) = sin(2 * h(x))"});
    //    m.add(2, Ast{"2 * h(x+y)"});
    //
    //    std::cout << m.toString() << '\n';

    QApplication       a(argc, argv);
    app::CentralWidget w;
    w.show();
    return a.exec();
}