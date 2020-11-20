//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEINTEGER_H
#define PARSER_ASTNODEINTEGER_H

#include "AstNode.h"

class AstNodeInteger : public AstNode {
  public:
    explicit AstNodeInteger(const std::string& string);

    [[nodiscard]] std::string toString() const override;

  private:
    size_t m_val;
};

#endif // PARSER_ASTNODEINTEGER_H
