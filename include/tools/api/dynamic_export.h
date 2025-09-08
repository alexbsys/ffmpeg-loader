
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
