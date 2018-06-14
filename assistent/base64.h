#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT int __stdcall base64_encode(const char *in_src,int len,char *out_base64);
DLL_EXPORT int __stdcall base64_decode(const char *in_base64,char *out_src);

#ifdef __cplusplus
}
#endif

#endif // BASE64_H_INCLUDED
