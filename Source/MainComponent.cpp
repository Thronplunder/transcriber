#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : state(transportState::Stopped), fileLoaded(false), audioLength(0.f), 
    inBuffer(2, bufferSize), outBuffer(2, bufferSize),  windowFunction(frameSize), analysisFrame(2, frameSize),
    outBufferWritePointer(analysisHopSize), outBufferReadPointer(0), inBufferPointer(0), inputHopCounter(0),
    fftResultLeft(frameSize), fftResultRight(frameSize), outPutHopCounter(0)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    addAndMakeVisible(transportField);
    addAndMakeVisible(controlField);

    transportField.setBoundsRelative(0.01f, 0.01f, 0.49f, 0.1f);
    transportField.playButton.addListener(this);
    transportField.stopButton.addListener(this);
    transportField.fileButton.addListener(this);

    controlField.setBoundsRelative(0.51f, 0.01f, 0.49f, 0.1f);
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    startTimer(50);
    fft.init(frameSize);
    fftResultLeft.resize(fft.ComplexSize(frameSize));
    fftResultRight.resize(fft.ComplexSize(frameSize));
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    inBuffer.clear();
    outBuffer.clear();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{

    if (fileReaderSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
    for (unsigned int sampleNum = 0; sampleNum < bufferToFill.numSamples; ++sampleNum) {
        for (juce::uint64 channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {//fill one frame of audio 
            inBuffer.setSample(channelNum, inBufferPointer, bufferToFill.buffer->getSample(channelNum, sampleNum));
        }
        inBufferPointer = (inBufferPointer + 1) % inBuffer.getNumSamples();
        if (++inputHopCounter == analysisHopSize) {//do stuff once one hop on the input side is filled
            fillAnalysisFrame();
            inputHopCounter = 0;
            //apply window function
            for (int channelNum = 0; channelNum < analysisFrame.getNumChannels(); ++channelNum) {
                auto writePointer = analysisFrame.getWritePointer(channelNum);
                auto numSamples = analysisFrame.getNumSamples();
                windowFunction.applyWindow(writePointer, numSamples, windowType::Hann);
            }
            //hardcoding both channels because im lazy
            fft.fft(analysisFrame.getReadPointer(0), fftResultLeft.getRealPointer(), fftResultLeft.getImagPointer());
            fft.fft(analysisFrame.getReadPointer(1), fftResultRight.getRealPointer(), fftResultRight.getImagPointer());

            //do more stuff


            //reverse fft
            fft.ifft(analysisFrame.getWritePointer(0), fftResultLeft.getRealPointer(), fftResultLeft.getImagPointer());
            fft.ifft(analysisFrame.getWritePointer(1), fftResultRight.getRealPointer(), fftResultRight.getImagPointer());
        }
        if (++outPutHopCounter == synthesisHopSize) {
            fillOutputBuffer();
        }

        //write outputbuffer to actual output
        for (unsigned int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {
            bufferToFill.buffer->setSample(channelNum, sampleNum, outBuffer.getSample(channelNum, outBufferReadPointer));
            outBuffer.setSample(channelNum, outBufferReadPointer, 0);
        }
        outBufferReadPointer = (outBufferReadPointer + 1) % outBuffer.getNumSamples();
    }
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    const int minutesTotal = audioLength / 60;
    const int secondsTotal = std::fmod(audioLength, 60.f);
    const int minutesPassed = transportSource.getCurrentPosition() / 60;
    const int secondsPassed = std::fmod(transportSource.getCurrentPosition(), 60.f);
    auto secondsTotalString = secondsTotal < 10 ? "0" + juce::String(secondsTotal) : juce::String(secondsTotal);
    auto secondsPassedString = secondsPassed < 10 ? "0" + juce::String(secondsPassed) : juce::String(secondsPassed);

    auto timeTotal = juce::String(minutesTotal) + ":" + secondsTotalString;
    auto timePassed = juce::String(minutesPassed) + ":" + secondsPassedString;
    auto completeString = timePassed + "/" + timeTotal;
    transportField.playTime.setText(completeString, juce::dontSendNotification);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    transportField.setBoundsRelative(0.01f, 0.01f, 0.49f, 0.1f);
    controlField.setBoundsRelative(0.5f, 0.01f, 0.5f, 0.1f);
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::timerCallback()
{
    repaint();
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* Source) {

    if (Source == &transportSource) {
        if (transportSource.isPlaying()) {
            changeState(transportState::Playing);
        }
        else if ((state == transportState::Stopping) || (state == transportState::Playing))
            changeState(transportState::Stopped);
        else if (transportState::Pausing == state)
            changeState(transportState::Paused);
    }

}

void MainComponent::buttonClicked(juce::Button* button) {
    if (button == &transportField.playButton) {
        playButtonClicked();
    }
    if (button == &transportField.stopButton) {
        stopButtonClicked();

    }
    if (button == &transportField.fileButton) {
        openButtonClicked();
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &controlField.stretchValueSlider) {
        stretchingFactor = slider->getValue();
    }
}
void MainComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged){
    if (comboBoxThatHasChanged == &controlField.analysisSizeComboBox) {
        auto id = comboBoxThatHasChanged->getSelectedId();
        switch (id) {
        case 1:
            frameSize = 512;
            break;
        case 2:
            frameSize = 1024;
            break;
        case 3:
            frameSize = 2048;
            break;
        case 4:
            frameSize = 4096;

        }
        fft.init(frameSize);
        fftResultLeft.resize(fft.ComplexSize(frameSize));
        fftResultRight.resize(fft.ComplexSize(frameSize));
        analysisFrame.setSize(2, frameSize, false);
        windowFunction.resize(frameSize);
    }
}


void MainComponent::changeState(transportState newState) {
if (state != newState)
{
    state = newState;

    switch (state)
    {
    case transportState::Stopped:                           // [3]
        transportField.stopButton.setButtonText("Stop");
        transportField.stopButton.setEnabled(false);
        transportField.playButton.setButtonText("Play");
        transportSource.setPosition(0.0);
        transportField.fileButton.setEnabled(true);
        controlField.setEnabled(true);
        inBuffer.clear();
        break;

    case transportState::Starting:                          // [4]
        //transportField.playButton.setEnabled(false);
        transportSource.start();
        transportField.fileButton.setEnabled(false);
        controlField.setEnabled(false);
        break;

    case transportState::Playing:                           // [5]
        transportField.stopButton.setEnabled(true);
        transportField.stopButton.setButtonText("Stop");
        transportField.playButton.setButtonText("Pause");
        break;

    case transportState::Pausing:
        transportSource.stop();
        break;

    case transportState::Paused:
        transportField.playButton.setButtonText("Resume");
        transportField.stopButton.setButtonText("Return to Zero");
        break;

    case transportState::Stopping:                          // [6]
        transportSource.stop();
        transportField.fileButton.setEnabled(true);

        break;
    }
}
}

void MainComponent::openButtonClicked() {
    fileChooser = std::make_unique<juce::FileChooser>("Select a File to Play", juce::File(), "*.wav;*.mp3;*.flac;*.ogg ");
#
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;
    fileChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();

        if (file != juce::File()) {
            auto audioFile = formatManager.createReaderFor(file);
            fileLoaded = true;

            if (audioFile != nullptr) {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(audioFile, true);
                transportSource.setSource(newSource.get(), 0, nullptr, audioFile->sampleRate);
                transportField.playButton.setEnabled(true);
                audioLength = audioFile->lengthInSamples / audioFile->sampleRate;
                fileReaderSource.reset(newSource.release());
            }
        }
    });
}

void MainComponent::playButtonClicked()
{
    if ((state == transportState::Stopped) || (state == transportState::Paused))
        changeState(transportState::Starting);
    else if (state == transportState::Playing) {
        changeState(transportState::Pausing);
    }
}

void MainComponent::stopButtonClicked()
{
    if (state == transportState::Paused) {
        changeState(transportState::Stopped);
    }
    else {
        changeState(transportState::Stopping);
    }
}

void MainComponent::fillAnalysisFrame() {
    //write the last analysisframesize samples into analysis frame
    juce::uint64 readStart;
    juce::uint64 buffersamples = inBuffer.getNumSamples();

    if (inBufferPointer <= frameSize) {
        readStart = (buffersamples  - (frameSize - inBufferPointer)) % buffersamples;
    }
    else {
        readStart = inBufferPointer - frameSize;
    }
    for (int sampleNum = 0; sampleNum < analysisFrame.getNumSamples(); ++sampleNum) {
        for (int channelNum = 0; channelNum < analysisFrame.getNumChannels(); ++channelNum) {
            auto valueToWrite = inBuffer.getSample(channelNum, readStart);
            analysisFrame.setSample(channelNum, sampleNum, valueToWrite);
        }
        ++readStart;
        if (readStart == buffersamples) {
            readStart = 0;
        }
    }
}

void MainComponent::fillOutputBuffer() {
    //write stuff to outputbuffer
    for (juce::uint64 channelNum = 0; channelNum < analysisFrame.getNumChannels(); ++channelNum) {
        for (juce::uint64 sampleNum = 0; sampleNum < analysisFrame.getNumSamples(); ++sampleNum) {
            auto writePointer = outBuffer.getWritePointer(channelNum);
            auto sampleIndex = (sampleNum + outBufferWritePointer) % outBuffer.getNumSamples();
            writePointer[sampleIndex] += analysisFrame.getSample(channelNum, sampleNum);
        }
    }
    outBufferWritePointer = (outBufferWritePointer + analysisHopSize) % outBuffer.getNumSamples();
    outPutHopCounter = 0;
}