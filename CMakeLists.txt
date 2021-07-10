cmake_minimum_required(VERSION 3.17)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(Parser main.cpp app/Resources/shaders.qrc  app/MainWindow.cpp app/MainWindow.h app/FormulaWidget.cpp app/FormulaWidget.h  gen/ErrorBase.cpp gen/ErrorBase.h app/FormulaSideBar.cpp app/FormulaSideBar.h app/cmd/NewFormulaWidgetCommand.cpp app/cmd/NewFormulaWidgetCommand.h app/cmd/RemoveFormulaWidgetCommand.cpp app/cmd/RemoveFormulaWidgetCommand.h app/UndoRedoConsumer.cpp app/UndoRedoConsumer.h app/UndoRedoHandler.cpp app/UndoRedoHandler.h app/cmd/SkipFirstRedoCommand.h app/cmd/FormulaChangedCommand.cpp app/cmd/FormulaChangedCommand.h alg/StringAlg.cpp alg/StringAlg.h app/Surface.cpp app/Surface.h app/OpenGlWidget.cpp app/OpenGlWidget.h app/SurfaceManager.cpp app/SurfaceManager.h app/Axes.cpp app/Axes.h app/CameraManager.cpp app/CameraManager.h app/CentralWidget.cpp app/CentralWidget.h app/FormulaSurfaceInterface.cpp app/FormulaSurfaceInterface.h gen/DependencyGraph.cpp gen/DependencyGraph.h ast/Tokenizer.cpp ast/Tokenizer.h ast/err/ParserError.cpp ast/err/ParserError.h ast/Token.cpp ast/Token.h ast/StructuralToken.cpp ast/StructuralToken.h gen/defines.h ast/err/ParserWarning.cpp ast/err/ParserWarning.h ast/StructuralTokenizer.cpp ast/StructuralTokenizer.h ast/Parser.cpp ast/Parser.h ast/err/ParserInfo.cpp ast/err/ParserInfo.h ast/AstToken.cpp ast/AstToken.h ast/Range.cpp ast/Range.h ast/err/ParserMessage.cpp ast/err/ParserMessage.h ast/Ast.cpp ast/Ast.h ast/ReservedFunction.cpp ast/ReservedFunction.h ast/CustomFunctionToken.cpp ast/CustomFunctionToken.h gen/Overloaded.h ast/TokenWriter.cpp ast/TokenWriter.h ast/TokenTemplates.h ast/Header.cpp ast/Header.h ast/UnrolledAstToken.cpp ast/UnrolledAstToken.h ast/UnrolledAst.cpp ast/UnrolledAst.h ast/VectorToken.h ast/AstManager.cpp ast/AstManager.h ast/UnrollCompanion.cpp ast/UnrollCompanion.h ast/err/DefinitionError.cpp ast/err/DefinitionError.h gen/Number.cpp)

target_link_libraries(Parser Qt5::Core Qt5::Widgets)
