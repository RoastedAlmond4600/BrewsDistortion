#include "Distortion.h"
template <typename SampleType>
Distortion<SampleType>::Distortion() {

}

template <typename SampleType>
void Distortion<SampleType>::prepare(juce::dsp::ProcessSpec& spec) {
    _sampleRate = spec.sampleRate;
    dcFilter.prepare(spec);
    dcFilter.setCutoffFrequency(10.0f);
    dcFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    reset();
}

template <typename SampleType>
void Distortion<SampleType>::reset() {
    if (_sampleRate > 0) {
        _mix.reset(_sampleRate, 0.3f);
	_input.reset(_sampleRate, 0.3f);
	_output.reset(_sampleRate, 0.3f);
	_ceiling.reset(_sampleRate, 0.3f);
    }
}

//Setters
template <typename SampleType>
void Distortion<SampleType>::setMix(SampleType mixVal) {
    _mix.setTargetValue(mixVal);
}

template <typename SampleType>
void Distortion<SampleType>::setInput(SampleType inputVal) {
    _input.setTargetValue(inputVal);
}

template <typename SampleType>
void Distortion<SampleType>::setOutput(SampleType outputVal) {
    _output.setTargetValue(outputVal);
}

template <typename SampleType>
void Distortion<SampleType>::setCeiling(SampleType ceilingVal) {
    _ceiling.setTargetValue(ceilingVal);
}
template <typename SampleType>
void Distortion<SampleType>::setDistortionModel(int modelIndex) {
    _model = static_cast<DistortionModel>(modelIndex);
}

//Getters
template <typename SampleType>
SampleType Distortion<SampleType>::getMix() {
    return _mix.getNextValue();
}

template <typename SampleType> SampleType Distortion<SampleType>::getInput() {
    return _input.getNextValue();
}

template <typename SampleType>
SampleType Distortion<SampleType>::getOutput() {
    return _output.getNextValue();
}


template class Distortion<float>;
template class Distortion<double>;
