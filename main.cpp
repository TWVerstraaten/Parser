#include "app/CentralWidget.h"
#include "app/FormulaSideBar.h"
#include "app/OpenGlWidget.h"
#include "fml/Formula.h"
#include "fml/prs/Tokenizer.h"

#include <QApplication>

int main(int argc, char* argv[]) {

    QApplication       a(argc, argv);
    app::CentralWidget w;
    w.show();
    return a.exec();
}