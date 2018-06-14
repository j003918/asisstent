#ifndef GBK_H_INCLUDED
#define GBK_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

unsigned short get_gbkcode(const unsigned short unicode);
unsigned short get_unicode(const unsigned short gbk_code);

#ifdef __cplusplus
}
#endif

#endif // GBK_H_INCLUDED
