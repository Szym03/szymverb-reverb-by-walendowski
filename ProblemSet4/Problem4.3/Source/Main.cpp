/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>

// Define constant values as global variables for ease of access
const float kSampleRate = 44100.0f;
const float kSec = 1.0f;
const float kNumFrames = kSampleRate * kSec;
const int kNumChannels = 2;
const float kFrequency = 440.0f;
const float kAmplitude = 0.25f;

//==============================================================================
int main (int argc, char* argv[])
{
    
    // Create a File object from the path
    juce::File destFile("/Users/Szymon/Documents/GitHub/EP-353_ProblemSets/ProblemSet4/Problem4.3/tremolo.wav");
    
    // Create an audio format manager and register the wav format
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    // Create a buffer to hold the samples
    juce::AudioBuffer<float> buffer(kNumChannels, kNumFrames);
    
    //Compute sine wave
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float *channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Compute A sin(2πft+φ). Phase is ignored in this case.
            // kFrequency/kSampleRate converts frequency (continous) to sample rate representation (discrete).
            float y = (2.0 * kAmplitude * std::sin(M_PI * (kFrequency/kSampleRate) * sample))*(std::sin(M_PI * (2.0/kSampleRate) * sample));
            channelData[sample] = y;
        }
    }
    
    // Create a unique_ptr for the writer
    std::unique_ptr<juce::AudioFormatWriter> writer(formatManager.findFormatForFileExtension("wav")
                                          ->createWriterFor(new juce::FileOutputStream(destFile),
                                                             kSampleRate,
                                                             kNumChannels,
                                                             16,
                                                             {},
                                                             0));

    if (writer)
    {
       // Write the mixed samples to the output file
       writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
    else
    {
       std::cout << "Failed to create a writer for the mixed file." << std::endl;
    }
}
