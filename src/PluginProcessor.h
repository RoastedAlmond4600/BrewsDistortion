#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <JuceHeader.h>
#include "Utils.h"
#include "DSP/Distortion.h"
namespace ParameterID {
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
    PARAMETER_ID(input);
    PARAMETER_ID(output);
    PARAMETER_ID(mix);
    PARAMETER_ID(ceiling);
    PARAMETER_ID(model);
}
//==============================================================================
class BrewsDistortionAudioProcessor final : public juce::AudioProcessor,
	                                     private juce::ValueTree::Listener
{
public:
    //==============================================================================
    BrewsDistortionAudioProcessor();
    ~BrewsDistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "apvts", createParameterLayout()};

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrewsDistortionAudioProcessor)
    //Parameter Pointers
    juce::AudioParameterFloat* inputParam;
    juce::AudioParameterFloat* outputParam;
    juce::AudioParameterFloat* mixParam;
    juce::AudioParameterFloat* ceilingParam;
    juce::AudioParameterChoice* modelParam;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    Distortion<float> distModule;
    std::atomic<bool> parameterChanged {true};
    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) {
        parameterChanged.store(true);
    }
    void update();
};
