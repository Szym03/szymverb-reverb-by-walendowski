#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbAudioProcessor::ReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
    parameters.reset(new juce::AudioProcessorValueTreeState(
        *this, nullptr, "PARAMETERS",///had to do this hear not in header because it was crashing
        {std::make_unique<juce::AudioParameterFloat>("dryMix", "Dry Mix", 0.0f, 1.0f, 0.8f),
         std::make_unique<juce::AudioParameterFloat>("wetMix", "Wet Mix", 0.0f, 1.0f, 0.3f),
         std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.0f, 0.99f, 0.84f),
         std::make_unique<juce::AudioParameterFloat>("damp", "Damp", 0.0f, 0.99f, 0.6f),
         std::make_unique<juce::AudioParameterBool>("freeze", "Freeze", false)

        }));
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
}

//==============================================================================
const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ReverbAudioProcessor::getProgramName(int index)
{
    return {};
}

void ReverbAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void ReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    maxDelaySamples = static_cast<int>(sampleRate);

    int numCh = getTotalNumInputChannels();
    combBuffers.resize(numCh);
    combWritePositions.resize(numCh);
    combFilters.clear();

    juce::dsp::ProcessSpec spec{sampleRate,
                                static_cast<uint32_t>(samplesPerBlock),
                                1};
    
    //get buffers and writers ready for combs
    for (int ch = 0; ch < numCh; ++ch)
    {
        combBuffers[ch].setSize(numCombs, maxDelaySamples);
        combBuffers[ch].clear();
        combWritePositions[ch].assign(numCombs, 0);

        combFilters.emplace_back(numCombs);
        for (auto &f : combFilters.back())
        {
            f.prepare(spec);
            f.reset();
        }
    }
    //get buffers and writers ready for allpass
    allpassBuffers.clear();
    allpassWritePositions.clear();
    int numOut = getTotalNumOutputChannels();
    for (int ch = 0; ch < numOut; ++ch)
    {
        juce::AudioBuffer<float> buf;
        buf.setSize(numAllpasses, maxDelaySamples);
        buf.clear();
        allpassBuffers.push_back(buf);
        allpassWritePositions.emplace_back(numAllpasses, 0);
    }
}

void ReverbAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void ReverbAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &)
{
    const int numCh = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    const bool frozen = isFrozen();
    const float rawFeedback = parameters->getRawParameterValue("feedback")->load();
    const float combFeedback = frozen ? 1.0f : rawFeedback;//if frozen then feedback is 1

    const float dryMix = parameters->getRawParameterValue("dryMix")->load();
    const float wetMix = parameters->getRawParameterValue("wetMix")->load();
    const float damp = parameters->getRawParameterValue("damp")->load();

    auto cutoffHz = juce::jmap(damp, 2000.0f, 20000.0f);
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), cutoffHz);

    for (int ch = 0; ch < numCh; ++ch)
        for (int k = 0; k < numCombs; ++k)
            combFilters[ch][k].coefficients = coeffs;

    for (int ch = 0; ch < numCh; ++ch)
    {
        float *channelData = buffer.getWritePointer(ch);
        auto &buf = combBuffers[ch];
        auto &positions = combWritePositions[ch];
        auto &apBuf = allpassBuffers[ch];
        auto &apPositions = allpassWritePositions[ch];

        for (int i = 0; i < numSamples; ++i)
        {
            //frozen logic
            float rawIn = channelData[i];
            float input = frozen ? 0.0f : rawIn;//if not frozen then use raw as input, otherwise we bypass and only feedback through the freezed buffer
            float sum = 0.0f;
            //reverb is based on this(except freeze): https://ccrma.stanford.edu/~jos/pasp/Freeverb.html
            // comb filters
            for (int k = 0; k < numCombs; ++k)
            {
                float *combLine = buf.getWritePointer(k);
                int writeIdx = positions[k];
                int delay = (ch == 0 ? combDelaysLeft[k] : combDelaysRight[k]);
                int readIdx = writeIdx - delay;
                if (readIdx < 0)
                    readIdx += maxDelaySamples;

                float delayed = combLine[readIdx];
                delayed = combFilters[ch][k].processSample(delayed);//processing with lowpass

                float y = delayed * combFeedback;
                combLine[writeIdx] = input + y;
                sum += y;

                if (++writeIdx >= maxDelaySamples)
                    writeIdx = 0;
                positions[k] = writeIdx;
            }

            // allpass filters
            for (int ap = 0; ap < numAllpasses; ++ap)
            {
                float *apLine = apBuf.getWritePointer(ap);
                int writeAp = apPositions[ap];
                int delayAp = allpassDelays[ap];
                int readAp = writeAp - delayAp;
                if (readAp < 0)
                    readAp += maxDelaySamples;

                float delayed = apLine[readAp];
                float y = -allpassFeedback * sum + delayed;
                apLine[writeAp] = sum + allpassFeedback * delayed;
                sum = y;//formula based on https://peabody.sapp.org/class/350.867/lab/prog6/#:~:text=The%20goal%20of%20this%20lab,This%20is%20called%20dispersion.

                if (++writeAp >= maxDelaySamples)
                    writeAp = 0;
                apPositions[ap] = writeAp;
            }

            channelData[i] = dryMix * rawIn + wetMix * sum * 0.5f;
        }
    }
}

bool ReverbAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor *ReverbAudioProcessor::createEditor() { return new ReverbAudioProcessorEditor(*this); }

void ReverbAudioProcessor::getStateInformation(juce::MemoryBlock &) {}
void ReverbAudioProcessor::setStateInformation(const void *, int) {}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new ReverbAudioProcessor(); }
