//
// Created by hz on 16-11-8.
//

#include <semaphore.h>
#include "sema.h"

Semaphore::Semaphore(unsigned int initCnt) {
    sem_init(&sem, 0, initCnt);
}

Semaphore::~Semaphore() {

}

bool
Semaphore::Get(Mode mode, unsigned long timeout) {
    return sem_wait(&sem) == 0;
}

bool
Semaphore::Post() {
    sem_post(&sem);
    return true;
}