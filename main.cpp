#include "app/CentralWidget.h"
#include "ast/Ast.h"
#include "ast/UnrolledAst.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
//   std::string cString = "f(x)=2.120 + 3";
//    ast::Ast a{cString};
//    std::cout << a.toStringFlat() << "\n";
//
    QApplication       a(argc, argv);
    app::CentralWidget w;
    w.show();
    return a.exec();
}
