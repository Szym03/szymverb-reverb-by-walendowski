/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ReverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
  ReverbAudioProcessorEditor(ReverbAudioProcessor &);
  ~ReverbAudioProcessorEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  ReverbAudioProcessor &audioProcessor;

  juce::Slider drySlider, wetSlider, feedbackSlider, dampSlider;
  juce::Label dryLabel, wetLabel, feedbackLabel, dampLabel;

  // a toggle button for Freeze
  juce::TextButton freezeButton;

  using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  std::unique_ptr<Attachment> dryAttachment, wetAttachment, feedbackAttachment, dampAttachment;
  using Attachmentb = juce::AudioProcessorValueTreeState::ButtonAttachment;
  std::unique_ptr<Attachmentb> freezeAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbAudioProcessorEditor)
};
