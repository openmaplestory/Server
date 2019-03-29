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

Created: 29/03/2019 22:54

//////////////////////////////////////////////////////////////////////////////*/

#pragma once

#pragma region EXPORT_SEMANTICS

#if defined(_MSC_VER)
	// Microsoft
	#ifdef OMS_BUILD_DLL
	#define OMS_API __declspec(dllexport)
	#else
	#define OMS_API __declspec(dllimport)
	#endif // OMS_BUILD_DLL

#elif defined(__GNUC__)
	// GCC
#ifdef OMS_BUILD_DLL
	#define OMS_API __attribute__((visibility("default")))
#else
	#define OMS_API 
#endif // OMS_BUILD_DLL
#else
	#pragma warning Unknown dynamic link semantics
#endif // Exporting semantics

#pragma endregion

#ifdef OMS_DEBUG
#define OMS_ENABLE_ASSERTS
#endif

#ifdef OMS_ENABLE_ASSERTS
#define OMS_ASSERT(x, ...) { if(!(x)) { OMS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define OMS_CORE_ASSERT(x, ...) { if(!(x)) { OMS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define OMS_ASSERT(x, ...)
#define OMS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)