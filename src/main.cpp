#include <unistd.h>
#include <sys/time.h>
#include "server/sess.h"
#include "util/msg_queue.h"
#include <iostream>
#include <cstring>
#include <thread>
#include "proto/protob.pb.h"
#include "arduino/cArduino.h"
#include "conf/config.h"


using namespace std;

IUINT32 iclock();



Config *setting;
cArduino *arduino;

int main() {

    string file("./cfg.conf");
    setting = new Config(file);
    assert(setting != nullptr);

    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    string serverIP;
    serverIP = setting->GetString("serverIP", "127.0.0.1");
    int port = setting->GetInt("serverPort", 9999);
    KCPSession *sess = KCPSession::Dial(serverIP.c_str(), port);
    sess->NoDelay(1, 20, 2, 1);
    sess->WndSize(128, 128);
    sess->SetMtu(1400);
    assert(sess != nullptr);
    ssize_t nsent = {0};
    ssize_t nrecv = {0};
    char *buf = (char *) malloc(128);


    char *name = (char *) "/dev/ttyACM0";
    arduino = new cArduino(ArduinoBaundRate::B19200bps, name);


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
            arduino->write(pb.content());
            arduino->flush();
        }
        nrecv += n;

        usleep(33);
    }
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
