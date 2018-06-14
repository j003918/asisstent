#include "base64.h"

#include<string.h>
#include<stdio.h>


const char *endcode_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char decode_table[]=
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x00,0x00,0x00,0x3f,
    0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,
    0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x00,0x00,0x00,0x00,0x00,
    0x00,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
    0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x00,0x00,0x00,0x00,0x00
};
const char padding = '=';


DLL_EXPORT int __stdcall base64_encode(const char *in_src,int len,char *out_base64)
{
    unsigned char *src = (unsigned char *)in_src;
    unsigned char *out = (unsigned char *)out_base64;

    for(int i=0, tmp=(len/3)*3; i<tmp; i+=3,src+=3 )
    {
        *out++ = endcode_table[src[0]>>2];
        *out++ = endcode_table[(src[0]&0x03)<<4 | (src[1]>>4)];
        *out++ = endcode_table[(src[1]&0x0f)<<2 | (src[2]>>6)];
        *out++ = endcode_table[src[2]&0x3f];
    }

    if (1==len%3)
    {
        *out++ = endcode_table[src[0]>>2];
        *out++ = endcode_table[(src[0]&0x03)<<4];
        *out++ = padding;
        *out++ = padding;
    }
    else if (2==len%3)
    {
        *out++ = endcode_table[src[0]>>2];
        *out++ = endcode_table[(src[0]&0x03)<<4 | (src[1]>>4)];
        *out++ = endcode_table[(src[1]&0x0f)<<2];
        *out++ = padding;
    }
    *out = 0x00;

    return (char *)out-out_base64;
}

void print_decode_table(void)
{
    unsigned char decode_table[128];
    memset(decode_table,0,128);

    for(int i=0; i<strlen(endcode_table); i++)
    {
        int index = endcode_table[i];
        decode_table[index]=(unsigned char)i;
    }

    for(int i=0; i<128; i++)
    {
        printf("0x%02x,",decode_table[i]);
        if((i+1)%16==0)
            printf("\n");
    }
}

unsigned char get_base64_value(unsigned char ch)
{
    if ((ch >= 'A') && (ch <= 'Z'))
        return ch - 'A';
    if ((ch >= 'a') && (ch <= 'z'))
        return ch - 'a' + 26;
    if ((ch >= '0') && (ch <= '9'))
        return ch - '0' + 52;
    switch (ch)
    {
    case '+':
        return 62;
    case '/':
        return 63;
    case '=': /* base64 padding */
        return 0;
    default:
        return 0;
    }
}

DLL_EXPORT int __stdcall base64_decode(const char *in_base64,char *out_src)
{
    unsigned char *src = (unsigned char *)in_base64;
    unsigned char *out = (unsigned char *)out_src;
    unsigned char b1,b2,b3,b4;

    int len = strlen(in_base64);
    if ( len%4 )
    {
        return 0;
    }

    for(int i=0; i<len; i+=4)
    {
        b1=decode_table[*src++];//get_base64_value(*src++);
        b2=decode_table[*src++];//get_base64_value(*src++);
        b3=decode_table[*src++];//get_base64_value(*src++);
        b4=decode_table[*src++];//get_base64_value(*src++);

        *out++ = (b1<<2)|(b2>>4);
        *out++ = (b2<<4)|(b3>>2);
        *out++ = (b3<<6)|b4;
    }

    *out++ = 0x00;
    return (char *)out-out_src;
}
