#ifndef UNICODE_ENC_H_INCLUDED
#define UNICODE_ENC_H_INCLUDED

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif


DLL_EXPORT int __stdcall utf8_to_gbk(const char *in_utf8, char *out_gbk);
DLL_EXPORT int __stdcall gbk_to_utf8(const char *in_gbk, char *out_utf8,int BOM);


DLL_EXPORT int __stdcall uni_to_gbk(const char *in_uni, char *out_gbk);
DLL_EXPORT int __stdcall gbk_to_uni(const char *in_gbk, char *out_uni,int big_endian,int BOM);

DLL_EXPORT int __stdcall uni_to_utf8(const char *in_uni, char *out_utf8,int BOM);
DLL_EXPORT int __stdcall utf8_to_uni(const char *in_utf8, char *out_uni,int big_endian,int BOM);


#ifdef __cplusplus
}
#endif

#endif // UNICODE_ENC_H_INCLUDED
