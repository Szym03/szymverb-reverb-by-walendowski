/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h> 

//==============================================================================
/**
 */
class ReverbAudioProcessor : public juce::AudioProcessor
{
public:
  //==============================================================================
  ReverbAudioProcessor();
  ~ReverbAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  float gain;
  float level;

  juce::AudioProcessorValueTreeState &valueTreeState() noexcept
  {
    return *parameters;
  }
  bool isFrozen() const
{
    return parameters->getRawParameterValue("freeze")->load() > 0.5f;
}

private:
  // Comb filter parameters
  static constexpr int numCombs = 8;
  std::array<int, numCombs> combDelaysLeft{1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617};  // Left channel delays
  std::array<int, numCombs> combDelaysRight{1188, 1277, 1356, 1422, 1491, 1557, 1617, 1687}; // Right channel delays
                                                                                             // in samples
  float combFeedback = 0.84f;                                                                // feedback gain for each comb
  std::unique_ptr<juce::AudioProcessorValueTreeState> parameters;

  // Circular buffers per channel
  std::vector<juce::AudioBuffer<float>> combBuffers; // buffer for each comb
  std::vector<std::vector<int>> combWritePositions;  // writers for each comb

  // Maximum buffer length 
  int maxDelaySamples = 4410;

  //Comb filter for damping
  std::vector<std::vector<juce::dsp::IIR::Filter<float>>> combFilters;


  // Allpass filter settings
  static constexpr int numAllpasses = 4;
  std::array<int, numAllpasses> allpassDelays{556, 441, 341, 225}; 
  float allpassFeedback = 0.5f;

  std::vector<juce::AudioBuffer<float>> allpassBuffers;
  std::vector<std::vector<int>> allpassWritePositions;



  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbAudioProcessor)
};
