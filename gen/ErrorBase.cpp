//
// Created by pc on 28-05-21.
//

#include "ErrorBase.h"

namespace gen {
    bool ErrorBase::success() const {
        return m_success;
    }

    const std::string& ErrorBase::errorString() const {
        return m_errorString;
    }

    bool ErrorBase::setErrorIf(bool b, const std::string& errorString) {
        if (b) {
            m_success     = false;
            m_errorString = errorString;
        }
        return b;
    }

    void ErrorBase::setError(const std::string& string) {
        m_success     = false;
        m_errorString = string;
    }

    void ErrorBase::setSuccess(bool b) {
        m_success = b;
        if (m_success) {
            m_errorString = "";
        }
    }
} // namespace gen