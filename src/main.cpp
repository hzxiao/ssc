#include <unistd.h>
#include <sys/time.h>
#include "server/sess.h"
#include "util/msg_queue.h"
#include <iostream>
#include <cstring>
#include <thread>
#include "proto/protob.pb.h"
#include "arduino/cArduino.h"


using namespace std;

IUINT32 iclock();

void handleSession(KCPSession *sess) {

    char buf[1024];

    cArduino arduino(ArduinoBaundRate::B19200bps, "/dev/ttyACM0");

    for (;;) {
        sess->Update(iclock());
        memset(buf, 0, 1024);
        ssize_t n = sess->Read(buf, 1024);
        if (n > 0) {
            printf("%s\n", buf);
            string msg(buf);
            arduino.write(msg);
            arduino.flush();
        }
//        serialSession->Write(buf, n);
    }
}


int main() {

//    ---------------------- 与golang服务器连接的测试代码---------------
//    //---------------------- 与golang服务器连接的测试代码---------------
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    KCPSession *sess = KCPSession::Dial("192.168.1.109", 29990);
    sess->NoDelay(1, 20, 2, 1);
    sess->WndSize(128, 128);
    sess->SetMtu(1400);
    assert(sess != nullptr);
    ssize_t nsent = {0};
    ssize_t nrecv = {0};
    char *buf = (char *) malloc(128);

    cArduino arduino(ArduinoBaundRate::B19200bps, "/dev/ttyACM0");


    sprintf(buf, "iam");
    sess->Write(buf, strlen(buf));
    for (;;) {
        sess->Update(iclock());
        memset(buf, 0, 128);
        ssize_t n = sess->Read(buf, 128);
        if (n > 0) {

            string msg(buf);
            Msg pb;
            pb.ParseFromString(msg);
            cout << pb.content() << endl;
//            arduino.write(pb.content());
//            arduino.flush();
        }
        nrecv += n;

        usleep(33);
//        if (nsent == nrecv) break;
    }
    KCPSession::Destroy(sess);
}


void
itimeofday(long *sec, long *usec) {
    struct timeval time;
    gettimeofday(&time, NULL);
    if (sec) *sec = time.tv_sec;
    if (usec) *usec = time.tv_usec;
}

IUINT64 iclock64(void) {
    long s, u;
    IUINT64 value;
    itimeofday(&s, &u);
    value = ((IUINT64) s) * 1000 + (u / 1000);
    return value;
}

IUINT32 iclock() {
    return (IUINT32) (iclock64() & 0xfffffffful);
}





//void* readSerial(void *arg) {
//    char buf[256];
//
//    SerialSession *sess = static_cast<SerialSession *> (arg);
//
//
//    while (true) {
//        memset(&buf, 0, sizeof(buf));
//
//        int n = 0;
//
//        while ((n = sess->Read(buf, sizeof(buf))) > 0) {
//            for (int i = 0; i < n; i++) {
//                //处理数据
//            }
//        }
//    }
//}

//    MsgQueue<int *> q;
//    int i = 12;
//    q.sendMsg(&i);
//
//    int *t = q.receiveMsg();
//    cout << *t << endl;

//    pb::Msg msg;
//    msg.set_content("123");






//    std::string msg_str;

//    q.receiveMsg();

//    char *deviceName = (char *) "/dev/ttyACM0";
//    SerialSession *sess = SerialSession::Connect(deviceName);
//
//    assert(sess != nullptr);
//
//    char buf[256];
//    char write_buf[256];
//
//    thread readS(readSerial, sess);
//    readS.detach();
//
//    //模拟写数据到串口
//    while(true) {
//        memset(&write_buf, 0, sizeof(write_buf));
//
//        strcpy(write_buf, "1\0");
//        sess->Write(write_buf, strlen(write_buf));
//        sleep(2);
//
//    }

//---------------------- 与golang服务器连接的测试代码---------------
//    struct timeval time;
//    gettimeofday(&time, NULL);
//    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
//
//    KCPSession *sess = KCPSession::Dial("127.0.0.1", 9995);
//    sess->NoDelay(1, 20, 2, 1);
//    sess->WndSize(128, 128);
//    sess->SetMtu(1400);
//    assert(sess != nullptr);
//    ssize_t nsent = {0};
//    ssize_t nrecv = {0};
//    char *buf = (char *) malloc(128);
//
//    for (int i = 0; i < 10; i++) {
//        sprintf(buf, "message:%d", i);
//        sess->Write(buf, strlen(buf));
//        nsent += strlen(buf);
//    }
//
//    for (;;) {
//        sess->Update(iclock());
//        memset(buf, 0, 128);
//        ssize_t n = sess->Read(buf, 128);
//        if (n > 0) { printf("%s\n", buf); }
//        nrecv += n;
//
//        usleep(33);
////        if (nsent == nrecv) break;
//    }
//    KCPSession::Destroy(sess);