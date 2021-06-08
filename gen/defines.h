//
// Created by pc on 07-06-21.
//

#ifndef PARSER_DEFINES_H
#define PARSER_DEFINES_H

#define TT_IT(x) x.begin(), x.end()
#define TT_LAMBDA(a, x) [](const auto& a) { x }
#define TT_LAMBDA_REF(a, x) [&](const auto& a) { x }
#define TT_LAMBDA_THIS(a, x) [&](const auto& a) { x }

#define TT_TO_STRING_DETAIL(x) #x
#define TT_TO_STRING(x) TT_TO_STRING_DETAIL(x)
#define TT_WHERE_STRING (__FILE__ ":" TT_TO_STRING(__LINE__))

#endif // PARSER_DEFINES_H
