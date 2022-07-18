#include "AudioFFT.h"
#include "ComplexVector.h"
#include "WindowFunction.h"
#include <vector>

class baseStretch {
public:
  baseStretch(unsigned int windowSize, unsigned int bufferSize)
      : analysisFrameSize(windowSize), analysisHopSize(windowSize / 2),
        synthesisHopsize(windowSize / 2), inBufferPointer(0),
        inputHopCounter(0), outbufferReadPointer(0), outBufferHopCounter(0),
        outBufferWritePointer(analysisHopSize), stretchValue(1.0), window(windowSize) {
    inputBuffer.resize(3 * windowSize);
    outputBuffer.resize(bufferSize);
    analysisFrame.resize(analysisFrameSize);
  }

  virtual void process(float *data, unsigned int dataLength);

  void changeStretchValue(float newValue) {
    stretchValue = newValue;
    analysisHopSize = synthesisHopsize / stretchValue;
    reset();
  }

  void changeBufferSize(unsigned int newSize) {
    outputBuffer.resize(newSize);
    outputBuffer.clear();
    reset();
  }

  void fillAnalysisFrame(){
    unsigned int bufferPointer = inBufferPointer > analysisFrameSize ? inBufferPointer - analysisFrameSize : (inputBuffer.size() + inBufferPointer) - analysisFrameSize;
    for(auto it : analysisFrame){
        it = inputBuffer[bufferPointer];
        bufferPointer = (bufferPointer + 1) % inputBuffer.size();
    };
  }
  void fillOutputBuffer(){
    for(auto it : analysisFrame){
      outputBuffer[outBufferWritePointer] = it;
      outBufferWritePointer = (outBufferWritePointer + 1) % outputBuffer.size();
    }
  };

  void changeAnalysisSize(unsigned int newSize) {
    analysisFrameSize = newSize;
    inputBuffer.resize(3 * analysisFrameSize);
    synthesisHopsize = analysisFrameSize / 2;
    analysisHopSize = synthesisHopsize / stretchValue;
    analysisFrame.resize(newSize);
    window.resize(newSize);
    reset();
  }
  void reset() {
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
  std::vector<float> inputBuffer, outputBuffer, analysisFrame;
  Window<float> window;
};