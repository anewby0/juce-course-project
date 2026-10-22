
namespace tremolo {
  namespace {
    juce::AudioParameterFloat& createModulationRateParameter(juce::AudioProcessor& processor) {
      constexpr auto versionHint = 1; //what is constexpr
      auto parameter = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"modulation.rate", versionHint},
        "Modulation rate",
        juce::NormalisableRange<float>{0.1f, 20.f, 0.01f, 0.4f},
        5.f,
      juce::AudioParameterFloatAttributes{}.withLabel("Hz") );
      auto& parameterReference = *parameter;
      processor.addParameter(parameter.release());
      return parameterReference;
    }
  }
Parameters::Parameters(juce::AudioProcessor& processor)
// TODO: create parameters
// TODO: retrieve references to parameters
// TODO: add parameters to the processor
 : rate{createModulationRateParameter(processor)}
{
  juce::ignoreUnused(processor);
}
}  // namespace tremolo
