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
    CustomRtrSlider()
        : juce::Slider(juce::Slider::RotaryHorizontalDrag,
                       juce::Slider::NoTextBox)
    {
    }
};

class BlackLabEQAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        private juce::Timer
{
public:
    BlackLabEQAudioProcessorEditor (BlackLabEQAudioProcessor&);
    ~BlackLabEQAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    std::vector<juce::Component*> getComps();

    juce::Rectangle<int> getRenderArea();
    juce::Rectangle<int> getAnalysisArea();

    void drawBackground(juce::Graphics& g);
    void drawResponseCurve(juce::Graphics& g, const juce::Rectangle<int>& responseArea);

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlackLabEQAudioProcessorEditor)
};

