//
// Created by pc on 07-06-21.
//

#include "Parser.h"

#include "AstToken.h"
#include "StructuralTokenizer.h"
#include "Tokenizer.h"

#include <QDebug>
#include <iostream>

Parser::Parser(std::string string) : m_string(std::move(string)) {
    Tokenizer           tok(m_string, m_info);
    StructuralTokenizer structuralTokenizer(tok.tokenList(), m_info);
    AstToken            astToken(structuralTokenizer.tokenList());

    if (not success()) {
        std::cout << originalString() << '\n';
        for (const auto& er : m_info.errors()) {
            qDebug() << QString::fromStdString(er.toString());
        }
    } else {
        std::cout << originalString() << '\n';
        astToken.printTree();
        if (hasWarnings()) {
            for (const auto& er : m_info.warnings()) {
                qWarning() << QString::fromStdString(er.toString());
            }
        }
    }
}

bool Parser::success() const {
    return m_info.success();
}

bool Parser::hasWarnings() const {
    return m_info.hasWarning();
}

const std::vector<ParserError>& Parser::errors() const {
    return m_info.errors();
}

const std::vector<ParserWarning>& Parser::warnings() const {
    return m_info.warnings();
}
const std::string& Parser::originalString() const {
    return m_string;
}
