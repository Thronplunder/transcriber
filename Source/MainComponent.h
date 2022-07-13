#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include "TransportComponent.h"
#include "Ringbuffer.h"
#include "WindowFunction.h"
#include "AudioFFT.h"
#include "ComplexVector.h"
#include "ControlComponent.h"
//#include "OLAStretch.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
    public juce::ChangeListener,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::ComboBox::Listener,
    public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override ;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    //==============================================================================
    void timerCallback() override;
    //==============================================================================
private:
    //==============================================================================
    // Your private member variables go here...
    enum class transportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Pausing,
        Paused
    };

    void changeState(transportState newState);
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void fillAnalysisFrame();
    void fillOutputBuffer();
    void setHopsizes();
    void resetCounters();


    static constexpr unsigned int bufferSize = 16384;
    unsigned int frameSize;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> fileReaderSource;
    juce::AudioTransportSource transportSource;
    TransportComponent transportField;
    ControlComponent controlField;

    double audioLength;
    bool fileLoaded;
    Window<float> windowFunction;
    std::unique_ptr<juce::FileChooser> fileChooser;
    transportState state;
    

    //==============================================================================
    //inputBuffer
    juce::AudioBuffer<float> inBuffer;
    juce::uint64 inBufferPointer;
    unsigned int analysisHopSize;
    juce::uint64 inputHopCounter;
    unsigned int analysisCounter;


    //==============================================================================
    //outputBuffer
    juce::AudioBuffer<float> outBuffer;
    juce::uint64 outBufferWritePointer;
    juce::uint64 outBufferReadPointer;
    unsigned int synthesisHopSize;
    juce::uint64 outPutHopCounter;

    //==============================================================================
    //analysis Frame
    juce::AudioBuffer<float> analysisFrame;

    //==============================================================================
    //fft things
    audiofft::AudioFFT fft;
    ComplexVector<float> fftResultLeft, fftResultRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
