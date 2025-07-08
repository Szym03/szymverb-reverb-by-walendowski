#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor(ReverbAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);

    // –– DRY slider
    drySlider.setSliderStyle(juce::Slider::LinearVertical);
    drySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(drySlider);
    dryLabel.setText("Dry", juce::dontSendNotification);
    dryLabel.setFont (juce::FontOptions { "Galvji", 15.0f,juce::Font::bold});
    addAndMakeVisible(dryLabel);
    dryAttachment = std::make_unique<Attachment>(
        audioProcessor.valueTreeState(), "dryMix", drySlider);

    // –– WET slider
    wetSlider.setSliderStyle(juce::Slider::LinearVertical);
    wetSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(wetSlider);
    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.setFont (juce::FontOptions { "Galvji", 15.0f,juce::Font::bold});
    addAndMakeVisible(wetLabel);
    wetAttachment = std::make_unique<Attachment>(
        audioProcessor.valueTreeState(), "wetMix", wetSlider);

    // –– FEEDBACK slider
    feedbackSlider.setSliderStyle(juce::Slider::LinearVertical);
    feedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(feedbackSlider);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.setFont (juce::FontOptions { "Galvji", 15.0f,juce::Font::bold});
    addAndMakeVisible(feedbackLabel);
    feedbackAttachment = std::make_unique<Attachment>(
        audioProcessor.valueTreeState(), "feedback", feedbackSlider);

    // –– DAMP slider
    dampSlider.setSliderStyle(juce::Slider::LinearVertical);
    dampSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(dampSlider);
    dampLabel.setText("Damping", juce::dontSendNotification);
    dampLabel.setFont (juce::FontOptions { "Galvji", 15.0f, juce::Font::bold });
    addAndMakeVisible(dampLabel);
    dampAttachment = std::make_unique<Attachment>(
        audioProcessor.valueTreeState(), "damp", dampSlider);

    //nicer colors
    drySlider.setColour(juce::Slider::thumbColourId, juce::Colours::whitesmoke);
    wetSlider.setColour(juce::Slider::thumbColourId, juce::Colours::whitesmoke);
    feedbackSlider.setColour(juce::Slider::thumbColourId, juce::Colours::whitesmoke);
    dampSlider.setColour(juce::Slider::thumbColourId, juce::Colours::whitesmoke);

    // –– FREEZE button
    freezeButton.setButtonText("Freeze");
    freezeButton.setClickingTogglesState(true); // already true by default, but explicit
    freezeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkblue);
    freezeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    freezeButton.setColour(juce::TextButton::textColourOnId, juce::Colours::cornflowerblue);
    freezeButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::whitesmoke);
    ;                                // text on the button
    addAndMakeVisible(freezeButton); // show it
    freezeAttachment = std::make_unique<Attachmentb>(
        audioProcessor.valueTreeState(), "freeze", freezeButton);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
}

void ReverbAudioProcessorEditor::paint(juce::Graphics &g)
{
    //background and text clrs
    g.fillAll(juce::Colours::cornflowerblue); 
    g.setColour(juce::Colours::whitesmoke);

    //nice font title (would have to embed if i released this)
    g.setFont (juce::FontOptions ("Pilowlava",40.0f,juce::Font::bold));
    g.setColour (juce::Colours::white);
    g.drawText ("SzymVerb",
                getLocalBounds().removeFromTop (60),
                juce::Justification::centred,
                true);

}

void ReverbAudioProcessorEditor::resized()
{
    //layout of everything
    const int sliderWidth = 60;
    const int sliderHeight = 160;
    const int labelHeight = 20;
    const int spacing = 20;
    const int topMargin = 60;

    // Center the four sliders
    const int totalWidth = 4 * sliderWidth + 3 * spacing;
    int startX = (getWidth() - totalWidth) / 2;
    int sliderY = topMargin;
    int labelY = sliderY + sliderHeight + 5;

    drySlider.setBounds(startX, sliderY, sliderWidth, sliderHeight);
    dryLabel.setBounds(startX, labelY, sliderWidth, labelHeight);
    startX += sliderWidth + spacing;

    wetSlider.setBounds(startX, sliderY, sliderWidth, sliderHeight);
    wetLabel.setBounds(startX, labelY, sliderWidth, labelHeight);
    startX += sliderWidth + spacing;

    feedbackSlider.setBounds(startX, sliderY, sliderWidth, sliderHeight);
    feedbackLabel.setBounds(startX, labelY, sliderWidth, labelHeight);
    startX += sliderWidth + spacing;

    dampSlider.setBounds(startX, sliderY, sliderWidth, sliderHeight);
    dampLabel.setBounds(startX, labelY, sliderWidth, labelHeight);

    
    const int buttonW = 80;
    const int buttonH = 24;
    const int buttonY = labelY + labelHeight + 10;  
    const int centerX = (getWidth() - buttonW) / 2; 

    freezeButton.setBounds(centerX, buttonY, buttonW, buttonH);
}
