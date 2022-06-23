/*
  ==============================================================================

    windowFunction.h
    Created: 17 Mar 2022 10:50:18am
    Author:  aless

  ==============================================================================
*/

#pragma once
//precomputed fixed size window function
#include<vector>
#include<numbers>
#include<cmath>

enum class windowType
{
    Hann,
    Triangle,
    Sine
};

template<typename T >
class Window {
public:
    Window(unsigned int length) : size(length) {

        //sets buffers to initial size and fills them 
        resize(length);
    };

    void applyWindow(T* buffer, unsigned int length, windowType type) {
        switch (type) {
            case windowType::Hann:
            for (int i = 0; i < length; ++i) {
                buffer[i] *= hannBuffer[i];
                }
            break;
            case windowType::Triangle:
                for (int i = 0; i < length; ++i) {
                    buffer[i] *= triangleBuffer[i];
                }
                break;
            case windowType::Sine:
                for (int i = 0; i < length; ++i) {
                    buffer[i] *= sineBuffer[i];
                }
                break;
        }
    };

    void resize(unsigned int newSize) {
        hannBuffer.resize(newSize);
        sineBuffer.resize(newSize);
        triangleBuffer.resize(newSize);
        size = newSize;
        for (int i = 0; i < size; ++i) { //recalculate values
            hannBuffer[i] = 0.5 * (1 - cos(2 * std::numbers::pi * ((float)i / size)));
            sineBuffer[i] = sin(std::numbers::pi * ((float)i /size));
            triangleBuffer[i] = 1 - std::abs((float)i / (size * 0.5) - 1);
        }
    }

    unsigned int getSize() {
        return size;
    }
private:
    unsigned int size;
    std::vector<T> hannBuffer, sineBuffer, triangleBuffer;


};