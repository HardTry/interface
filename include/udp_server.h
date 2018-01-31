#ifndef FCRECEIVER_H
#define FCRECEIVER_H

#include "utility_global.h"

#include <uv.h>
#include <functional>

#define BUFFLEN 8192

template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
   template <typename... Args>
   static Ret callback(Args... args) {
      return func(args...);
   }
   static std::function<Ret(Params...)> func;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;


typedef int (*process_package_callback)(const char*, int len, const char* sender_ip);

class UTILITY_API UdpSrvReceiver
{
public:
    UdpSrvReceiver();
    virtual ~UdpSrvReceiver();

    int start();
    void stop();

    void set_process_callback(process_package_callback cb) {process_package_cb_ = cb;}

protected:
    void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    void on_read(uv_udp_t* req,
                 ssize_t nread,
                 const uv_buf_t* buf,
                 const struct sockaddr* addr,
                 unsigned flags);

private:
    uv_loop_t * loop_ = nullptr;
    uv_buf_t buf_;
    uv_udp_t recv_socket_;
    uv_alloc_cb alloc_cb_ = nullptr;
    uv_udp_recv_cb read_cb_ = nullptr;

private:
    process_package_callback process_package_cb_ = nullptr;
};

#endif // FCRECEIVER_H
