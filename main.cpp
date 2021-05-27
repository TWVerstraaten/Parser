#include "App/FormulaWidget.h"
#include "Parser/Formula.h"
#include "Parser/FormulaHeader.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication  a(argc, argv);
    FormulaWidget w;
    w.show();
    return a.exec();
}