#pragma once

namespace tremolo {
struct Parameters {
  explicit Parameters(juce::AudioProcessor&); //explicity accepts 1 argument

  juce::AudioParameterFloat& rate;

  JUCE_DECLARE_NON_COPYABLE(Parameters);
  JUCE_DECLARE_NON_MOVEABLE(Parameters);
};
}  // namespace tremolo
