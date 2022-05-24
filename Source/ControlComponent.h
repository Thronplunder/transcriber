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
        stretchValueSlider.setValue(1.0);
        addAndMakeVisible(analysisSizeComboBox);
        addAndMakeVisible(stretchValueSlider);
        analysisSizeComboBox.setBoundsRelative(0.0f, 0.0f, comboBoxRatio, 0.5f);
        analysisSizeComboBox.addItem("512", 1);
        analysisSizeComboBox.addItem("1024", 2);
        analysisSizeComboBox.addItem("2048", 3);
        analysisSizeComboBox.addItem("4096", 4);
        analysisSizeComboBox.setSelectedId(1);
        
    };
    ~ControlComponent() override {
    };
    void resized() override {
        stretchValueSlider.setBoundsRelative(comboBoxRatio, 0.0f, sliderRatio, 1.0f);
        analysisSizeComboBox.setBoundsRelative(0.0f, 0.0f, comboBoxRatio, 0.5f);
        analysisSizeComboBox.setSelectedId(1);
    };

    unsigned int getFrameSize() {
        auto id = analysisSizeComboBox.getSelectedId();
        switch (id) {
        case 1:
            return 512;
        case 2:
            return 1024;
        case 3:
            return 2048;
            
        case 4:
            return 4096;
        }
    }

    juce::ComboBox analysisSizeComboBox;
    juce::Slider stretchValueSlider;
private:
    static constexpr float comboBoxRatio = 0.25;
    static constexpr float sliderRatio = 0.75;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlComponent)
};