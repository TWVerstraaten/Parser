//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEERROR_H
#define PARSER_ASTNODEERROR_H

#include "AstNode.h"

class AstNodeError : public AstNode {
  public:
    AstNodeError();

    [[nodiscard]] std::string toString() const override;
};

#endif // PARSER_ASTNODEERROR_H
