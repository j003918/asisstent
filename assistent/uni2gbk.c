#include "uni2gbk.h"

#include "gbk_data.h"


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

/*
unsigned short get_gbkcode(unsigned short unicode)
{
    int len = sizeof(uni2oem936)/sizeof(uni2oem936[0]);

    for(int i=0; i<len; i+=2)
    {
        if(uni2oem936[i]==unicode)
            return uni2oem936[i+1];
    }

    return 0xFFFF;
}
*/
/*
unsigned short get_unicode(unsigned short gbk_code)
{
    int len = sizeof(uni2oem936)/sizeof(uni2oem936[0]);

    for(int i=1; i<len; i+=2)
    {
        if(uni2oem936[i]==gbk_code)
            return uni2oem936[i-1];
    }

    return 0xFFFF;
}
*/
