#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT int __stdcall get_error(void);
DLL_EXPORT int __stdcall fd_state(int sec);
DLL_EXPORT int __stdcall init_net(void);
DLL_EXPORT int __stdcall tcp_conn(const char *srv_addr,int srv_port);
DLL_EXPORT int __stdcall tcp_send(const char *buf,int len);
DLL_EXPORT int __stdcall tcp_recv(char *buf,int len);
DLL_EXPORT int __stdcall tcp_close(void);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
