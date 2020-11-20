//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEDOUBLE_H
#define PARSER_ASTNODEDOUBLE_H

#include "AstNode.h"

class AstNodeDouble : public AstNode {

  public:
    explicit AstNodeDouble(const std::string& string);

    [[nodiscard]] std::string toString() const override;

  private:
    double m_val;
};

#endif // PARSER_ASTNODEDOUBLE_H
