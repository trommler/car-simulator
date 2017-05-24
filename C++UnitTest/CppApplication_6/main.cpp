/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: liu
 *
 * Created on 16. Mai 2017, 12:14
 */

#include <cstdlib>
#include "Complex.h"
#include <iostream>
using namespace std;


int main(int argc, char** argv) {
    Complex *m1, *m2, *m3, *m4;
    m1 = new Complex(10,1);
    m2 = new Complex(5,1);
    m3 = new Complex(0,0);
    m4 = new Complex(5,1);
    
    if(*m1==*m2){cout<<"yes\n"<<endl;}
    if(*m2==*m4){cout<<"yes\n"<<endl;}
    *m2/(*m3);
    
    return 0;
}

