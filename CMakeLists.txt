cmake_minimum_required(VERSION 3.17)
project(Parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 17)

add_executable(Parser main.cpp Parser/Parser.cpp Parser/Parser.h Parser/Ast.cpp Parser/Ast.h Parser/AstNode/AstNode.cpp Parser/AstNode/AstNode.h Parser/AstNode/AstNodeVar.cpp Parser/AstNode/AstNodeVar.h Parser/AstNode/AstNodeError.cpp Parser/AstNode/AstNodeError.h Parser/AstNode/AstNodeAdd.cpp Parser/AstNode/AstNodeAdd.h Parser/AstNode/AstNodeDouble.cpp Parser/AstNode/AstNodeDouble.h Parser/AstNode/AstNodePower.cpp Parser/AstNode/AstNodePower.h Parser/Tokenizer.cpp Parser/Tokenizer.h Parser/AstNode/AstNodeInteger.cpp Parser/AstNode/AstNodeInteger.h Parser/AstNode/AstNodeSubtract.cpp Parser/AstNode/AstNodeSubtract.h Parser/AstNode/AstNodeMul.cpp Parser/AstNode/AstNodeMul.h Parser/AstNode/AstNodeDiv.cpp Parser/AstNode/AstNodeDiv.h Parser/AstNode/AstNodeUnaryMinus.cpp Parser/AstNode/AstNodeUnaryMinus.h Parser/AstNode/AstNodeFunction.cpp Parser/AstNode/AstNodeFunction.h Parser/AstNode/Pattern/Pattern.cpp Parser/AstNode/Pattern/Pattern.h)