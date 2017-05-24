/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Complex.cpp
 * Author: liu
 * 
 * Created on 16. Mai 2017, 12:16
 */

#include "Complex.h"



Complex::Complex(){
}

Complex::Complex(const double& r, const double& i){
    this->real = r;
    this->imaginary = i;
}

Complex::~Complex() {
}

bool operator==(const Complex& a, const Complex& b){
    return a.real == b.real && a.imaginary == b.imaginary;
}

Complex& operator +(const Complex& a, const Complex& b){
    Complex *n = new Complex();
    n->real = a.real + b.real;
    n->imaginary = a.imaginary + b.imaginary;
    
    return *n;
}

Complex& operator /(const Complex& a, const Complex& b){
    Complex *n = new Complex();
    if(b.real == 0 && b.imaginary == 0)
        throw std::overflow_error("DividedByZeroException");
    std::cout<<"yes, it works\n"<<std::endl;
    return *n;
}