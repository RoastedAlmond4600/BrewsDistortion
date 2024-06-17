#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
BrewsDistortionAudioProcessorEditor::BrewsDistortionAudioProcessorEditor (BrewsDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    juce::LookAndFeel::setDefaultLookAndFeel(&brewsLNF);
    //Knobs
    driveKnob.label = "Drive";
    addAndMakeVisible(driveKnob);
    outputKnob.label = "Output";
    addAndMakeVisible(outputKnob);
    mixKnob.label = "Mix";
    addAndMakeVisible(mixKnob);
    auto* parameter = processorRef.apvts.getParameter("model");
    modelBox.addItemList(parameter->getAllValueStrings(), 1);
    modelBox.addListener(this); 

    std::atomic<float>* rawModelParam = processorRef.apvts.getRawParameterValue(ParameterID::model.getParamID());
    int rawModelParamValue = static_cast<int>(rawModelParam->load());
    if (rawModelParamValue == 0) updateToggleState(hardButton, modelBox, 0);
    if (rawModelParamValue == 1) updateToggleState(softButton, modelBox, 1);
    if (rawModelParamValue == 2) updateToggleState(saturateButton, modelBox, 2);

    //TODO: Get current model and toggle corresponding button if NOT already toggled.
    // Perhaps if we made the radiogroup a listener, and let the combo box broadcast 
    //Button Lambdas.
    auto modelLambda = [this](int index, juce::Button& button) {
        updateToggleState(button, modelBox, index);
    };
    
    //Buttons

    //Hard Button
    hardButton.setButtonText("Hard");
    hardButton.onClick = [this, modelLambda]() { modelLambda(0, hardButton); };
    hardButton.setRadioGroupId(1);
    addAndMakeVisible(hardButton);
    //Soft Button 
    softButton.setButtonText("Soft");
    softButton.onClick = [this, modelLambda]() { modelLambda(1, softButton); };
    softButton.setRadioGroupId(1);
    addAndMakeVisible(softButton);
    //Saturate Button
    saturateButton.setButtonText("Saturate");
    saturateButton.onClick = [this, modelLambda]() { modelLambda(2, saturateButton); };
    saturateButton.setRadioGroupId(1);
    addAndMakeVisible(saturateButton);
    setSize (750, 450);

}

BrewsDistortionAudioProcessorEditor::~BrewsDistortionAudioProcessorEditor()
{
    modelBox.removeListener(this);
}

//==============================================================================
void BrewsDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //Banner
    juce::Rectangle<float> bannerRect {20.0f, 30.0f, 140.0f, 400.0f};
    //Banner Text
    auto bannerImage = juce::ImageCache::getFromMemory(BinaryData::Banner_png, BinaryData::Banner_pngSize);
    g.drawImageWithin(bannerImage, bannerRect.getX()-5, bannerRect.getY()+20, bannerRect.getWidth(), bannerRect.getHeight(),  juce::RectanglePlacement::centred);
}

void BrewsDistortionAudioProcessorEditor::resized()
{
    auto localBounds = getLocalBounds();
    auto margin = 20;
    auto secSize = 100;
    auto buttonWidth = 80;
    auto buttonHeight = 30;
    auto secMult = 1;
    // Drive Knob Position
    juce::Rectangle driveBounds (localBounds.getWidth() / 2, localBounds.getHeight() / 2, localBounds.getWidth() / 2, localBounds.getWidth() / 2);
    driveBounds.setCentre(localBounds.getWidth() / 2, localBounds.getHeight() / 2);
    driveKnob.setBounds(driveBounds);
    //Output Knob Position
    juce::Rectangle outputBounds (driveBounds.getRight() +  margin, driveBounds.getY(), secSize, secSize);
    outputKnob.setBounds(outputBounds);
    //Mix Knob Position
    juce::Rectangle mixBounds (driveBounds.getRight() + margin, driveBounds.getY() + secSize * secMult++, secSize, secSize);
    mixKnob.setBounds(mixBounds); 
    auto secBoundsCentre = mixBounds.getRight() - secSize / 2;
    //Hard Button Position
    juce::Rectangle hardBounds (driveBounds.getRight() + margin, mixBounds.getBottom() + buttonHeight, buttonWidth, buttonHeight); 
    hardBounds.setCentre(secBoundsCentre, hardBounds.getY());
    hardButton.setBounds(hardBounds);
    //Soft Button Position
    juce::Rectangle softBounds (driveBounds.getRight() + margin, hardBounds.getBottom() + buttonHeight, buttonWidth, buttonHeight); 
    softBounds.setCentre(secBoundsCentre, softBounds.getY());
    softButton.setBounds(softBounds);
    //Saturate Button Position
    juce::Rectangle saturateBounds (driveBounds.getRight() + margin, softBounds.getBottom() + buttonHeight, buttonWidth, buttonHeight); 
    saturateBounds.setCentre(secBoundsCentre, saturateBounds.getY());
    saturateButton.setBounds(saturateBounds);
}

void BrewsDistortionAudioProcessorEditor::updateToggleState(juce::Button& button, juce::ComboBox& box, int index) {
    box.setSelectedItemIndex(index);
    button.setToggleState(true, juce::NotificationType::dontSendNotification);
}

void BrewsDistortionAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    //Get the value of the model
    if (comboBoxThatHasChanged == &modelBox) {
        currentModelId.store(comboBoxThatHasChanged->getSelectedId());
        if (currentModelId.load() == 1) hardButton.triggerClick();
        if (currentModelId.load() == 2) softButton.triggerClick();
        if (currentModelId.load() == 3) saturateButton.triggerClick();
        std::printf("%d\n", currentModelId.load());
    }
}
