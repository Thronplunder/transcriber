/*
  ==============================================================================

    ComplexVector.h
    Created: 14 Apr 2022 10:06:40am
    Author:  aless

  ==============================================================================
*/
#include<vector>

#pragma once
//wrapper around vector to create a vector of complex numbers able to return vectors of real and imaginary parts

template<typename T>
class ComplexVector {
public:
    ComplexVector(unsigned int length): real(length), imag(length), length(length) {

    };
    //return vectors
    std::vector<T>& getReal() {
        return real;
    }
    std::vector<T>& getImag() {
        return imag;
    }
    //return pointer to raw data

    T* getRealPointer() {
        return real.data();
    }

    T* getImagPointer() {
        return imag.data();
    }

    //return size
    unsigned int getSize() {
        return length;
    }

    void resize(unsigned int newLength) {
        real.resize(newLength);
        imag.resize(newLength);
        length = newLength;
    }
private:
    std::vector<T> real;
    std::vector<T> imag;
    unsigned int length;
};