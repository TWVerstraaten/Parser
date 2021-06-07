//#include "app/CentralWidget.h"
//#include "gen/DependencyGraph.h"
//
//#include <QApplication>
#include "prs/Tokenizer.h"

#include <iostream>

int main(int argc, char* argv[]) {
    Tokenizer tok("f2f (x + tt + 2,y + 9)");
    if (not tok.success()) {
        std::cout << tok.string() << '\n';
        const auto& errors = tok.errors();
        for (const auto& er : errors) {
            std::cout << er.toString() << '\n';
        }
    } else {
        std::cout << tok.structuralTokensToString() << '\n';
    }

    //    gen::DependencyGraph g;
    //
    //    g.addDependsOn("f", "h");
    //    g.addDependsOn("f", "g");
    //    g.addDependsOn("h", "t");
    //    g.addDependsOn("h", "g");
    //    g.addDependsOn("g", "t");
    //    g.addDependsOn("h", "k");
    //    g.addDependsOn("t", "f");
    //    g.write();
    //    const auto dep = g.getDependents("k");
    //    for (const auto& el : dep) {
    //        std::cout << el << "\t";
    //    }
    //    std::cout << "\n";
    //    std::cout << "Circular?\t" << g.hasCircularDependency("k") << '\n';
    //    std::cout << "Circular?\t" << g.hasCircularDependency("t") << '\n';
    //    std::cout << "Circular?\t" << g.hasCircularDependency("f") << '\n';
    //    std::cout << "Circular?\t" << g.hasCircularDependency("g") << '\n';
    //    std::cout << "Circular?\t" << g.hasCircularDependency("h") << '\n';
    //
    //    std::cout << g.edgeCount() << '\n';
    //    g.addDependsOn("k", "k");
    //    g.addDependsOn("k", "k");
    //    g.addDependsOn("k", "k");
    //    g.addDependsOn("k", "k");
    //    std::cout << g.edgeCount() << '\n';

    //    QApplication       a(argc, argv);
    //    app::CentralWidget w;
    //    w.show();
    //    return a.exec();
}