//
// Created by pc on 22-08-21.
//

#include "Test.h"

#include "../ast/Ast.h"
#include "../ast/Dimension.h"
#include "../ast/UnrollManager.h"

#include <cmath>
#include <iostream>

#define IS_TRUE(x)                                                                                                                                                                 \
    {                                                                                                                                                                              \
        if (!(x)) {                                                                                                                                                                \
            std::cout << __FUNCTION__ << " failed on line " << __FILE__ << ":" << __LINE__ << std::endl;                                                                           \
        }                                                                                                                                                                          \
    }

namespace test {
    void Test::TEST_ALL() {
        TEST_DIMENSIONS();
        TEST_VARIABLES();
        TEST_CAN_UNROLL();
        TEST_DEPENDENCY_MANAGER();
        TEST_UNROLL_MANAGER();
        TEST_HEADERS();
    }

    void Test::TEST_DIMENSIONS() {
        IS_TRUE(ast::Ast{"f = (1,2,3,4) * 3 * 2 + (1,2,3,4)"}.dimension() == 4);
        IS_TRUE(ast::Ast{"f = (1,2,3,4) / 3 * 2 + (1,2,3,4)"}.dimension() == 4);
        IS_TRUE(ast::Ast{"f(x) = sin(x)"}.dimension() == 1);
        IS_TRUE(ast::Ast{"f(x) = sin(x) * (1,2)"}.dimension() == 2);
        IS_TRUE(ast::Ast{"f(x) = 1 * (1,2) * 2 * (2,3,4)"}.dimension() == DIMENSION_MISMATCH);
    }

    void Test::TEST_VARIABLES() {
        IS_TRUE((ast::Ast{"f(x,y,t) = x+y+t"}.declaredVariables() == std::vector<std::string>{"x", "y", "t"}));
        IS_TRUE((ast::Ast{"f(x,y,t) = x+y+t"}.variablesUsed() == std::set<std::string>{"x", "y", "t"}));
        IS_TRUE((ast::Ast{"x+y+t"}.variablesUsed() == std::set<std::string>{"x", "y", "t"}));
    }

    void Test::TEST_CAN_UNROLL() {
        IS_TRUE(ast::Ast{"f(x,y,t) = x+y+t"}.canUnroll());
        IS_TRUE(ast::Ast{"x+y"}.canUnroll());
        IS_TRUE(ast::Ast{"z=x+y"}.canUnroll());
        IS_TRUE(not ast::Ast{"g = x+y"}.canUnroll());
        IS_TRUE(not ast::Ast{"x+y+t"}.canUnroll());
        IS_TRUE(not ast::Ast{"f(t) = x+y+t"}.canUnroll());
        IS_TRUE(ast::Ast{"f(t, y, x) = x+y+t"}.canUnroll());
    }

    void Test::TEST_DEPENDENCY_MANAGER() {
        ast::DependencyManager manager;
        manager.addVertex(0, ast::Ast{"f(x)=x"});
        IS_TRUE((manager.unrollable() == std::set<size_t>{0}));
        IS_TRUE((manager.unrollOrder() == std::vector<size_t>{0}));
        IS_TRUE((manager.updated() == std::set<size_t>{0}));
        manager.addVertex(1, ast::Ast{"h(x)=f(x)"});
        IS_TRUE((manager.unrollable() == std::set<size_t>{0, 1}));
        IS_TRUE((manager.unrollOrder() == std::vector<size_t>{0, 1}));
        IS_TRUE((manager.updated() == std::set<size_t>{0, 1}));
        manager.clearUpdated();
        IS_TRUE((manager.updated().empty()));
        manager.addVertex(0, ast::Ast{"f(x)= x + 1"});
        IS_TRUE((manager.unrollable() == std::set<size_t>{0, 1}));
        IS_TRUE((manager.unrollOrder() == std::vector<size_t>{0, 1}));
        IS_TRUE((manager.updated() == std::set<size_t>{0, 1}));
        manager.clearUpdated();
        manager.addVertex(1, ast::Ast{"h(x)= f(x) + 1"});
        IS_TRUE((manager.updated() == std::set<size_t>{1}));
        manager.removeVertex(0);
        IS_TRUE((manager.unrollable().empty()));
        manager.removeVertex(1);
        manager.addVertex(0, ast::Ast{"a1(x) = x"});
        manager.addVertex(1, ast::Ast{"a2(x) = a1(x)"});
        manager.addVertex(2, ast::Ast{"a3(x) = a2(x)"});
        IS_TRUE((manager.unrollOrder() == std::vector<size_t>{0, 1, 2}));
        manager.clearUpdated();
        manager.removeVertex(0);
        IS_TRUE((manager.unrollOrder().empty()));
        IS_TRUE((manager.updated() == std::set<size_t>{1, 2}));
        manager.addVertex(0, ast::Ast{"a1(x) = a3(x)"});
        IS_TRUE((manager.unrollable().empty()));
        manager.addVertex(0, ast::Ast{"a1(x) = x"});
        IS_TRUE((manager.unrollOrder() == std::vector<size_t>{0, 1, 2}));
    }

    void Test::TEST_UNROLL_MANAGER() {
        ast::UnrollManager unrollManager;
        unrollManager.add(0, ast::Ast{"f(x)=x+3"});
        unrollManager.unrollAll();
        IS_TRUE(unrollManager.getUnrolledAt(0).setVariable("x", gen::Number(3ll)).toNumber() == gen::Number(6ll));
        IS_TRUE(unrollManager.getUnrolledAt(0).setVariable("x", gen::Number(3.2)).toNumber() == gen::Number(6.2));
        unrollManager.add(0, ast::Ast{"atan2(a,b)"});
        unrollManager.add(1, ast::Ast{"a = 0.1"});
        unrollManager.add(2, ast::Ast{"b = 2^(0.5)"});
        unrollManager.unrollAll();
        IS_TRUE(unrollManager.getUnrolledAt(0).token().toNumber() == gen::Number(std::atan2(0.1, std::pow(2ll, 0.5))));
    }

    void Test::TEST_HEADERS() {
        IS_TRUE(ast::Ast{"f(x)=x"}.headerType() == ast::Header::HEADER_TYPE::FULL_HEADER);
        IS_TRUE(ast::Ast{"f=x"}.headerType() == ast::Header::HEADER_TYPE::CONSTANT);
        IS_TRUE(ast::Ast{"(x)=x"}.headerType() == ast::Header::HEADER_TYPE::COORDINATE_VECTOR);
        IS_TRUE(ast::Ast{"(x,y)=x"}.headerType() == ast::Header::HEADER_TYPE::COORDINATE_VECTOR);
        IS_TRUE(ast::Ast{"z=x"}.headerType() == ast::Header::HEADER_TYPE::SINGLE_COORDINATE);
        IS_TRUE(ast::Ast{"x"}.headerType() == ast::Header::HEADER_TYPE::EMPTY);
    }

} // namespace test

#undef IS_TRUE
