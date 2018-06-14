#include <stdio.h>
#include <stdlib.h>
#include<windows.h>

//#include "base64.h"


void prt_hex(unsigned char *p,int len)
{
    for(int i=0; i<len; i++)
    {
        printf("0x%02X,",*(p+i));
    }
    printf("\n");
}

void prt_bin(unsigned char *ch,int len)
{
    unsigned char c;

    ch += len-1;
    for(int i=0; i<len; i++)
    {
        c = *ch--;
        printf("%d%d%d%d%d%d%d%d  ",c>>7&0x01,c>>6&0x01,c>>5&0x01,c>>4&0x01,c>>3&0x01,c>>2&0x01,c>>1&0x01,c&0x01);
    }

    printf("\n");
}


int main()
{
    HMODULE hDllLib = LoadLibrary("assistent.dll");

    FARPROC base64_decode = GetProcAddress(hDllLib,"base64_decode");
    FARPROC utf8_to_gbk = GetProcAddress(hDllLib,"utf8_to_gbk");

    unsigned char pout[512]= {0};
    unsigned char pout2[512]= {0};
    int ret = 0;

    char *p64="5Zu96ZmF5Lya6K6u5Lit5b+D5rOw5bGx5Y6F77yMDQoNCuOAgOOAgOS5oOi/keW5s+S4u+W4reS4u+aMgeWkp+iMg+WbtOS8muiwiOOAgg0KDQrjgIDjgIDmiJDlkZjlm73pooblr7zkurrjgIHluLjorr7mnLrmnoTotJ/otKPkurrjgIENCg0K44CA44CA6KeC5a+f5ZGY5Zu96aKG5a+85Lq65Y+K6IGU5ZCI5Zu9562J5Zu96ZmF57uE57uH6LSf6LSj5Lq677yMDQoNCuOAgOOAgOWbtOWdkOS4gOi1t++8jA0KDQrjgIDjgIDlhbHlkIzmj4/nu5jlkIjkvZzlj5HlsZXnmoTmlrDok53lm77jgIINCg0K44CA44CA5Lmg6L+R5bmz5Li75bit55m76auY5pyb6L+c77yMDQoNCuOAgOOAgOaKiuaPoeaXtuS7o+a9rua1ge+8jA0KDQrjgIDjgIDmj5Dlh7rkuK3lm73mlrnmoYjjgII=";
    ret = base64_decode(p64,pout);

    prt_bin(pout,10);


    getchar();
    return 0;
}
