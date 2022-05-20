/*
  ==============================================================================

    HPS.h
    Created: 4 Mar 2022 11:48:22am
    Author:  aless

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <complex>


class HPS {
    public:
        void process(juce::AudioBuffer<float> buffer);

    private:
        std::vector<std::complex<float>> harmonic;
        std::vector<std::complex<float>> percussive;

};