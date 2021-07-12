cmake_minimum_required(VERSION 3.17)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(Parser main.cpp app/Resources/shaders.qrc app/UndoRedoConsumer.h app/UndoRedoHandler.h app/UndoRedoConsumer.cpp app/UndoRedoHandler.cpp app/MainWindow.cpp app/MainWindow.h app/FormulaWidget.cpp app/FormulaWidget.h app/FormulaSideBar.cpp app/FormulaSideBar.h app/cmd/NewFormulaWidgetCommand.cpp app/cmd/NewFormulaWidgetCommand.h app/cmd/RemoveFormulaWidgetCommand.cpp app/cmd/RemoveFormulaWidgetCommand.h app/cmd/SkipFirstRedoWrapper.h app/cmd/TextEditChangedCommand.cpp app/cmd/TextEditChangedCommand.h alg/StringAlg.cpp alg/StringAlg.h app/Surface.cpp app/Surface.h app/OpenGlWidget.cpp app/OpenGlWidget.h app/SurfaceManager.cpp app/SurfaceManager.h app/Axes.cpp app/Axes.h app/CameraManager.cpp app/CameraManager.h app/CentralWidget.cpp app/CentralWidget.h app/FormulaSurfaceInterface.cpp app/FormulaSurfaceInterface.h gen/DependencyGraph.cpp gen/DependencyGraph.h ast/par/Tokenizer.cpp ast/par/Tokenizer.h ast/err/ParserError.cpp ast/err/ParserError.h ast/par/Token.cpp ast/par/Token.h ast/par/StructuralToken.cpp ast/par/StructuralToken.h gen/defines.h ast/err/ParserWarning.cpp ast/err/ParserWarning.h ast/par/StructuralTokenizer.cpp ast/par/StructuralTokenizer.h ast/par/Parser.cpp ast/par/Parser.h ast/err/ParserInfo.cpp ast/err/ParserInfo.h ast/par/AstToken.cpp ast/par/AstToken.h ast/par/Range.cpp ast/par/Range.h ast/err/ParserMessage.cpp ast/err/ParserMessage.h ast/Ast.cpp ast/Ast.h ast/par/ReservedToken.cpp ast/par/ReservedToken.h ast/par/CustomFunctionToken.cpp ast/par/CustomFunctionToken.h gen/Overloaded.h ast/par/TokenWriter.cpp ast/par/TokenWriter.h ast/par/TokenTemplates.h ast/Header.cpp ast/Header.h ast/par/UnrolledAstToken.cpp ast/par/UnrolledAstToken.h ast/UnrolledAst.cpp ast/UnrolledAst.h ast/par/VectorToken.h ast/AstManager.cpp ast/AstManager.h ast/UnrollCompanion.cpp ast/UnrollCompanion.h ast/err/DefinitionError.cpp ast/err/DefinitionError.h gen/Number.cpp app/TextEdit.cpp app/TextEdit.h app/OldCursor.cpp app/OldCursor.h)

target_link_libraries(Parser Qt5::Core Qt5::Widgets)
