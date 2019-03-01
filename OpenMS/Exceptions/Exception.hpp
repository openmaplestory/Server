/*//////////////////////////////////////////////////////////////////////////////

MIT License

Copyright (c) 2019 OpenMaplestory

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Created: 22/02/2019 20:59

//////////////////////////////////////////////////////////////////////////////*/
#pragma once

#include "Common/Common.hpp"
#include <exception>
#include <string>
#include <sstream>
#include <iomanip>
#include "ExceptionStatus.hpp"

using namespace OpenMS;

namespace OpenMS
{
namespace Exceptions
{

class Exception: public std::exception
{

public:

    Exception(Exceptions::ExceptionStatus status, const std::string& message = std::string()):
		std::exception(),
		m_message(message),
        m_status(status)
    {
    }

	Exceptions::ExceptionStatus getStatus() const
    {
        return m_status;
    }

	virtual const char* what() const noexcept override
	{
		return m_message.c_str();
	}

private:
	std::string m_message;
	Exceptions::ExceptionStatus m_status;

    static std::string createExceptionMessage(Exceptions::ExceptionStatus status)
    {
        std::stringstream exceptionString;

        exceptionString << "Exception (0x";
        exceptionString << std::setw(4) << std::setfill('0') << std::hex << (Exceptions::ExceptionStatusType)status;
        exceptionString << "): " << Exceptions::exceptionStatusName(status);

        return exceptionString.str();
    }
};

}
}
