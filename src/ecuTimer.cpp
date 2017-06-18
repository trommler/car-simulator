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
#include <iostream>
#include <thread>

using namespace std;
using std::chrono::system_clock;

ecuTimer::ecuTimer() {
    t_id = 0;
}

ecuTimer::ecuTimer(const ecuTimer& orig) {
}

ecuTimer::~ecuTimer() {
}

/**
 * start the timer
 * if the timer is already running, calling this function resets the timer
 * @param ms time value in milliseconds before the timer wakes up
 * @return void
 **/
void ecuTimer::start(int ms)
{
    mutex_.lock();
    t_start_ = system_clock::now();
    duration_ = ms;
    t_id += 1;
    mutex_.unlock();

    thread t(&ecuTimer::sleep, this);
    t.detach();
    usleep(5000);
}


/**
 * this functions sleeps the appropriate amount of time
 * @return void
 **/
void ecuTimer::sleep()
{
    int id_begin;
    int id_now;
    int duration;

    mutex_.lock();
    id_begin = t_id;
    duration = duration_;
    mutex_.unlock();

    usleep(useconds_t(duration_) * useconds_t(1000));

    while (true)
    {
        mutex_.lock();
        auto diff = chrono::duration_cast<chrono::milliseconds>(system_clock::now() - t_start_);
        id_now = t_id;
        duration = duration_;
        mutex_.unlock();

        if (id_begin != id_now)
        {
            return;  // timer has been restarted
        }
        if (diff.count() >= duration)
        {
            break;
        }
        else
        {
            int sleep_for_ms = duration - diff.count();
            usleep(useconds_t(sleep_for_ms) * useconds_t(1000));
        }
    }

    // wakeup procedure depends on the derived class
    // call this only if the timer wasn't restarted
    mutex_.lock();
    bool cond = (id_now == t_id);
    mutex_.unlock();

    if (cond)
    {
        wakeup();
    }
}

/**
 * reset the timer
 * @return void
 */
void ecuTimer::reset()
{
    mutex_.lock();
    t_start_ = system_clock::now();
    mutex_.unlock();
}



/**
 * set delay value for the timer
 * @param delay time value, unit Millisecond
 * @notice: the value will be set in both the slp_timer and cond_timer
 * @return void
 **/
//void ecuTimer::set_delay(int delayMS){
    //this->delay_ = delayMS * 1000;
    //delayTime.tv_sec = (long) delayMS / 1000 ;
    //delayTime.tv_nsec = (long) delayMS * 1000000 ;
//}

/**
 *   start the slp_timer and delay for the time value set before
 *  @param none
 *  @notice: slp_timer is perfect for single thread application
 *          problems or bugs in multi threading need to be further tested
 *  @return always 0
 **/
//int ecuTimer::slp_delay(){
    //printf("enter slp_delay for %d ms \n",this->delay_/1000);       // for debug propose
    //usleep(delay_);
    //printf("leave slp_delay for %d ms \n",this->delay_/1000);       // for debug propose
    //wakeup();
    //return 0;
//}

/*  start the cond_timer and delay for the time value set before
 *  @param pthread_cond_t cond, signal for multi threading, should be defined outside
 *         ptread_mutex_t mutex, signal for multi threading, should be defined outside
 *  @notice: cond_timer is perfect for multi thread application
 *  @return always 0
 **/
//int ecuTimer::cond_delay(pthread_cond_t cond, pthread_mutex_t mutex){
    //printf("enter cond_delay for %ld s \n",this->delayTime.tv_sec);       // for debug propose
    //printf("enter cond_delay for %ld ns \n",this->delayTime.tv_nsec);       // for debug propose
    //gettimeofday(&currentTime,NULL);
    //pthread_cond_timedwait(&cond,&mutex,&delayTime);
    //printf("leave cond_delay for %d ms \n",this->delay_/1000);       // for debug propose
    //return 0;
//}


/**
 * one little test function for timers
 * @param testCode ,test conditions
 */
//void timer_test(int testCode){
    //ecuTimer timer1,timer2;
    //char input;
    //pthread_mutex_init(&mutex,NULL);
    //pthread_cond_init(&cond,NULL);
    //switch(testCode){
        //case 1:
            //printf("enter timer test function 1: single thread, two slp_timer \n");
            //timer1.set_delay(2000);
            //timer2.set_delay(5000);
            //timer1.slp_delay();
            //timer2.slp_delay();
            //break;
        //case 2:
            //printf("enter timer test function 2: multiple threads, one slp_timer each\n");
            //if(0 != pthread_create(&thread1, NULL, thr_slp_dly, NULL)){
                //printf("error when creating thread1 %d\n",errno);
            //}
            //if(0 != pthread_create(&thread2, NULL, thr_slp_dly, NULL)){
                //printf("error when creating thread2 %d\n",errno);
            //}
            //while(input = getchar() != 'q');
            //printf("test function 2 finished \n");
            //break;
        //case 3:
            //printf("enter timer test function 2: multiple threads, one cond_timer each\n");
            //if(0 != pthread_create(&thread1, NULL, thr_cond_dly, NULL)){
                //printf("error when creating thread1 %d\n",errno);
            //}
            //if(0 != pthread_create(&thread2, NULL, thr_cond_dly, NULL)){
                //printf("error when creating thread2 %d\n",errno);
            //}
            //while(input = getchar() != 'q');
            //printf("test function 3 finished \n");
        //default:
            //break;
    //}
//}

/**
 * one example of how slp_timer is used in thread
 * @param arg
 * @return
 */
//void * thr_slp_dly(void * arg){
    //printf("enter thread\n");
    //pthread_mutex_lock(&mutex);
    //ecuTimer selfTimer;
    //selfTimer.set_delay(2000);
    //selfTimer.slp_delay();
    //pthread_mutex_unlock(&mutex);
    //printf("delay finished, destruction\n");
//}

/**
 * one example of how cond_timer is used in thread
 * @param arg
 * @return
 */
//void * thr_cond_dly(void * arg){
    //printf("enter thread\n");
    //pthread_mutex_lock(&mutex);
    //ecuTimer selfTimer;
    //selfTimer.set_delay(2000);
    //selfTimer.cond_delay(cond,mutex);
    //pthread_mutex_unlock(&mutex);
    //printf("delay finished, destruction\n");
//}





/**
 * overwritten wakeup function, just a demo.
 * in a real timer do some useful stuff here, like state switch of the server
 */
void SomePurposeTimer23_42::wakeup()
{
    cout << "you clearly wanted to call me to to something." << endl;
}
