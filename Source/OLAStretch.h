/*
  ==============================================================================

    OLAStretch.h
    Created: 1 Jul 2022 8:50:07pm
    Author:  aless

  ==============================================================================
*/

#pragma once
#include "BaseStretch.h"
//output buffer has to be at least length of audio overhang (50% of full audio for 0.5 str5tch value)
class OLAStretch : public baseStretch{
  OLAStretch(unsigned int windowSize, unsigned int bufferSize)
  : baseStretch ( windowSize,  bufferSize){};

  void process(float* data, unsigned int dataLength) override {
    for(int i = 0; i < dataLength; i++){
      inputBuffer[inBufferPointer] = data[i];
      inBufferPointer = ( inBufferPointer + 1 ) % inputBuffer.size();

      if(inputHopCounter++ == analysisHopSize){
        fillAnalysisFrame();
        inputHopCounter = 0;
      }

      if(outBufferHopCounter++ == synthesisHopsize){
        outBufferHopCounter = 0;
        fillOutputBuffer();
      }
    }

  } ;
};