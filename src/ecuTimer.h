/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ecuTimer.h
 * Author: visen
 *
 * Created on June 12, 2017, 11:41 PM
 */

#ifndef ECUTIMER_H
#define ECUTIMER_H

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
using namespace std;

class ecuTimer {
public:
    ecuTimer();
    ecuTimer(const ecuTimer& orig);
    virtual ~ecuTimer();
    void set_delay(int delay);
    int slp_delay();
    int cond_delay(pthread_cond_t cond, pthread_mutex_t mutex); 
private:
    int _delay;
    struct timeval currentTime;
    struct timespec delayTime;
};

static pthread_t thread1,thread2;
static pthread_cond_t cond;
static pthread_mutex_t mutex; 
void timer_test(int testCode);
void * thr_slp_dly(void * arg);
void * thr_cond_dly(void * arg);

#endif /* ECUTIMER_H */
