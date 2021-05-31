cmake_minimum_required(VERSION 3.17)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(Parser main.cpp app/Resources/shaders.qrc fml/prs/Parser.cpp fml/prs/Parser.h ast/Ast.cpp ast/Ast.h ast/AstNode.cpp ast/AstNode.h ast/AstNodeVar.cpp ast/AstNodeVar.h ast/AstNodeError.cpp ast/AstNodeError.h ast/AstNodeAdd.cpp ast/AstNodeAdd.h ast/AstNodePower.cpp ast/AstNodePower.h fml/prs/Tokenizer.cpp fml/prs/Tokenizer.h ast/AstNodeNumber.cpp ast/AstNodeNumber.h ast/AstNodeSubtract.cpp ast/AstNodeSubtract.h ast/AstNodeMul.cpp ast/AstNodeMul.h ast/AstNodeDiv.cpp ast/AstNodeDiv.h ast/AstNodeUnaryMinus.cpp ast/AstNodeUnaryMinus.h ast/AstNodeFunction.cpp ast/AstNodeFunction.h ast/AstNodeCommutative.cpp ast/AstNodeCommutative.h gen/Number.h gen/Number.cpp app/MainWindow.cpp app/MainWindow.h app/FormulaWidget.cpp app/FormulaWidget.h fml/FormulaHeader.cpp fml/FormulaHeader.h fml/Formula.cpp fml/Formula.h fml/prs/ParserException.cpp fml/prs/ParserException.h gen/ErrorBase.cpp gen/ErrorBase.h alg/BoostWrapper.cpp alg/BoostWrapper.h app/FormulaSideBar.cpp app/FormulaSideBar.h app/cmd/NewFormulaWidgetCommand.cpp app/cmd/NewFormulaWidgetCommand.h app/cmd/RemoveFormulaWidgetCommand.cpp app/cmd/RemoveFormulaWidgetCommand.h app/UndoRedoConsumer.cpp app/UndoRedoConsumer.h app/UndoRedoHandler.cpp app/UndoRedoHandler.h app/cmd/SkipFirstRedoCommand.h app/cmd/FormulaChangedCommand.cpp app/cmd/FormulaChangedCommand.h alg/StringAlg.cpp alg/StringAlg.h app/Surface.cpp app/Surface.h app/OpenGlWidget.cpp app/OpenGlWidget.h app/SurfaceManager.cpp app/SurfaceManager.h app/Axes.cpp app/Axes.h app/CameraWidget.cpp app/CameraWidget.h app/CentralWidget.cpp app/CentralWidget.h)

target_link_libraries(Parser Qt5::Core Qt5::Widgets)
