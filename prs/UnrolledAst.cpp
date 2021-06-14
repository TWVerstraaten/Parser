//
// Created by pc on 14-06-21.
//

#include "UnrolledAst.h"

#include "Ast.h"

UnrolledAst::UnrolledAst(const Ast& ast) : m_header(ast.m_header), m_token(ast.body()) {
}

std::string UnrolledAst::toString() const {
    return "Header:\t" + m_header.toString() + "\nBody:\t" + m_token.toString() + "\n";
}
