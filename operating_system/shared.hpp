//shared.hpp
//Sandy Zhang
//3/28/25

#ifndef SHARED_HPP
#define SHARED_HPP

//Needed for shared memory
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>

//Needed for producer/consumer
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

struct table{
    int table[2];
    int item = 0;
    sem_t full;
    sem_t empty;
    sem_t S;
};

#endif