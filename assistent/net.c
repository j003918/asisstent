#include "net.h"

#include<windows.h>
#include <winsock2.h>
#include <stdio.h>

int fd_client = 0;

DLL_EXPORT int __stdcall get_error(void)
{
    return WSAGetLastError();
}

DLL_EXPORT int __stdcall init_net(void)
{
    WSADATA wsadata;
    if (WSAStartup(0x0202,&wsadata))
        return -1;

    return 0;
}

DLL_EXPORT int __stdcall tcp_conn(const char *srv_addr,int srv_port)
{
    struct sockaddr_in addrServ;
    int ret = 0;
    fd_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == fd_client)
        return -1;

    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(srv_port);
    addrServ.sin_addr.S_un.S_addr = inet_addr(srv_addr);

    ret = connect(fd_client, (LPSOCKADDR)&addrServ, sizeof(addrServ));
    if (SOCKET_ERROR == ret)
        return -1;

    return 0;
}

DLL_EXPORT int __stdcall tcp_send(const char *buf,int len)
{
    return send(fd_client,buf,len,0);
}

DLL_EXPORT int __stdcall tcp_recv(char *buf,int len)
{
    return recv(fd_client,buf,len,0);
}

DLL_EXPORT int __stdcall tcp_close(void)
{
    return closesocket(fd_client);
}


#define WL_FD_STATUS_READABLE  0x01
#define WL_FD_STATUS_WRITEABLE 0x02
#define WL_FD_STATUS_EXCEPTION 0x04
DLL_EXPORT int __stdcall fd_state( int sec)
{
    struct timeval tm;
    struct fd_set rfds;
    //struct fd_set wfds;
    //struct fd_set efds;
    int rst;
    int retval = 0;

    FD_ZERO(&rfds);
    //FD_ZERO(&wfds);
    //FD_ZERO(&efds);


    FD_SET(fd_client,&rfds);
    //FD_SET(fd_client,&wfds);
    //FD_SET(fd_client,&efds);


    tm.tv_sec = sec;
    tm.tv_usec = 0;

    rst = select((int)fd_client+1, &rfds, NULL, NULL, &tm);
    if ( rst > 0)
    {
        if ( FD_ISSET(fd_client,&rfds) )
        {
            retval |= WL_FD_STATUS_READABLE;
        }
        /*
                if ( FD_ISSET(fd_client,&wfds) )
                {
                    retval |= WL_FD_STATUS_WRITEABLE;
                }

                if ( FD_ISSET(fd_client,&efds) )
                {
                    retval |= WL_FD_STATUS_EXCEPTION;
                }
        */
        return retval;
    }

    return rst;
}
