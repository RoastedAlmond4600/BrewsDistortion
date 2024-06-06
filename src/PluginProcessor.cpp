#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
BrewsDistortionAudioProcessor::BrewsDistortionAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    castParameter(apvts, ParameterID::input, inputParam);
    castParameter(apvts, ParameterID::output, outputParam);
    castParameter(apvts, ParameterID::mix, mixParam);
    castParameter(apvts, ParameterID::ceiling, ceilingParam);
    castParameter(apvts, ParameterID::model, modelParam);
    apvts.state.addListener(this);
}

BrewsDistortionAudioProcessor::~BrewsDistortionAudioProcessor()
{
    apvts.state.removeListener(this);
}

//==============================================================================
const juce::String BrewsDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BrewsDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BrewsDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BrewsDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BrewsDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BrewsDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BrewsDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BrewsDistortionAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String BrewsDistortionAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void BrewsDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void BrewsDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    distModule.prepare(spec);
}

void BrewsDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool BrewsDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void BrewsDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bool expected = true;
    if (isNonRealtime() || parameterChanged.compare_exchange_strong(expected, false)) {
        update();
    }
    juce::dsp::AudioBlock<float> block {buffer};
    distModule.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void BrewsDistortionAudioProcessor::update() {
    distModule.setInput(juce::Decibels::decibelsToGain(inputParam->get()));
    distModule.setOutput(juce::Decibels::decibelsToGain(outputParam->get()));
    float ceilingVal =  0.99f / ceilingParam->get();
    distModule.setCeiling(ceilingVal);
    distModule.setMix(mixParam->get() / 100.0f);
    distModule.setDistortionModel(modelParam->getIndex());
}

//==============================================================================
bool BrewsDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BrewsDistortionAudioProcessor::createEditor()
{
    return new BrewsDistortionAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void BrewsDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void BrewsDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessorValueTreeState::ParameterLayout BrewsDistortionAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterChoice>(
 			       ParameterID::model,
 			       "Model",
        		       juce::StringArray {"Hard", "Soft", "Saturate"},
 			       0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
			        ParameterID::input,
				"Drive",
				juce::NormalisableRange(0.0f, 48.0f, 0.1f),
				18.0f,
				juce::AudioParameterFloatAttributes().withLabel("+dB")));
    layout.add(std::make_unique<juce::AudioParameterFloat> (
			        ParameterID::output,
				"Output",
				juce::NormalisableRange(-36.0f, 6.0f, 0.1f),
				-6.0f,
				juce::AudioParameterFloatAttributes().withLabel("dB")));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
			        ParameterID::ceiling,
				"Punishment",
				juce::NormalisableRange(1.0f, 99.0f, 0.01f),
				1.0f,
				juce::AudioParameterFloatAttributes().withLabel("%")));
    layout.add(std::make_unique<juce::AudioParameterFloat> (
			        ParameterID::mix,
				"Mix",
				juce::NormalisableRange(0.0f, 100.0f, 1.0f),
				80.0f,
				juce::AudioParameterFloatAttributes().withLabel("%")));
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BrewsDistortionAudioProcessor();
}
