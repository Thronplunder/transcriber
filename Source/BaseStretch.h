#include<vector>
#include "AudioFFT.h"

template<typename T, int length>
class baseStretch{
    public:
    baseStretch(unsigned int windowSize, float stretchValue, unsigned int sampleRate);
    process(T* inputData, unsigned int length);

    private:
    unsigned int analysisHopSize, windowSize, synthesisHopsize;
    unsigned int inBufferPointer, inputHopCounter;
    unsigned int oubufferReadPointer, outBufferWritePointer, outBufferHopCounter;
    std::vector<T> inputBuffer, outputBuffer;

    
}