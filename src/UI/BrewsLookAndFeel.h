#pragma once
#include <JuceHeader.h>
#include "res/BinaryData.h"
class BrewsLookAndFeel : public juce::LookAndFeel_V4 {
public:
    BrewsLookAndFeel();
    ~BrewsLookAndFeel();
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    juce::Slider::SliderLayout getSliderLayout(Slider& slider) override;
    juce::Font getLabelFont(Label& label) override;
    void drawButtonBackground(Graphics& g, juce::Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonDown) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrewsLookAndFeel)
};
