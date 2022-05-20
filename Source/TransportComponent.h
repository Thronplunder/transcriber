/*
  ==============================================================================

    TransportComponent.h
    Created: 10 Feb 2022 12:35:31pm
    Author:  aless

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TransportComponent.h"
#include <cmath>

//==============================================================================
/*
*/
class TransportComponent  : public juce::Component
{
public:
    TransportComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        const float relativeWidth = 1.f / 4.f; //width = 1 / number of components
        addAndMakeVisible(playButton);
        addAndMakeVisible(stopButton);
        addAndMakeVisible(fileButton);
        addAndMakeVisible(playTime);
        playTime.setText("X:XX/X:XX", juce::dontSendNotification);
        fileButton.setButtonText("Open File");
        playButton.setButtonText("Play");
        playButton.setEnabled(false);
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        stopButton.setButtonText("Stop");
        stopButton.setEnabled(false);
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        fileButton.setBoundsRelative(0.f * relativeWidth, 0.f, relativeWidth, 1.f);
        playButton.setBoundsRelative(1.f * relativeWidth, 0.f, relativeWidth, 1.f);
        stopButton.setBoundsRelative(2.f * relativeWidth, 0.f, relativeWidth, 1.f);
        playTime.setBoundsRelative(3.f * relativeWidth, 0.f, relativeWidth, 1.f);

    }

    ~TransportComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        //g.setColour (juce::Colours::grey);
        //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //g.setColour (juce::Colours::white);
        //g.setFont (14.0f);
        //g.drawText ("TransportComponent", getLocalBounds(),
        //            juce::Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        const float relativeWidth = 1.f / 4.f; //width = 1 / number of components
        fileButton.setBoundsRelative(0.f * relativeWidth, 0.f, relativeWidth, 1.f);
        playButton.setBoundsRelative(1.f * relativeWidth, 0.f, relativeWidth, 1.f);
        stopButton.setBoundsRelative(2.f * relativeWidth, 0.f, relativeWidth, 1.f);
        playTime.setBoundsRelative(3.f * relativeWidth, 0.f, relativeWidth, 1.f);
    }
    juce::TextButton fileButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label playTime;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComponent)
};
