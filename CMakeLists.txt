cmake_minimum_required(VERSION 3.17)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


add_executable(Parser main.cpp Parser/Parser.cpp Parser/Parser.h Parser/Ast.cpp Parser/Ast.h Parser/AstNode/AstNode.cpp Parser/AstNode/AstNode.h Parser/AstNode/AstNodeVar.cpp Parser/AstNode/AstNodeVar.h Parser/AstNode/AstNodeError.cpp Parser/AstNode/AstNodeError.h Parser/AstNode/AstNodeAdd.cpp Parser/AstNode/AstNodeAdd.h Parser/AstNode/AstNodePower.cpp Parser/AstNode/AstNodePower.h Parser/Tokenizer.cpp Parser/Tokenizer.h Parser/AstNode/AstNodeNumber.cpp Parser/AstNode/AstNodeNumber.h Parser/AstNode/AstNodeSubtract.cpp Parser/AstNode/AstNodeSubtract.h Parser/AstNode/AstNodeMul.cpp Parser/AstNode/AstNodeMul.h Parser/AstNode/AstNodeDiv.cpp Parser/AstNode/AstNodeDiv.h Parser/AstNode/AstNodeUnaryMinus.cpp Parser/AstNode/AstNodeUnaryMinus.h Parser/AstNode/AstNodeFunction.cpp Parser/AstNode/AstNodeFunction.h Parser/AstNode/AstNodeCommutative.cpp Parser/AstNode/AstNodeCommutative.h Parser/Number.h Parser/Number.cpp App/MainWindow.cpp App/MainWindow.h App/FormulaWidget.cpp App/FormulaWidget.h Parser/FormulaHeader.cpp Parser/FormulaHeader.h Parser/Formula.cpp Parser/Formula.h Parser/ParserException.cpp Parser/ParserException.h Parser/ErrorBase.cpp Parser/ErrorBase.h Algorithm/BoostWrapper.cpp Algorithm/BoostWrapper.h App/FormulaSideBar.cpp App/FormulaSideBar.h Command/NewFormulaWidgetCommand.cpp Command/NewFormulaWidgetCommand.h Command/RemoveFormulaWidgetCommand.cpp Command/RemoveFormulaWidgetCommand.h App/UndoRedoConsumer.cpp App/UndoRedoConsumer.h App/UndoRedoHandler.cpp App/UndoRedoHandler.h Command/SkipFirstRedoCommand.h Command/FormulaChangedCommand.cpp Command/FormulaChangedCommand.h Algorithm/StringAlg.cpp Algorithm/StringAlg.h)

target_link_libraries(Parser Qt5::Core Qt5::Widgets)
