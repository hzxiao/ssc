//
// Created by hz on 16-11-5.
//

#include "sess.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

KCPSession *
KCPSession::Dial(const char *ip, uint16_t port) {
    struct sockaddr_in sockAddress;
    memset(&sockAddress, 0, sizeof(sockAddress));

    sockAddress.sin_family = AF_INET;
    sockAddress.sin_port = htons(port);

    int ret = inet_pton(AF_INET, ip, &(sockAddress.sin_addr));
    if (ret <= 0) {
        return nullptr;
    }

    int sockFd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockFd == -1) {
        return nullptr;
    }

    ret = connect(sockFd, (sockaddr *) &sockAddress, sizeof(struct sockaddr));
    if (ret < 0) {
        close(sockFd);
        return nullptr;
    }

    KCPSession *sess = KCPSession::createSession(sockFd);
    if (sess == nullptr) {
        close(sockFd);
        return nullptr;
    }

    return sess;
}

KCPSession *
KCPSession::createSession(int sockFd) {
    int flags = fcntl(sockFd, F_GETFL, 0);
    if (flags < 0) {
        return nullptr;
    }

    if (fcntl(sockFd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return nullptr;
    }

    KCPSession *sess = new(KCPSession);
    sess->sockFd_ = sockFd;
    sess->kcp_ = ikcp_create(IUINT32(rand()), sess);
    sess->buff_ = (char *) malloc(KCPSession::mtuLimit);
    sess->streamBuff = (char *) malloc(KCPSession::streamBufferLimit);
    sess->kcp_->output = out_wrapper;

    if (sess->kcp_ == nullptr || sess->buff_ == nullptr || sess->streamBuff == nullptr) {
        if (sess->kcp_ != nullptr) {
            ikcp_release(sess->kcp_);
        }
        if (sess->buff_ != nullptr) {
            free(sess->buff_);
        }
        if (sess->streamBuff != nullptr) {
            free(sess->streamBuff);
        }
        return nullptr;
    }

    return sess;
}

void
KCPSession::Update(uint32_t current) noexcept {
    while (true) {
        ssize_t n = recv(sockFd_, buff_,KCPSession::mtuLimit, 0);
        if (n > 0) {
            ikcp_input(kcp_, static_cast<const char *> (buff_), n);
        } else{
            break;
        }
    }
    ikcp_update(kcp_, current);
}

void
KCPSession::Destroy(KCPSession *sess) {
    if (sess == nullptr) {
        return;
    }
    if (sess->sockFd_ != 0) {
        close(sess->sockFd_);
    }
    if (sess->kcp_ != nullptr) {
        ikcp_release(sess->kcp_);
    }
    if (sess->streamBuff != nullptr) {
        free(sess->streamBuff);
    }
    delete sess;
}

ssize_t
KCPSession::Read(char *buf, size_t sz) noexcept {
    if (streamBuffSize_ > 0) {
        size_t n = streamBuffSize_;
        if (n > sz) {
            n = sz;
        }
        memcpy(buf, streamBuff, n);

        streamBuffSize_ -= n;
        if (streamBuffSize_ != 0) {
            memmove(streamBuff, streamBuff + n, streamBuffSize_);
        }
        return n;
    }

    int psz = ikcp_peeksize(kcp_);
    if (psz < 0) {
        return 0;
    }

    if (psz <= sz) {
        return (ssize_t) ikcp_recv(kcp_, buf, int(sz));
    } else{
        ikcp_recv(kcp_, streamBuff, KCPSession::streamBufferLimit);
        memcpy(buf, streamBuff, sz);
        streamBuffSize_ = psz - sz;
        memmove(streamBuff, streamBuff + sz, psz - sz);
        return sz;
    }
}

ssize_t
KCPSession::Write(const char *buf, size_t sz) noexcept {
    int n = ikcp_send(kcp_, static_cast<const char *>(buf), int(sz));
    if (n == 0) {
        return sz;
    } else {
        return n;
    }
}

int
KCPSession::out_wrapper(const char *buf, int len, struct IKCPCB *, void *user) {
    assert(user != nullptr);
    KCPSession *sess = static_cast<KCPSession *>(user);
    return (int) sess->output(buf, static_cast<size_t>(len));
}

ssize_t
KCPSession::output(const void *buffer, size_t length) {
    ssize_t n = send(sockFd_, buffer, length, 0);
    return n;
}
