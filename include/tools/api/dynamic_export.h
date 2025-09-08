//
// Copyright (c) 2025, Alex Bobryshev <alexbobryshev555@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef DYNAMIC_EXPORT_HEADER
#define DYNAMIC_EXPORT_HEADER

#ifdef __cplusplus
#define EXTERN_C        extern "C"
#define EXTERN_C_BEGIN  extern "C" {
#define EXTERN_C_END    }//extern "C"
#else
#define EXTERN_C
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif //__cplusplus

#ifdef _WIN32
#define API_EXPORT
#define API_STDCALL __stdcall
#define API_CDECL   __cdecl

#else /*_WIN32*/

#ifdef __APPLE__

#define API_STDCALL
#define API_CDECL
#define API_EXPORT __attribute__((visibility("default")))

#else //__APPLE__

#define API_STDCALL
#define API_CDECL

#ifdef __GNUC__
#define API_EXPORT __attribute__((visibility("default")))
#else /*__GNUC__*/
#define API_EXPORT 
#endif  /*__GNUC__*/

#endif /*__APPLE__*/

#endif /*_WIN32*/

#endif /*DYNAMIC_EXPORT_HEADER*/
