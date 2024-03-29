//
// Created by pc on 28-05-21.
//

#ifndef PARSER_ERRORBASE_H
#define PARSER_ERRORBASE_H

#include <string>

namespace gen {
    class ErrorBase {
      public:
        void setSuccess(bool b);
        [[nodiscard]] bool               success() const;
        [[nodiscard]] const std::string& errorString() const;

      public:
        void setError(const std::string& string);

      protected:
        bool setErrorIf(bool b, const std::string& errorString);

        bool        m_success     = true;
        std::string m_errorString = "";
    };
} // namespace gen

#endif // PARSER_ERRORBASE_H
