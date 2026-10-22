#pragma once

namespace tremolo {
class Tremolo {
public:
  // ?? what is this for
  enum class LfoWaveform : size_t { //what is size_t type?
    sine = 0,
    triangle = 1,
  };

  Tremolo() {
    for (auto& lfo : lfos) { //iterating through references
      lfo.setFrequency(5.f /* Hz */, true); //2nd arg allows instantly changing frequency instead of smoothing
    }
  }

  void prepare(double sampleRate, int expectedMaxFramesPerBlock) {
    //juce::ignoreUnused(sampleRate, expectedMaxFramesPerBlock); // since its not unused anymore dont need this

    const juce::dsp::ProcessSpec processSpec {
      .sampleRate = sampleRate,
      .maximumBlockSize = static_cast<juce::uint32>(expectedMaxFramesPerBlock),
      .numChannels = 1u,
    };

    for (auto& lfo : lfos) {
      lfo.prepare(processSpec); //prepare
    }
  }

  void setLfoWaveform(LfoWaveform waveform) {
    jassert(waveform == LfoWaveform::sine || waveform == LfoWaveform::triangle);
    lfoToSet = waveform;
  }

  void setModulationRate(float rateHz) {
    for (auto& lfo : lfos) {
      lfo.setFrequency(rateHz);
    }
  }

  void process(juce::AudioBuffer<float>& buffer) noexcept {
    updateLfoWaveform();
    
    // this is frame-wise processing because we iterate over frames first
    for (const auto frameIndex : std::views::iota(0, buffer.getNumSamples())) {
    
      //const auto lfoValue = lfo.processSample(0.f); // generate the LFO value, 0 bc processSample adds its output to whatever we pass in, so this just 'get's the generated value
      const auto lfoValue = lfos[juce::toUnderlyingType(currentLfo)].processSample(0.f);

      //calculate the modulation value
      constexpr auto modulationDepth = 0.4f; //constexpr: may be evaluated at compile time, allow named/typed numerical constants, no performance penalty
      const auto modulationValue = modulationDepth * lfoValue +1.f;
      
      // for each channel sample in the frame
      for (const auto channelIndex :
           std::views::iota(0, buffer.getNumChannels())) {
        // get the input sample
        const auto inputSample = buffer.getSample(channelIndex, frameIndex);

        // modulate the sample
        //const auto outputSample = inputSample;
        const auto outputSample = inputSample * modulationValue;

        // set the output sample
        buffer.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept {
    for (auto& lfo : lfos) {
      lfo.reset(); //sets phase back to beginning
    }
  }

private:
  // You should put class members and private functions here
  /*
  juce::dsp::Oscillator<float> lfo{ [](auto phase){ return std::sin(phase); } // lambda that returns sine wave
    [](float x){ return x / juce::MathConstants<float>::pi; } //saw tooth
    //[]()
  };
  */
  static float triangle(float phase) { //static so not dependent on a temelo class instance
    const auto ft = phase / juce::MathConstants<float>::twoPi;
    return 4.f * std::abs(ft - std::floor(ft + 0.5f)) - 1.f;
  }

  //
  float getNextLfoValue() {
    return lfos[juce::toUnderlyingType(currentLfo)].processSample(0.f); //what is this casting ;-;
  }

  void updateLfoWaveform() {
    if (currentLfo != lfoToSet) {
      currentLfo = lfoToSet;
    }
  }

  std::array<juce::dsp::Oscillator<float>, 2u> lfos{
    juce::dsp::Oscillator<float>{ [](auto phase){ return std::sin(phase); }},
    juce::dsp::Oscillator<float>{triangle}
  };

  LfoWaveform currentLfo = LfoWaveform::triangle;
  LfoWaveform lfoToSet = currentLfo;
};
}  // namespace tremolo
