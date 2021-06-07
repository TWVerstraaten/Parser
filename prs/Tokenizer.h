//
// Created by pc on 06-06-21.
//

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

#include "ParserError.h"
#include "ParserInfo.h"
#include "ParserWarning.h"
#include "StructuralToken.h"
#include "Token.h"

#include <list>
#include <string>
#include <vector>

class Tokenizer {

  public:
    Tokenizer(std::string string, ParserInfo& info);

    [[nodiscard]] std::string             toString(bool readable = true) const;
    [[nodiscard]] const std::list<Token>& tokenList() const;

  private:
    void tokenize();
    void replaceUnaryMinuses();
    void findIllegalCharacters();
    void checkBrackets();
    void checkDoubleEquals();
    void checkRepeatedCommas();
    void checkRepeatedOperators();
    void checkIdentifierNumberPatternWithNoSpace();

    const std::string m_string;
    std::list<Token>  m_tokenList;
    ParserInfo&       m_info;
};

#endif // PARSER_TOKENIZER_H
