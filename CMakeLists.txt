cmake_minimum_required(VERSION 3.17)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


add_executable(Parser main.cpp Parser/Parser.cpp Parser/Parser.h Parser/Ast.cpp Parser/Ast.h Parser/AstNode/AstNode.cpp Parser/AstNode/AstNode.h Parser/AstNode/AstNodeVar.cpp Parser/AstNode/AstNodeVar.h Parser/AstNode/AstNodeError.cpp Parser/AstNode/AstNodeError.h Parser/AstNode/AstNodeAdd.cpp Parser/AstNode/AstNodeAdd.h Parser/AstNode/AstNodePower.cpp Parser/AstNode/AstNodePower.h Parser/Tokenizer.cpp Parser/Tokenizer.h Parser/AstNode/AstNodeNumber.cpp Parser/AstNode/AstNodeNumber.h Parser/AstNode/AstNodeSubtract.cpp Parser/AstNode/AstNodeSubtract.h Parser/AstNode/AstNodeMul.cpp Parser/AstNode/AstNodeMul.h Parser/AstNode/AstNodeDiv.cpp Parser/AstNode/AstNodeDiv.h Parser/AstNode/AstNodeUnaryMinus.cpp Parser/AstNode/AstNodeUnaryMinus.h Parser/AstNode/AstNodeFunction.cpp Parser/AstNode/AstNodeFunction.h Parser/AstNode/AstNodeCommutative.cpp Parser/AstNode/AstNodeCommutative.h Algorithm/Algorithm.h Algorithm/Algorithm.cpp Parser/AstNode/Number.h Parser/AstNode/Number.cpp App/MainWindow.cpp App/MainWindow.h App/FormulaWidget.cpp App/FormulaWidget.h)

target_link_libraries(Parser Qt5::Core Qt5::Widgets)
