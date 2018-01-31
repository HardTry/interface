#include "udp_server.h"

#include <memory.h>



#define alloc_call_back_func void(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
#define on_read_call_back void(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)

UdpSrvReceiver::UdpSrvReceiver() {
  buf_.base = new char[BUFFLEN]; buf_.len = BUFFLEN;
  Callback<alloc_call_back_func>::func
          = std::bind(&UdpSrvReceiver::alloc_buffer,
                      this,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3);
  alloc_cb_ = static_cast<uv_alloc_cb>(Callback<alloc_call_back_func>::callback);

  Callback<on_read_call_back>::func =
          std::bind(&UdpSrvReceiver::on_read,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4,
                    std::placeholders::_5);
  read_cb_ = static_cast<uv_udp_recv_cb>(Callback<on_read_call_back>::callback);
}

UdpSrvReceiver::~UdpSrvReceiver() {
  delete [] buf_.base;
}

int UdpSrvReceiver::start() {
    loop_ = uv_default_loop();

    uv_udp_init(loop_, &recv_socket_);
    struct sockaddr_in recv_addr;
    uv_ip4_addr("10.10.10.17", 27452, &recv_addr);
    uv_udp_bind(&recv_socket_, (const struct sockaddr *)&recv_addr, 0);// UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket_, alloc_cb_, read_cb_);

    return uv_run(loop_, UV_RUN_DEFAULT);
}

void uv_closed (uv_handle_t* handle) {
    (void)handle;
}

void UdpSrvReceiver::stop() {
  uv_udp_recv_stop(&recv_socket_);
  uv_close((uv_handle_t*)&recv_socket_, uv_closed);
  uv_stop(loop_);
  uv_loop_close(loop_);
}


void UdpSrvReceiver::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    (void)handle;
    (void)suggested_size;

    buf->base = buf_.base;
    buf->len = buf_.len;
}

void UdpSrvReceiver::on_read(uv_udp_t* req,
             ssize_t nread,
             const uv_buf_t* buf,
             const struct sockaddr* addr,
             unsigned flags) {
    (void)flags;

    if (nread < 0) {
        fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_udp_recv_stop(req);
        return;
    } else if (nread > 0) {
        char sender[17] = { 0 };
        uv_ip4_name((const struct sockaddr_in*)addr, sender, 16);

        process_package_cb_(buf->base, nread, sender);
    }
}
