//
// Created by hz on 16-11-8.
//

#ifndef SSCAR_SEMA_H
#define SSCAR_SEMA_H


#include <semaphore.h>

class Semaphore {
public:
    typedef enum {
        kTimeout,
        kForever
    }Mode;

    Semaphore(unsigned int initCnt);
    ~Semaphore();

    bool Get(Mode mode = kForever, unsigned long timeout = 0);

    bool Post();

private:
    sem_t sem;
};


#endif //SSCAR_SEMA_H
