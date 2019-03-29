/*//////////////////////////////////////////////////////////////////////////////

MIT License

Copyright (c) 2019 OpenMaplestory

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
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

Created: 29/03/2019 23:00

//////////////////////////////////////////////////////////////////////////////*/

#pragma once

#include "OpenMS/Core.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace OpenMS
{

	class OMS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define OMS_CORE_ERROR(...)		::OpenMS::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OMS_CORE_WARN(...)		::OpenMS::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OMS_CORE_INFO(...)		::OpenMS::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OMS_CORE_TRACE(...)		::OpenMS::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OMS_CORE_FATAL(...)		::OpenMS::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define OMS_ERROR(...)	::OpenMS::Log::GetClientLogger()->error(__VA_ARGS__)
#define OMS_WARN(...)	::OpenMS::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OMS_INFO(...)	::OpenMS::Log::GetClientLogger()->info(__VA_ARGS__)
#define OMS_TRACE(...)	::OpenMS::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OMS_FATAL(...)	::OpenMS::Log::GetClientLogger()->fatal(__VA_ARGS__)