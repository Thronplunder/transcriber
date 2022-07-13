/*
  ==============================================================================

    Ringbuffer.h
    Created: 7 Mar 2022 12:22:27pm
    Author:  aless

  ==============================================================================
*/

#pragma once
#include <array>
#include <cmath>


//todo change to use audiosourcechannelinfo 

template<typename T, unsigned int Length>
class ringbuffer {
public:

    ringbuffer() : readCounter(0), writeCounter(0) {
        for (auto& it : data) {
            it = 0;
        };
    }

    void writeAddSample(T sample) {
        data.at(writeCounter) += sample;
        writeCounter = writeCounter + 1 % data.size();
    }
    void writeReplaceSample(T sample) {
        data.at(writeCounter) = sample;
        writeCounter = writeCounter + 1 % data.size();
    }

    void writeAddRingbuffer(T* dataToWrite, unsigned int length) {
        for (unsigned int i = 0; i < length; ++i) {
            data.at(writeCounter) += dataToWrite[i];
            writeCounter = writeCounter + 1 % data.size();
        }
    }

    void writeReplaceRingbuffer(T* dataToWrite, unsigned int length) {
        for (unsigned int i = 0; i < length; ++i) {
            data.at(writeCounter) = dataToWrite[i];
            writeCounter = writeCounter + 1 % data.size();
        }
    }
    void getNextAudioBlock(T* dataToFill, unsigned int length) {
        for (unsigned int i = 0; i < length; ++i) {
            dataToFill[i] = data.at(readCounter);
            data.at(readCounter) = 0;
            ++readCounter;
        }
    }

    void clear() {
        for (auto &it : data) {
            it = 0;
        }
    }
    
private:
    unsigned int readCounter;
    unsigned int writeCounter;
    std::array<T, Length> data;
};