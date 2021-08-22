//
// Created by pc on 22-08-21.
//

#ifndef TEST_TEST_H
#define TEST_TEST_H

namespace test {

    class Test {

      public:
        static void TEST_ALL();
        static void TEST_DIMENSIONS();
        static void TEST_VARIABLES();
        static void TEST_CAN_UNROLL();
        static void TEST_DEPENDENCY_MANAGER();
        static void TEST_UNROLL_MANAGER();
        static void TEST_HEADERS();
    };

} // namespace test

#endif // TEST_TEST_H
