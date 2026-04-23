/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlackLabEQAudioProcessorEditor::BlackLabEQAudioProcessorEditor (BlackLabEQAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      lowCutFreqAttachment(audioProcessor.apvts, "LowCutFreq", lowCutFreqSlider),
      peakFreqAttachment(audioProcessor.apvts, "PeakFreq", peakFreqSlider),
      peakGainAttachment(audioProcessor.apvts, "PeakGain", peakGainSlider),
      peakQualityAttachment(audioProcessor.apvts, "PeakQuality", peakQualitySlider),
      highCutFreqAttachment(audioProcessor.apvts, "HighCutFreq", highCutFreqSlider)
{
    for (auto* comp : getComps())
    {
        addAndMakeVisible(comp);
    }

    lowCutLabel.setText("LOW CUT", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    lowCutLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    lowCutLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(lowCutLabel);

    peakFreqLabel.setText("FREQ", juce::dontSendNotification);
    peakFreqLabel.setJustificationType(juce::Justification::centred);
    peakFreqLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    peakFreqLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(peakFreqLabel);

    peakGainLabel.setText("GAIN", juce::dontSendNotification);
    peakGainLabel.setJustificationType(juce::Justification::centred);
    peakGainLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    peakGainLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(peakGainLabel);

    peakQLabel.setText("Q", juce::dontSendNotification);
    peakQLabel.setJustificationType(juce::Justification::centred);
    peakQLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    peakQLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(peakQLabel);

    highCutLabel.setText("HIGH CUT", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    highCutLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(225, 235, 230));
    highCutLabel.setFont(juce::Font(13.5f, juce::Font::bold));
    addAndMakeVisible(highCutLabel);

    setSize(600, 280);
}

BlackLabEQAudioProcessorEditor::~BlackLabEQAudioProcessorEditor()
{
}

//==============================================================================

void BlackLabEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto background = juce::Colour::fromRGB(72, 130, 122);
    auto panel = juce::Colour::fromRGB(64, 118, 111);
    auto outline = juce::Colour::fromRGBA(255, 255, 255, 35);
    auto text = juce::Colour::fromRGB(228, 236, 232);

    g.fillAll(background);

    auto panelArea = getLocalBounds().reduced(14);
    g.setColour(panel);
    g.fillRoundedRectangle(panelArea.toFloat(), 10.0f);

    g.setColour(outline);
    g.drawRoundedRectangle(panelArea.toFloat(), 10.0f, 1.0f);

    g.setColour(juce::Colour::fromRGBA(255, 255, 255, 28));
    g.drawVerticalLine(128.0f, 78.0f, 215.0f);
    g.drawVerticalLine(470.0f, 78.0f, 215.0f);

    g.setColour(text);
    g.setFont(16.0f);
    g.drawText("BLACKLAB EQ", 0, 12, getWidth(), 22, juce::Justification::centred);
}


std::vector<juce::Component*> BlackLabEQAudioProcessorEditor::getComps()
{
    return
    {
        &lowCutFreqSlider,
        &peakFreqSlider,
        &peakGainSlider,
        &peakQualitySlider,
        &highCutFreqSlider
    };
}

void BlackLabEQAudioProcessorEditor::resized()
{
    const int knobSize = 90;
    const int labelHeight = 20;

    const int lowCutX = 30;
    const int peakFreqX = 145;
    const int peakGainX = 255;
    const int peakQX = 365;
    const int highCutX = 480;

    const int knobY = 48;

    lowCutFreqSlider.setBounds(lowCutX, knobY, knobSize, knobSize);
    peakFreqSlider.setBounds(peakFreqX, knobY, knobSize, knobSize);
    peakGainSlider.setBounds(peakGainX, knobY, knobSize, knobSize);
    peakQualitySlider.setBounds(peakQX, knobY, knobSize, knobSize);
    highCutFreqSlider.setBounds(highCutX, knobY, knobSize, knobSize);

    lowCutLabel.setBounds(lowCutX, knobY + knobSize + 4, knobSize, labelHeight);
    peakFreqLabel.setBounds(peakFreqX, knobY + knobSize + 4, knobSize, labelHeight);
    peakGainLabel.setBounds(peakGainX, knobY + knobSize + 4, knobSize, labelHeight);
    peakQLabel.setBounds(peakQX, knobY + knobSize + 4, knobSize, labelHeight);
    highCutLabel.setBounds(highCutX, knobY + knobSize + 4, knobSize, labelHeight);
}

