#include <vector>
#include "AudioFFT.h"
#include "WindowFunction.h"
#include "ComplexVector.h"

template<typename T>
class baseStretch{
    public:
    baseStretch(unsigned int windowSize, unsigned int bufferSize)
    : analysisFrameSize(windowSize),
    analysisHopSize(windowSize / 2),
    synthesisHopsize(windowSize/ 2),
    inBufferPointer(0),
    inputHopCounter(0),
    outbufferReadPointer(0),
    outBufferHopCounter(0),
    outBufferWritePointer(analysisHopSize),
    stretchValue(1.0)
    {
        inputBuffer.resize(3 * windowSize);
        outputBuffer.resize(bufferSize);
        analysisFrame.resize(analysisFrameSize);
        fft.init(analysisFrameSize);
        fftResult.resize(fft.ComplexSize(analysisFrameSize));
    }
    
    virtual void process(T* Data, unsigned int dataLength){
    }

    void changeStretchValue(float newValue){
        stretchValue = newValue;
        analysisHopSize =  synthesisHopsize / stretchValue;
        reset();
    }

    void changeBufferSize(unsigned int newSize){
        outputBuffer.resize(newSize);
        outputBuffer.clear();
        reset();
    }

    void fillAnalysisFrame();
    void fillOutputBuffer();

    void changeAnalysisSize(unsigned int newSize){
        analysisFrameSize = newSize;
        inputBuffer.resize(3* analysisFrameSize);
        synthesisHopsize = analysisFrameSize / 2;
        analysisHopSize = synthesisHopsize / stretchValue;
        analysisFrame.resize(newSize);
        window.resize(newSize);
        reset();
    }
    void reset(){
        inBufferPointer = 0;
        inputHopCounter = 0;
        outbufferReadPointer = 0;
        outBufferWritePointer = analysisFrameSize;
        outBufferHopCounter = 0;
    }

    protected:
    unsigned int analysisHopSize, analysisFrameSize, synthesisHopsize;
    unsigned int inBufferPointer, inputHopCounter;
    unsigned int outbufferReadPointer, outBufferWritePointer, outBufferHopCounter;
    float stretchValue;
    std::vector<T> inputBuffer, outputBuffer, analysisFrame;
    Window<T> window;
    audiofft::AudioFFT fft;
    ComplexVector<T> fftResult;
};