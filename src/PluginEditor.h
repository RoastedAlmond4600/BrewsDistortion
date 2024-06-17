#pragma once

#include "PluginProcessor.h"
#include "UI/RotaryKnob.h"
#include "UI/BrewsLookAndFeel.h"
//==============================================================================
class BrewsDistortionAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                  public juce::ComboBox::Listener
{
public:
    explicit BrewsDistortionAudioProcessorEditor (BrewsDistortionAudioProcessor&);
    ~BrewsDistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BrewsDistortionAudioProcessor& processorRef;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    using ComboBoxAttachment = APVTS::ComboBoxAttachment;
    RotaryKnob driveKnob;
    SliderAttachment driveAttachment {processorRef.apvts, ParameterID::input.getParamID(), driveKnob.slider};
    RotaryKnob outputKnob;
    SliderAttachment outputAttachment {processorRef.apvts, ParameterID::output.getParamID(), outputKnob.slider};
    RotaryKnob mixKnob;
    SliderAttachment mixAttachment {processorRef.apvts, ParameterID::mix.getParamID(), mixKnob.slider};
    RotaryKnob punishmentKnob;
    //Buttons
    juce::TextButton hardButton;
    juce::TextButton softButton;
    juce::TextButton saturateButton;
    juce::ComboBox modelBox;
    ComboBoxAttachment modelAttachment {processorRef.apvts, ParameterID::model.getParamID(), modelBox};
    BrewsLookAndFeel brewsLNF;
    void updateToggleState(juce::Button& button, juce::ComboBox& box, int index);
    std::atomic<int> currentModelId;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
};
