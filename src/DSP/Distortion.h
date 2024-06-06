#pragma once
#include <JuceHeader.h>
template <typename SampleType>
class Distortion {
public:
    Distortion();
    void prepare(juce::dsp::ProcessSpec& spec);
    void reset();
   
    template <typename ProcessContext>
    void process(const ProcessContext& context) {
        const auto& inputBlock = context.getInputBlock();
	auto& outputBlock = context.getOutputBlock();
	const auto numChannels = outputBlock.getNumChannels();
	const auto numSamples = outputBlock.getNumSamples();
	for (size_t channel = 0; channel < numChannels; ++channel) {
	    auto* inputSamples = inputBlock.getChannelPointer(channel);
	    auto* outputSamples = outputBlock.getChannelPointer(channel);
	    for (size_t sample = 0; sample < numSamples; ++sample) {
	        outputSamples[sample] = dcFilter.processSample(channel, processSample(channel, inputSamples[sample]));
	    }
	}
    }
    SampleType processSample(int channel, SampleType inputSample) noexcept {
	juce::ignoreUnused(channel);
        switch (_model) {
	    case DistortionModel::kSoft: {
	        return processSoftClip(inputSample);				 
	    }
	    case DistortionModel::kHard: {
	        return processHardClip(inputSample);				 
	    }
	    case DistortionModel::kSaturate: {
	        return processSaturate(inputSample);				 
	    }
	    default: return inputSample;
	}	
    }

    SampleType processHardClip(SampleType inputSample) {
	auto wetSignal = inputSample * _input.getNextValue();
	if (std::abs(wetSignal) > _ceiling.getNextValue()) {
	    wetSignal *= _ceiling.getNextValue() / std::abs(wetSignal);
	}
	auto mixedSignal = (1.0f - _mix.getNextValue()) * inputSample + _mix.getNextValue() * wetSignal;
        return mixedSignal * _output.getNextValue();
    }
    SampleType processSoftClip(SampleType inputSample){ 
	auto wetSignal = inputSample * _input.getNextValue();
	auto softSignal= TWO_OVER_PI * myAtanApprox(juce::dsp::FastMathApproximations::sin( wetSignal)); 
	softSignal = jlimit (-1.0f, 1.0f, (float)softSignal);
	auto mixedSignal = (1.0f - _mix.getNextValue()) * inputSample + _mix.getNextValue() * softSignal;
	return mixedSignal * _output.getNextValue();
    }
    SampleType processSaturate(SampleType inputSample) {
	auto wetSignal = std::abs(inputSample * _input.getNextValue());
        auto a = _ceiling.getNextValue();
	auto saturateSignal = a + (wetSignal - a) / (1 + std::pow(((wetSignal - a) / (1 - wetSignal)), 2));
	saturateSignal = jlimit(-1.0f, 1.0f, (float)saturateSignal);
	auto mixedSignal = (1.0f - _mix.getNextValue()) * inputSample + _mix.getNextValue() * saturateSignal;
	return mixedSignal * _output.getNextValue();
    }

    enum class DistortionModel {
        kHard,
	kSoft,
	kSaturate
    };

    void setMix(SampleType mixVal);
    void setInput(SampleType inputVal);
    void setOutput(SampleType outputVal);
    void setCeiling(SampleType ceilingVal);
    void setDistortionModel(int modelIndex);

    SampleType getMix();
    SampleType getInput();
    SampleType getOutput();

private:
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Distortion)
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _input;
    juce::SmoothedValue<float> _output;
    juce::SmoothedValue<float> _ceiling;
    float makeupFactor;
    int _sampleRate;
    DistortionModel _model = DistortionModel::kHard;
    juce::dsp::LinkwitzRileyFilter<float> dcFilter;
    const float TWO_OVER_PI = 0.636619772368f;
    const float PI_OVER_TWO = 1.57079632679f;
    inline float myAtanApprox(SampleType x) {
	auto x3 = x * x * x;
        return -0.2 * x3 + x;
    }
};
