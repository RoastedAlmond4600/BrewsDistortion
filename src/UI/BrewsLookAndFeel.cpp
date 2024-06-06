#include "BrewsLookAndFeel.h"

BrewsLookAndFeel::BrewsLookAndFeel() {
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(255, 255, 255));
    setColour(juce::Slider::backgroundColourId, juce::Colour(255, 0, 0));
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(100, 100, 100));
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(255, 255, 255));
    setColour(juce::Slider::thumbColourId, juce::Colour(255, 0, 0));
    setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(160, 0, 0));
    setColour(juce::Label::textColourId, juce::Colour(0, 0, 0));

    setColour(juce::TextButton::buttonColourId, juce::Colour(255, 255, 255));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(255, 255, 255));
    //setColour(juce::ComboBox::outlineColourId, juce::Colour(0, 0, 0));
    setColour(juce::TextButton::textColourOffId, juce::Colour(0, 0, 0));
    setColour(juce::TextButton::textColourOnId, juce::Colour(255, 0, 0));
}

BrewsLookAndFeel::~BrewsLookAndFeel() {

}

void BrewsLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) {
    auto outlineColor = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
    auto fillColour = slider.findColour(juce::Slider::rotarySliderFillColourId);
    auto dialColour = slider.findColour(juce::Slider::thumbColourId);
    auto knobFillColour = slider.findColour(juce::Slider::backgroundColourId);
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat()
	                 .withTrimmedLeft(16.0f).withTrimmedRight(16.0f)
			 .withTrimmedTop(0.0f).withTrimmedBottom(8.0f);

    auto radius = bounds.getWidth() / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = 1.0f;
    auto arcRadius = radius - lineW/ 2.0f;

    auto arg = toAngle - juce::MathConstants<float>::halfPi;
    auto dialW = 6.0f;
    auto dialRadius = arcRadius - 12.0f; 

    auto center = bounds.getCentre();
    auto strokeType = juce::PathStrokeType(lineW, juce::PathStrokeType::beveled, juce::PathStrokeType::butt);
    juce::Path knobBackgroundPath;
    juce::Path knobForegroundPath;
    auto foregroundProportion = bounds.getWidth() * 0.95f;
    knobBackgroundPath.addEllipse(juce::Rectangle<float>(bounds.getWidth(), bounds.getWidth()).withCentre(center));
    knobForegroundPath.addEllipse(juce::Rectangle<float>(foregroundProportion, foregroundProportion).withCentre(center));
    if (slider.isEnabled()) {
        juce::Path backgroundArc;
	g.setOpacity(0.1f);
        backgroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

        g.setColour(outlineColor);
        //g.strokePath(backgroundArc, strokeType);
        g.setColour(juce::Colours::black);
	// Draw Knob Background
	g.fillPath(knobBackgroundPath);
    }
    juce::Point<float> thumbPoint(center.x + dialRadius * std::cos(arg),
	                          center.y + dialRadius * std::sin(arg));
    g.setColour(dialColour);
    //Center nub
    g.fillEllipse(juce::Rectangle<float>(dialW, dialW).withCentre(center));
    //Line to Edge
    g.drawLine(center.x, center.y, thumbPoint.x, thumbPoint.y, dialW);
    //Outer nub
    g.fillEllipse(juce::Rectangle<float>(dialW, dialW).withCentre(thumbPoint));
   
}

juce::Slider::SliderLayout BrewsLookAndFeel::getSliderLayout(Slider& slider) {
    auto bounds = slider.getLocalBounds().toFloat();
    juce::Slider::SliderLayout layout;
    auto textBounds = bounds.withSizeKeepingCentre(50, 25).translated(0, 25);
    layout.textBoxBounds = textBounds.toNearestInt();
    layout.sliderBounds = slider.getLocalBounds();
    return layout;
}

juce::Font BrewsLookAndFeel::getLabelFont(Label& label) {
    juce::Typeface::Ptr someFont = juce::Typeface::createSystemTypefaceFor(BinaryData::RalewayVariableFont_wght_ttf, BinaryData::RalewayVariableFont_wght_ttfSize);
   return someFont; 
}

void BrewsLookAndFeel::drawButtonBackground (Graphics& g,
                                           Button& button,
                                           const Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto cornerSize = 3.0f;
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                      .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour (baseColour);

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! (flatOnLeft  || flatOnTop),
                                  ! (flatOnRight || flatOnTop),
                                  ! (flatOnLeft  || flatOnBottom),
                                  ! (flatOnRight || flatOnBottom));

        g.fillPath (path);

        g.setColour (button.findColour (ComboBox::outlineColourId));
        g.strokePath (path, PathStrokeType (1.0f));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        g.setColour (button.findColour (ComboBox::outlineColourId));
        g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
    }
}
