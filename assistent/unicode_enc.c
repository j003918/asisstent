#include<stdio.h>
#include "unicode_enc.h"
#include "gbk_data.h"


/*
   |  Unicode符号范围      |  UTF-8编码方式
 n |  (十六进制)           | (二进制)
---+-----------------------+------------------------------------------------------
 1 | 0000 0000 - 0000 007F |                                              0xxxxxxx
 2 | 0000 0080 - 0000 07FF |                                     110xxxxx 10xxxxxx
 3 | 0000 0800 - 0000 FFFF |                            1110xxxx 10xxxxxx 10xxxxxx
 4 | 0001 0000 - 0010 FFFF |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 5 | 0020 0000 - 03FF FFFF |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
*/

unsigned short get_gbkcode(const unsigned short unicode)
{
    int len = sizeof(uni2gbk)/sizeof(uni2gbk[0]);
    int l,r,m;
    l=0;
    r=len;

    while(l<=r)
    {
        m=(l+r)/2;
        if(unicode==uni2gbk[m][0])
            return uni2gbk[m][1];

        if(unicode>uni2gbk[m][0])
            l=m+1;
        else
            r=m-1;
    }

    return 0xffff;
}

unsigned short get_unicode(const unsigned short gbk_code)
{
    if (gbk_code==0x0080)
        return 0x20AC;

    if(gbk_code<0x8140)
        return 0xffff;

    if(gbk_code>0xFEA0)
        return 0xffff;

    return gbk2uni[gbk_code-0x8140];
}

DLL_EXPORT int __stdcall gbk_to_utf8(const char *in_gbk, char *out_utf8,int BOM)
{
    unsigned char *gbk = (unsigned char *)in_gbk;
    unsigned char *utf8 = (unsigned char *)out_utf8;
    unsigned short unic;
    unsigned char h,l;

    if (BOM) //add BOM EFBBBF
    {
        *utf8++=0xef;
        *utf8++=0xbb;
        *utf8++=0xbf;
    }

    while(*gbk)
    {
        if(*gbk<0x80)
        {
            *utf8++=*gbk++;
        }
        else
        {
            h = *gbk++;
            l = *gbk++;
            unic = get_unicode(h<<8|l);

            if ( unic >= 0x00000080 && unic <= 0x000007ff )
            {
                // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
                *utf8++=((unic >> 6) & 0x1f) | 0xc0;
                *utf8++=(unic & 0x3f) | 0x80;
            }
            else if ( unic >= 0x00000800 && unic <= 0x0000ffff )
            {
                // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
                *utf8++=((unic>>12) & 0x3f) | 0xe0;;
                *utf8++=((unic>>6) & 0x3f) | 0x80;
                *utf8++=(unic & 0x3f) | 0x80;
            }
            else
            {
                *out_utf8=0;
                return 0;
            }
        }
    }

    *utf8 = 0;
    return utf8-(unsigned char *)out_utf8;
}

int get_utf8_size(unsigned char c)
{
    int size = 0;

    while(c&0x80)
    {
        size++;
        c=c<<1;
    }

    return size;
}

DLL_EXPORT int __stdcall utf8_to_gbk(const char *in_utf8, char *out_gbk)
{
    unsigned char *utf8 = (unsigned char *)in_utf8;
    unsigned char *gbk = (unsigned char *)out_gbk;
    unsigned int gbk_code;
    unsigned char b1,b2,b3,h,l;

    if (utf8[0]==0xef && utf8[1]==0xbb && utf8[2]==0xbf) //skip BOM EFBBBF
    {
        utf8+=3;
    }

    while(*utf8)
    {
        switch (get_utf8_size(*utf8))
        {
        case 0:
            *gbk++ = *utf8++;
            break;
        case 2:
            b1 = *utf8++;
            b2 = *utf8++;

            h = (b1&0x1f) >> 2;
            l = (b1<<6) | (b2&0x3f);
            gbk_code = get_gbkcode( h<<8 | l );

            *gbk++ = gbk_code>>8&0xff;
            *gbk++ = gbk_code&0xff;
            break;
        case 3:
            b1 = *utf8++;
            b2 = *utf8++;
            b3 = *utf8++;

            l = (b3&0x3f) | (b2<<6);
            h = ((b2>>2) & 0x0f) | (b1<<4);
            gbk_code = get_gbkcode( h<< 8 | l );

            *gbk++=gbk_code>>8;
            *gbk++=gbk_code&0xff;
            break;
        default:
            *out_gbk=0;
            return 0;
        }
    }
    *gbk++ = 0;
    *gbk = 0;
    return gbk-(unsigned char *)out_gbk;
}

