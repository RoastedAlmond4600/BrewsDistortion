#pragma once
#include <JuceHeader.h>
class RotaryKnob : public juce::Component {
public:
    RotaryKnob();
    ~RotaryKnob() override;
    void paint(juce::Graphics& g);
    void resized();
    juce::Slider slider;
    juce::String label;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
};
