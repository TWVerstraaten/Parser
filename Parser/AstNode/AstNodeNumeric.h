//
// Created by pc on 25-11-20.
//

#ifndef PARSER_ASTNODENUMERIC_H
#define PARSER_ASTNODENUMERIC_H

#include "../Numeric.h"
#include "AstNode.h"

class AstNodeNumeric : public AstNode {

  public:
    [[nodiscard]] virtual Numeric toNumeric() const = 0;
};

#endif // PARSER_ASTNODENUMERIC_H
