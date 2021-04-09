#include "App/FormulaWidget.h"
#include "App/MainWindow.h"
#include "Parser/Ast.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    std::string eq1 = "xs * y + z00as*(1 + x)";
    Ast         ast1(eq1);
    std::cout << ast1.toString() << '\n';
    auto copyNode1 = ast1.differentiate("x");
    std::cout << "Derivative wrt x:\t" << copyNode1.toString() << '\n';

    const auto c = copyNode1.variables();
    for (const auto& a : c) {
        std::cout << a << "\t";
    }

    QApplication  a(argc, argv);
    FormulaWidget w;
    w.show();
    return a.exec();
}
