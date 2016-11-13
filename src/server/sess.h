//
// Created by hz on 16-11-5.
//

#ifndef SSCAR_SESS_H
#define SSCAR_SESS_H

#include "ikcp.h"
#include "io.h"
#include <sys/types.h>
#include <cstdint>

class KCPSession: public ReadWriter {
public:
    KCPSession(const KCPSession &) = delete;

    //Dial 连接远程服务器并返回一个连接对象的指针
    static KCPSession* Dial(const char* ip, uint16_t port);
    //Update 尝试重新读写UPD报文, current为通过的毫秒
    void Update(uint32_t current) noexcept;

    static void Destroy(KCPSession *sess);
    // Read reads from kcp with buffer size sz.
    ssize_t Read(char *buf, size_t sz) noexcept;

    // Write writes into kcp with buffer size sz.
    ssize_t Write(const char *buf, size_t sz) noexcept;

    // Wrappers for kcp control
    inline int NoDelay(int nodelay, int interval, int resend, int nc) {
        return ikcp_nodelay(kcp_, nodelay, interval, resend, nc);
    }

    inline int WndSize(int sendWind, int recvWind) { return ikcp_wndsize(kcp_, sendWind, recvWind); }

    inline int SetMtu(int mtu) { return ikcp_setmtu(kcp_, mtu); }

private:
    KCPSession() = default;
    ~KCPSession() = default;
    // out_wrapper
    static int out_wrapper(const char *buf, int len, struct IKCPCB *kcp, void *user);

    // output udp packet
    ssize_t output(const void *buffer, size_t length);

    static KCPSession *createSession(int sockFd);

private:
    int sockFd_{0};
    ikcpcb *kcp_{nullptr};
    char *buff_{nullptr};
    char *streamBuff{nullptr};
    size_t streamBuffSize_{0};

    static const size_t mtuLimit{2048};
    static const size_t streamBufferLimit{65535};
};


#endif //SSCAR_SESS_H
