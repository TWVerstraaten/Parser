//
// Created by pc on 07-06-21.
//

#include "Parser.h"

#include "AstToken.h"
#include "StructuralTokenizer.h"
#include "Tokenizer.h"

#include <QDebug>
#include <iostream>
#include <memory>

Parser::Parser(std::string string) : m_string(std::move(string)) {
    Tokenizer           tok(m_string, m_info);
    StructuralTokenizer structuralTokenizer(tok.tokenList(), m_info);
    m_astToken = std::make_unique<AstToken>(structuralTokenizer.tokenList());
    std::cout << "Parsed\t" << originalString() << '\n';
    if (success()) {
        m_astToken->printTree();
    }
    m_info.printAll();
}

bool Parser::success() const {
    return m_astToken && m_info.success();
}

const std::string& Parser::originalString() const {
    return m_string;
}

const ParserInfo& Parser::info() const {
    return m_info;
}
