//
// Created by pc on 28-05-21.
//

#include "ErrorBase.h"

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