DLL_EXPORT int __stdcall uni_to_gbk(const char *in_uni, char *out_gbk)
{
    unsigned char *gbk = (unsigned char *)out_gbk;
    unsigned short *uni = (unsigned short *)in_uni;
    unsigned char *tmp;
    unsigned short gbkcode,unicode;
    unsigned char isLE,h,l;

    tmp = (unsigned char *)uni;
    if(tmp[0]==0xff && tmp[1]==0xfe)//LE
    {
        uni++;
        isLE = 1;
    }
    else if(tmp[0]==0xfe && tmp[1]==0xff)//BE
    {
        uni++;
        isLE = 0;
    }
    else
    {
        unsigned short t = 0x0001;
        isLE = *((unsigned char *)&t);
    }

    while(*uni)
    {
        tmp = (unsigned char *)uni++;
        h=isLE?*(tmp+1):*tmp;
        l=isLE?*tmp:*(tmp+1);
        unicode = h<<8|l;
        if(unicode < 0x80)
        {
            *gbk++ = unicode&0xff;
        }
        else
        {
            gbkcode=get_gbkcode(unicode);
            *gbk++ = gbkcode>>8;
            *gbk++ = gbkcode&0xff;
        }
    }
    return (char *)gbk-out_gbk;
}

DLL_EXPORT int __stdcall gbk_to_uni(const char *in_gbk, char *out_uni,int big_endian,int BOM)
{
    unsigned char *gbk = (unsigned char *)in_gbk;
    unsigned char *uni = (unsigned char *)out_uni;
    unsigned short unic;
    unsigned char h,l;

    if(BOM)
    {
        *uni++ = big_endian?0xfe:0xff;
        *uni++ = big_endian?0xff:0xfe;
    }

    while(*gbk)
    {
        if(*gbk<0x80)
        {
            *uni++= big_endian?0:*gbk++;
            *uni++= big_endian?*gbk++:0;
        }
        else
        {
            h = *gbk++;
            l = *gbk++;
            unic = get_unicode(h<<8|l);
            *uni++=big_endian?unic>>8:unic&0xff;
            *uni++=big_endian?unic&0xff:unic>>8;
        }
    }

    *uni++ = 0;
    *uni = 0;
    return (char *)uni-out_uni-1;
}

DLL_EXPORT int __stdcall uni_to_utf8(const char *in_uni, char *out_utf8,int BOM)
{
    unsigned char *utf8 = (unsigned char*)out_utf8;
    unsigned char *tmp;
    unsigned short *uni = (unsigned short*)in_uni;
    unsigned short unic;
    unsigned char isLE,h,l;

    if(BOM)
    {
        *utf8++=0xef;
        *utf8++=0xbb;
        *utf8++=0xbf;
    }

    tmp = (unsigned char *)uni;
    if(tmp[0]==0xff && tmp[1]==0xfe)//LE
    {
        uni++;
        isLE = 1;
    }
    else if(tmp[0]==0xfe && tmp[1]==0xff)//BE
    {
        uni++;
        isLE = 0;
    }
    else
    {
        unsigned short t = 0x0001;
        isLE = *((unsigned char *)&t);
    }

    while(*uni)
    {
        tmp=(unsigned char *)uni++;
        h=isLE?tmp[1]:tmp[0];
        l=isLE?tmp[0]:tmp[1];

        unic = h<<8|l;

        if(unic<0x80)
        {
            *utf8++=unic&0x7f;
        }
        else if ( unic >= 0x00000080 && unic <= 0x000007ff )
        {
            // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            *utf8++=((unic >> 6) & 0x1f) | 0xc0;
            *utf8++=(unic & 0x3f) | 0x80;
        }
        else if ( unic >= 0x00000800 && unic <= 0x0000ffff )
        {
            // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            *utf8++=((unic>>12) & 0x3f) | 0xe0;;
            *utf8++=((unic>>6) & 0x3f) | 0x80;
            *utf8++=(unic & 0x3f) | 0x80;
        }
        else
        {
            *out_utf8=0;
            return 0;
        }
    }
    return (char *)utf8-out_utf8;
}

DLL_EXPORT int __stdcall utf8_to_uni(const char *in_utf8, char *out_uni,int big_endian,int BOM)
{
    unsigned char *utf8 = (unsigned char *)in_utf8;
    unsigned char *uni = (unsigned char *)out_uni;
    unsigned char b1,b2,b3;

    if (utf8[0]==0xef && utf8[1]==0xbb && utf8[2]==0xbf) //skip BOM
    {
        utf8+=3;
    }

    if(BOM) //add BOM
    {
        *uni++ = big_endian?0xfe:0xff;
        *uni++ = big_endian?0xff:0xfe;
    }

    while(*utf8)
    {
        switch(get_utf8_size(*utf8))
        {
        case 0:
            *uni++=big_endian?0:*utf8++;
            *uni++=big_endian?*utf8++:0;
             break;
        case 2:
            b1 = *utf8++;
            b2 = *utf8++;
            //110xxxxx 10xxxxxx
            *uni++=big_endian ? (b1&0x1f)>>2 : (b2&0x3f)|(b1<<6);
            *uni++=big_endian ? (b2&0x3f)|(b1<<6) : (b1&0x1f)>>2;

            break;
        case 3:
            b1 = *utf8++;
            b2 = *utf8++;
            b3 = *utf8++;
            //1110xxxx 10xxxxxx 10xxxxxx
            *uni++=big_endian? (b1&0x0f)<<4 | (((b2<<2)>>4)) : (b3&0x3f) | (b2<<6);
            *uni++=big_endian? (b3&0x3f) | (b2<<6) : (b1&0x0f)<<4 | (((b2<<2)>>4));
            break;
        default:
            *out_uni++=0;
            *out_uni++=0;
            return 0;
        }
    }

    return (char *)uni-out_uni;
}
