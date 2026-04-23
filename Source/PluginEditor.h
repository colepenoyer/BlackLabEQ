/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct CustomRtrSlider : juce::Slider
{
  CustomRtrSlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalDrag,juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        
    }
};

//==============================================================================
/**
*/
class BlackLabEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BlackLabEQAudioProcessorEditor (BlackLabEQAudioProcessor&);
    ~BlackLabEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::vector<juce::Component*> getComps();

        BlackLabEQAudioProcessor& audioProcessor;

        CustomRtrSlider lowCutFreqSlider;
        CustomRtrSlider peakFreqSlider;
        CustomRtrSlider peakGainSlider;
        CustomRtrSlider peakQualitySlider;
        CustomRtrSlider highCutFreqSlider;

        juce::Label lowCutLabel;
        juce::Label peakFreqLabel;
        juce::Label peakGainLabel;
        juce::Label peakQLabel;
        juce::Label highCutLabel;

        using APVTS = juce::AudioProcessorValueTreeState;
        using Attachment = APVTS::SliderAttachment;

        Attachment lowCutFreqAttachment;
        Attachment peakFreqAttachment;
        Attachment peakGainAttachment;
        Attachment peakQualityAttachment;
        Attachment highCutFreqAttachment;


    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlackLabEQAudioProcessorEditor)
};
