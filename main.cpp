#include "app/CentralWidget.h"
#include "ast/Ast.h"
#include "ast/UnrollManager.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    ast::UnrollManager m;
    ast::Ast           a1{"f(x)=t(x)"};
    ast::Ast           a2{"t(x)= x*2 "};

    m.add(0, a1);
    m.add(1, a2);
    m.unrollAll();
    std::cout << m.getUnrolledAt(0).toString() << '\n';
    m.remove(1);
    m.add(1, ast::Ast{"t(x) = 7*x"});
    m.unrollAll();
    std::cout << m.getUnrolledAt(0).toString() << '\n';


    //    m.unrollAll();
    //    m.add(2, ast::Ast{"g = 1"});
    //    m.unrollAll();

    //    std::cout << m.getUnrolledAt(0).toString() << '\n';
    //    std::cout << m.dependencyManager().toString() << "\n";
    //    const auto order = m.dependencyManager().unrollOrder();
    //    for (const auto el : order) {
    //        std::cout << el << "\t";
    //    }
    //    std::cout << "\n";

    //    m.unrollAll();
    //
    //    std::cout << m.getUnrolledAt(1).toString() << '\n';

    //    m.checkStatus(0);
    //    QApplication       a(argc, argv);
    //    app::CentralWidget w;
    //    w.show();
    //    return a.exec();
}
