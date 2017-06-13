/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ecuTimer.cpp
 * Author: visen
 * 
 * Created on June 12, 2017, 11:41 PM
 */

#include "ecuTimer.h"

ecuTimer::ecuTimer() {
}

ecuTimer::ecuTimer(const ecuTimer& orig) {
}

ecuTimer::~ecuTimer() {
}

void ecuTimer::set_delay(int delayMS){
    this->_delay = delayMS * 1000;
    delayTime.tv_sec = (long) delayMS / 1000 ;
    delayTime.tv_nsec = (long) delayMS * 1000000 ;
}

int ecuTimer::slp_delay(){
    printf("enter slp_delay for %d ms \n",this->_delay/1000);       // for debug propose
    usleep(_delay);
    printf("leave slp_delay for %d ms \n",this->_delay/1000);       // for debug propose
    return 0;
}

int ecuTimer::cond_delay(pthread_cond_t cond, pthread_mutex_t mutex){
    printf("enter cond_delay for %ld s \n",this->delayTime.tv_sec);       // for debug propose
    printf("enter cond_delay for %ld ns \n",this->delayTime.tv_nsec);       // for debug propose
    gettimeofday(&currentTime,NULL);
    pthread_cond_timedwait(&cond,&mutex,&delayTime);
    printf("leave cond_delay for %d ms \n",this->_delay/1000);       // for debug propose
    return 0;
}

void timer_test(int testCode){
    ecuTimer timer1,timer2;
    char input;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    switch(testCode){
        case 1:
            printf("enter timer test function 1: single thread, two slp_timer \n");
            timer1.set_delay(2000);
            timer2.set_delay(5000);
            timer1.slp_delay();
            timer2.slp_delay();
            break;
        case 2:
            printf("enter timer test function 2: multiple threads, one slp_timer each\n");
            if(0 != pthread_create(&thread1, NULL, thr_slp_dly, NULL)){
                printf("error when creating thread1 %d\n",errno);
            }
            if(0 != pthread_create(&thread2, NULL, thr_slp_dly, NULL)){
                printf("error when creating thread2 %d\n",errno);
            }
            while(input = getchar() != 'q');
            printf("test function 2 finished \n");
            break;
        case 3:
            printf("enter timer test function 2: multiple threads, one cond_timer each\n");
            if(0 != pthread_create(&thread1, NULL, thr_cond_dly, NULL)){
                printf("error when creating thread1 %d\n",errno);
            }
            if(0 != pthread_create(&thread2, NULL, thr_cond_dly, NULL)){
                printf("error when creating thread2 %d\n",errno);
            }
            while(input = getchar() != 'q');
            printf("test function 3 finished \n");
        default:
            break;
    }
}

void * thr_slp_dly(void * arg){
    printf("enter thread\n");
    pthread_mutex_lock(&mutex);
    ecuTimer selfTimer;
    selfTimer.set_delay(2000);
    selfTimer.slp_delay();
    pthread_mutex_unlock(&mutex);
    printf("delay finished, destruction\n");
}

void * thr_cond_dly(void * arg){
    printf("enter thread\n");
    pthread_mutex_lock(&mutex);
    ecuTimer selfTimer;
    selfTimer.set_delay(2000);
    selfTimer.cond_delay(cond,mutex);
    pthread_mutex_unlock(&mutex);
    printf("delay finished, destruction\n");
}