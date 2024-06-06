#include "RotaryKnob.h"
#include <JuceHeader.h>
static constexpr int labelHeight = 15;
static constexpr int textBoxHeight = 40;

RotaryKnob::RotaryKnob() {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 10, textBoxHeight);
    addAndMakeVisible(slider);
}

RotaryKnob::~RotaryKnob() {

}

void RotaryKnob::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::white);
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::RalewayVariableFont_wght_ttf, BinaryData::RalewayVariableFont_wght_ttfSize);
    g.setFont(typeface);
    g.setFont(18);
    g.setColour(juce::Colours::black);
    g.drawText(label, juce::Rectangle<int>{0, 0, bounds.getWidth(), labelHeight}, juce::Justification::centred);
    g.setColour(juce::Colours::green);
}

void RotaryKnob::resized() {
    auto bounds = getLocalBounds();
    slider.setBounds(0, labelHeight, bounds.getWidth(), bounds.getHeight() - labelHeight);
}
