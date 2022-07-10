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
template<typename T>
class OLAStretch : public baseStretch<T>{
  OLAStretch(unsigned int windowSize, float stretchValue, unsigned int sampleRate, unsigned int bufferSize)
  {
  }
};