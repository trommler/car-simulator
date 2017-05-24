/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Complex.h
 * Author: liu
 *
 * Created on 16. Mai 2017, 12:16
 */

#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdexcept>
#include <iostream>
class Complex {
public:
    Complex(const double& r, const double& i);
    Complex();
    virtual ~Complex();
private:
    friend bool operator ==(const Complex& a, const Complex& b);
    friend Complex& operator +(const Complex& a, const Complex& b);
    friend Complex& operator /(const Complex& a, const Complex& b);
    double real, imaginary;
};

#endif /* COMPLEX_H */

