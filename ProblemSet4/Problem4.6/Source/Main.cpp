/*
  ==============================================================================

    The template DAFx file.

  ==============================================================================
*/

// This gets generated dynamically by CMake. VSCode will complain it can't
// find it until you generate the CMake build directory
#include <JuceHeader.h>

// Function prototypes
int readSoundFile (juce::AudioBuffer<float>* buffer);
int writeSoundFile (juce::AudioBuffer<float> buffer, float sampleRate);
void process (juce::AudioBuffer<float> inBuffer, juce::AudioBuffer<float> *outBuffer);
short keep_bits_from_16( short input, int keepBits );

//==============================================================================
int main ()
{   

    
    // Read the source file into a buffer for processing
    juce::AudioBuffer<float> inBuffer;
    float sampleRate = readSoundFile (&inBuffer);
    if (!sampleRate) return 1;

    // Create a buffer for processing audio data
    juce::AudioBuffer<float> outBuffer (inBuffer.getNumChannels(), inBuffer.getNumSamples());

    // Process the audio data
    process (inBuffer, &outBuffer);

    // Write the processed audio data to a sound file
    if (writeSoundFile (outBuffer, sampleRate)) return 1;
}

// TODO: Modify this function for your digital audio effect
void process (juce::AudioBuffer<float> inBuffer, juce::AudioBuffer<float> *outBuffer)
{   
    
    int Numsamps = inBuffer.getNumSamples();

    for (int sample = 0; sample < inBuffer.getNumSamples(); ++sample)
    {
        for (int channel = 0; channel < inBuffer.getNumChannels(); ++channel)
        {
            
            const float* channelReadData = inBuffer.getReadPointer (channel);
            float* channelWriteData = outBuffer->getWritePointer (channel);
            
            
            short sampleShort = static_cast<short>(channelReadData[sample] * 32767.0f);
            
            
            short reducedShort = keep_bits_from_16(sampleShort, 4);
            
            
            channelWriteData[sample] = static_cast<float>(reducedShort) / 32767.0f;
            
        }
    }
}

int readSoundFile (juce::AudioBuffer<float> *buffer)
{
    
    juce::File file("/Users/Szymon/Documents/GitHub/EP-353_ProblemSets/ProblemSet4/Problem4.6/sine.wav");

    // Create an audio format manager and register basic formats (especially WAV)
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    // Create a unique_ptr for the reader
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    // Check if the reader was created
    if (reader)
    {
        // Set the size of buffer
        buffer->setSize(reader->numChannels, static_cast<int>(reader->lengthInSamples));

        // Read the samples from the first file into the buffer
        bool success = reader->read(buffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);

        // Return the sample rate for this file if reading was successful
        if (success) return reader->sampleRate;

        std::cout << "Failed to copy samples into the buffer from the source wave file." << std::endl;
        return 0;
    }

    std::cout << "Failed to read the source wave file." << std::endl;
    return 0;
}

int writeSoundFile (juce::AudioBuffer<float> buffer, float sampleRate)
{
    
    juce::File file("/Users/Szymon/Documents/GitHub/EP-353_ProblemSets/ProblemSet4/Problem4.6/reduct.aiff");

    // Create an audio format manager and register basic formats (especially WAV)
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    // Create a unique_ptr for the writer (WAV format)
    std::unique_ptr<juce::AudioFormatWriter> writer(formatManager.findFormatForFileExtension("aiff")
                                                    ->createWriterFor(new juce::FileOutputStream(file),
                                                                       sampleRate,
                                                                       buffer.getNumChannels(),
                                                                       16,
                                                                       {},
                                                                       0));
    // Check if the writer was created
    if (writer)
    {
        // Write the mixed samples to the output file
        bool success = writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        if (success) return 0;

        std::cout << "Failed to write samples into the buffer to the destination wave file." << std::endl;
        return 1;
    }

    std::cout << "Failed to create a writer for the destination file." << std::endl;
    return 1;
}

short keep_bits_from_16( short input, int keepBits ) {
    return (input & (-1 << (16-keepBits)));
  }