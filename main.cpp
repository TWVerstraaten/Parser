#include "app/CentralWidget.h"
#include "gen/DependencyGraph.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
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

    QApplication       a(argc, argv);
    app::CentralWidget w;
    w.show();
    return a.exec();
}