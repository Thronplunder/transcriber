/*
  ==============================================================================

    ControlComponent.h
    Created: 6 May 2022 3:39:53pm
    Author:  aless

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ControlComponent : public juce::Component {
public:
    ControlComponent() {
        stretchValueSlider.setBoundsRelative(comboBoxRatio, 0.0f, sliderRatio, 1.0f);
        stretchValueSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        stretchValueSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 40, stretchValueSlider.getHeight());
        stretchValueSlider.setRange(1.0, 2.0, 0.01);
        addAndMakeVisible(analysisSizeComboBox);
        addAndMakeVisible(stretchValueSlider);
        analysisSizeComboBox.setBoundsRelative(0.0f, 0.0f, comboBoxRatio, 0.5f);
        analysisSizeComboBox.addItem("512", 1);
        analysisSizeComboBox.addItem("1024", 2);
        analysisSizeComboBox.addItem("2048", 3);
        analysisSizeComboBox.addItem("4096", 4);
        
    };
    ~ControlComponent() override {
    };
    void resized() override {
        stretchValueSlider.setBoundsRelative(comboBoxRatio, 0.0f, sliderRatio, 1.0f);
        analysisSizeComboBox.setBoundsRelative(0.0f, 0.0f, comboBoxRatio, 0.5f);
        analysisSizeComboBox.setSelectedId(1);
    };

    juce::ComboBox analysisSizeComboBox;
    juce::Slider stretchValueSlider;
private:
    static constexpr float comboBoxRatio = 0.25;
    static constexpr float sliderRatio = 0.75;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlComponent)
};